//
// Book:      OpenCL(R) Programming Guide
// Authors:   Aaftab Munshi, Benedict Gaster, Timothy Mattson, James Fung, Dan Ginsburg
// ISBN-10:   0-321-74964-2
// ISBN-13:   978-0-321-74964-2
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780132488006/
//            http://www.openclprogrammingguide.com
//

// HelloWorld.cpp
//
//    This is a simple example that demonstrates basic OpenCL setup and
//    use.

#include <iostream>
#include <fstream>
#include <sstream>

#include <chrono>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

///
//  Constants
//
const int ARRAY_SIZE = 1000;

cl_context CreateContext()
{
	cl_uint numPlatforms;
	cl_platform_id firstPlatformId;
	//
	cl_int errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
	if (errNum != CL_SUCCESS || numPlatforms <= 0)
	{
		std::cerr << "Failed to find any OpenCL platforms." << std::endl;
		return NULL;
	}

	cl_context_properties contextProperties[] =
	{
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)firstPlatformId,
		0
	};
	cl_context context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU,
		nullptr, nullptr, &errNum);
	if (errNum != CL_SUCCESS)
	{
		std::cout << "Could not create GPU context, trying CPU..." << std::endl;
		context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU,
			nullptr, nullptr, &errNum);
		if (errNum != CL_SUCCESS)
		{
			std::cerr << "Failed to create an OpenCL GPU or CPU context." << std::endl;
			return nullptr;
		}
	}
	return context;
}

///
//  Create a command queue on the first device available on the
//  context
//
cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device)
{
	size_t deviceBufferSize = -1;
	cl_int errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, nullptr, &deviceBufferSize);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Failed call to clGetContextInfo(...,GL_CONTEXT_DEVICES,...)";
		return nullptr;
	}

	if (deviceBufferSize <= 0)
	{
		std::cerr << "No devices available.";
		return nullptr;
	}

	cl_device_id *devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, nullptr);
	if (errNum != CL_SUCCESS)
	{
		delete[] devices;
		std::cerr << "Failed to get device IDs";
		return nullptr;
	}

	cl_command_queue commandQueue = clCreateCommandQueue(context, devices[0], 0, nullptr);
	if (commandQueue == nullptr)
	{
		delete[] devices;
		std::cerr << "Failed to create commandQueue for device 0";
		return nullptr;
	}

	*device = devices[0];
	delete[] devices;
	return commandQueue;
}


cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName)
{
	cl_int errNum;
	cl_program program;

	std::ifstream kernelFile(fileName, std::ios::in);
	if (!kernelFile.is_open())
	{
		std::cerr << "Failed to open file for reading: " << fileName << std::endl;
		return nullptr;
	}

	std::ostringstream oss;
	oss << kernelFile.rdbuf();

	std::string srcStdStr = oss.str();
	const char *srcStr = srcStdStr.c_str();
	program = clCreateProgramWithSource(context, 1,
		(const char**)&srcStr,
		NULL, NULL);
	if (program == NULL)
	{
		std::cerr << "Failed to create CL program from source." << std::endl;
		return NULL;
	}

	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (errNum != CL_SUCCESS)
	{
		// Determine the reason for the error
		char buildLog[16384];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
			sizeof(buildLog), buildLog, NULL);

		std::cerr << "Error in kernel: " << std::endl;
		std::cerr << buildLog;
		clReleaseProgram(program);
		return NULL;
	}

	return program;
}

bool CreateMemObjects(cl_context context, cl_mem memObjects[3],
	float *a, float *b)
{
	memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		sizeof(float) * ARRAY_SIZE, a, NULL);
	memObjects[1] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		sizeof(float) * ARRAY_SIZE, b, NULL);
	memObjects[2] = clCreateBuffer(context, CL_MEM_READ_WRITE,
		sizeof(float) * ARRAY_SIZE, NULL, NULL);

	if (memObjects[0] == NULL || memObjects[1] == NULL || memObjects[2] == NULL)
	{
		std::cerr << "Error creating memory objects." << std::endl;
		return false;
	}
	return true;
}


void Cleanup(cl_context context, cl_command_queue commandQueue,
	cl_program program, cl_kernel kernel, cl_mem memObjects[3])
{
	if (memObjects != nullptr)
	{
		for (int i = 0; i < 3; i++)
		{
			if (memObjects[i] != 0)
				clReleaseMemObject(memObjects[i]);
		}
	}
	if (commandQueue != nullptr)
		clReleaseCommandQueue(commandQueue);

	if (kernel != nullptr)
		clReleaseKernel(kernel);

	if (program != nullptr)
		clReleaseProgram(program);

	if (context != nullptr)
		clReleaseContext(context);

}

void cpu_fun(float *result, float *a, float *b)
{
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		float sum = 0;
		for (int j = 0; j <= 1000; j++)
		{
			float f = (j) / 100.0f;
			sum += sin(f);
		}
		result[i] = sum + a[i] + b[i];
	}
}

void test_opencl()
{
	// 创建上下文，不管他是CPU还是GPU
	cl_context context = CreateContext();
	if (context == nullptr)
	{
		std::cerr << "上下文创建失败." << std::endl;
		return;
	}

	// 在设备上创建命令队列
	cl_device_id device = nullptr;
	cl_command_queue commandQueue = CreateCommandQueue(context, &device);
	if (commandQueue == nullptr)
	{
		Cleanup(context, commandQueue, nullptr, nullptr, nullptr);
		return;
	}

	// 在设备上创建程序,如果程序的语法错误在这里将会出现错误
	cl_program program = CreateProgram(context, device, "HelloWorld.cl");
	if (program == nullptr)
	{
		Cleanup(context, commandQueue, program, nullptr, nullptr);
		return;
	}

	// 用程序创建内核对象
	cl_kernel kernel = clCreateKernel(program, "hello_kernel", nullptr);
	if (kernel == nullptr)
	{
		std::cerr << "内核对象创建失败" << std::endl;
		Cleanup(context, commandQueue, program, kernel, nullptr);
		return;
	}

	float *result = new float[ARRAY_SIZE];
	float *a = new float[ARRAY_SIZE];
	float *b = new float[ARRAY_SIZE];
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		a[i] = (float)i;
		b[i] = (float)(i * 2);
	}

	// 创建内存对象
	cl_mem memObjects[3] = { nullptr, nullptr, nullptr };
	if (!CreateMemObjects(context, memObjects, a, b))
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return;
	}

	// 设置参数
	cl_int errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObjects[0]);
	errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &memObjects[1]);
	errNum |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &memObjects[2]);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "设置参数失败" << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return;
	}

	size_t globalWorkSize[1] = { ARRAY_SIZE };
	size_t localWorkSize[1] = { 1 };

	// 时间库
	using namespace std;
	using namespace chrono;
	auto begin = std::chrono::system_clock::now();
	
	for (int i = 0; i < 500; i++)
	{
		// 开始执行
		errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 1, nullptr,
			globalWorkSize, localWorkSize, 0, nullptr, nullptr);
		if (errNum != CL_SUCCESS)
		{
			std::cerr << "错误的内核." << std::endl;
			Cleanup(context, commandQueue, program, kernel, memObjects);
			return;
		}

		// 读取内存结果
		errNum = clEnqueueReadBuffer(commandQueue, memObjects[2], CL_TRUE,
			0, ARRAY_SIZE * sizeof(float), result, 0, nullptr, nullptr);
		if (errNum != CL_SUCCESS)
		{
			std::cerr << "读取结果失败." << std::endl;
			Cleanup(context, commandQueue, program, kernel, memObjects);
			return;
		}
	}

	auto end = system_clock::now();
	auto duration = duration_cast<microseconds>(end - begin);
	cout << "opencl花费了" << (duration.count() * microseconds::period::num) / microseconds::period::den << "秒" << endl;
	float sum = 0;

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		sum += result[i];
	}
	std::cout << sum << std::endl;

	begin = std::chrono::system_clock::now();

#pragma omp parallel for
	for (int i = 0; i < 500; i++)
	{
		cpu_fun(result, a, b);
	}
	end = system_clock::now();
	duration = duration_cast<microseconds>(end - begin);
	cout << "cpu花费了" << (duration.count() * microseconds::period::num) / microseconds::period::den << "秒" << endl;

	// 输出结果
	sum = 0;
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		sum += result[i];
		//std::cout << result[i] << " ";
	}
	std::cout << sum << std::endl;

	std::cout << "执行成功." << std::endl;
	Cleanup(context, commandQueue, program, kernel, memObjects);
}
