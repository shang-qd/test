#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <CL/cl.h>

const char  cl_kernel_histogram_filename[]    = "histogram_image.cl";

// 每个工作项（线程）要处理的数据数
const int num_pixels_per_work_item = 32;
static int num_iterations = 10;

// 随机生成 unsigned char 数据
static void* create_image_data_unorm8(int w, int h)
{
    unsigned char  *p = (unsigned char *)malloc(w * h * 4);
	for (int i = 0; i < w * h * 4; i++)
	{
		p[i] = (unsigned char)(rand() & 0xFF);
	}
    return (void *)p;
}

// CPU 执行直方图统计
static void * generate_reference_histogram_results_unorm8(void *image_data, int w, int h)
{
    unsigned int    *ref_histogram_results = (unsigned int *)malloc(256 * 3 * sizeof(unsigned int));
    unsigned char   *img = (unsigned char *)image_data;
    unsigned int    *ptr = ref_histogram_results;

    memset(ref_histogram_results, 0x0, 256 * 3 * sizeof(unsigned int));
    for (int i = 0; i < w * h * 4; i += 4)
    {
        int indx = img[i];
        ptr[indx]++;
    }
    ptr += 256;
    for (int i = 1; i < w * h * 4; i+=4)
    {
        int indx = img[i];
        ptr[indx]++;
    }

    ptr += 256;
    for (int i = 2; i < w * h * 4; i += 4)
    {
        int indx = img[i];
        ptr[indx]++;
    }
    return ref_histogram_results;
}

// 随机生成float数据
static void * create_image_data_fp32(int w, int h)
{
    float *p = (float*)malloc(w * h * 4 * sizeof(float));
	for (int i = 0; i < w * h * 4; i++)
	{
		p[i] = (float)rand() / (float)RAND_MAX;
	}
    return (void *)p;
}

// 计算浮点数直方图
static void * generate_reference_histogram_results_fp32(void *image_data, int w, int h)
{
    unsigned int *ref_histogram_results = (unsigned int *)malloc(257 * 3 * sizeof(unsigned int));
    float        *img = (float *)image_data;
    unsigned int *ptr = ref_histogram_results;
    memset(ref_histogram_results, 0x0, 257 * 3 * sizeof(unsigned int));
    for (int i = 0; i < w * h * 4; i+=4)
    {
        float           f = img[i];
        unsigned int    indx;
		if (f > 1.0f)
		{
			f = 1.0f;
		}
        f *= 256.0f;
        indx = (unsigned int)f;
        ptr[indx]++;
    }
    ptr += 257;
    for (int i = 1; i < w * h * 4; i += 4)
    {
        float f = img[i];
        unsigned int indx;
		if (f > 1.0f)
		{
			f = 1.0f;
		}
        f *= 256.0f;
        indx = (unsigned int)f;
        ptr[indx]++;
    }

    ptr += 257;
    for (int i = 2; i < w * h * 4; i += 4)
    {
        float f = img[i];
        unsigned int indx;
		if (f > 1.0f)
		{
			f = 1.0f;
		}
        f *= 256.0f;
        indx = (unsigned int)f;
        ptr[indx]++;
    }
    return ref_histogram_results;
}

// 验证结果
static int verify_histogram_results(const char *str, unsigned int *histogram_results, unsigned int *ref_histogram_results, int num_entries)
{
    for (int i = 0; i < num_entries; i++)
    {
        if (histogram_results[i] != ref_histogram_results[i])
        {
            printf("%s: 验证失败 indx = %d, opencl result = %d, expected result = %d\n", 
				str, i, histogram_results[i], ref_histogram_results[i]);
            return -1;
        }
    }
    printf("%s: 验证成功！\n", str);
    return 0;
}

// 读文件
static int read_kernel_from_file(const char *filename, char **source, size_t *len)
{
    struct stat statbuf;
    FILE        *fh;
    size_t      file_len;
    
    fh = fopen(filename, "r");
    if (fh == 0)
        return -1;
    
    stat(filename, &statbuf);
    file_len = (size_t)statbuf.st_size;
    *len = file_len;
    *source = (char *) malloc(file_len+1);
    fread(*source, file_len, 1, fh);
    (*source)[file_len] = '\0';
    
    fclose(fh);
    return 0;
}

const int image_width = 1920;
const int image_height = 1080;

int test_histogram(cl_context context, cl_command_queue queue, cl_device_id device)
{
    srand(0);
	char *source[1];
	size_t src_len[1];
	int err = read_kernel_from_file(cl_kernel_histogram_filename, &source[0], &src_len[0]);
    if (err)
    {
        printf("read_kernel_from_file() 失败. (%s) 文件没有找到 \n", cl_kernel_histogram_filename);
        return EXIT_FAILURE;
    }
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)source, (size_t *)src_len, &err);
    if(!program || err)
    {
        printf("clCreateProgramWithSource() 失败. (%d)\n", err);
        return EXIT_FAILURE;
    }
    free(source[0]);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        char    buffer[2048] = "";
        printf("clBuildProgram() 失败.\n");
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, NULL);
        printf("Log:\n%s\n", buffer);
        return EXIT_FAILURE;
    }
    
    cl_kernel histogram_rgba_unorm8 = clCreateKernel(program, "histogram_image_rgba_unorm8", &err);
    if(!histogram_rgba_unorm8 || err)
    {
        printf("clCreateKernel() 失败. histogram_rgba_unorm8(). (%d)\n", err);
        return EXIT_FAILURE;
    }
    cl_kernel histogram_rgba_fp = clCreateKernel(program, "histogram_image_rgba_fp", &err);
    if(!histogram_rgba_fp || err)
    {
        printf("clCreateKernel() 失败. histogram_image_rgba_fp(). (%d)\n", err);
        return EXIT_FAILURE;
    }
    cl_kernel histogram_sum_partial_results_unorm8 = clCreateKernel(program, "histogram_sum_partial_results_unorm8", &err);
    if(!histogram_sum_partial_results_unorm8 || err)
    {
        printf("clCreateKernel() 失败. histogram_sum_partial_results_unorm8(). (%d)\n", err);
        return EXIT_FAILURE;
    }
    cl_kernel histogram_sum_partial_results_fp = clCreateKernel(program, "histogram_sum_partial_results_fp", &err);
    if(!histogram_sum_partial_results_fp || err)
    {
        printf("clCreateKernel() 失败. histogram_sum_partial_results_fp(). (%d)\n", err);
        return EXIT_FAILURE;
    }

    cl_mem histogram_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 257*3*sizeof(unsigned int), NULL, &err);
    if (!histogram_buffer || err)
    {
        printf("clCreateBuffer() 失败. (%d)\n", err);
        return EXIT_FAILURE;
    }

	cl_image_format  image_format;
    image_format.image_channel_order = CL_RGBA;
    image_format.image_channel_data_type = CL_UNORM_INT8;
    void *image_data_unorm8 = create_image_data_unorm8(image_width, image_height);
    cl_mem input_image_unorm8 = clCreateImage2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  
                                            &image_format, image_width, image_height, 0, image_data_unorm8, &err);
    if (!input_image_unorm8 || err)
    {
        printf("clCreateImage2D() 失败. (%d)\n", err);
        return EXIT_FAILURE;
    }
    image_format.image_channel_order = CL_RGBA;
    image_format.image_channel_data_type = CL_FLOAT;
    void *image_data_fp32 = create_image_data_fp32(image_width, image_height);
    cl_mem input_image_fp32 = clCreateImage2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  
                                            &image_format, image_width, image_height, 0, image_data_fp32, &err);
    if (!input_image_fp32 || err)
    {
        printf("clCreateImage2D() 失败. (%d)\n", err);
        return EXIT_FAILURE;
    }
    
    /************  Testing RGBA 8-bit histogram **********/
	size_t global_work_size[2];
	size_t local_work_size[2];
	size_t workgroup_size, num_groups;
    clGetKernelWorkGroupInfo(histogram_rgba_unorm8, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &workgroup_size, NULL);
    {
        size_t  gsize[2];
        int     w;
        
        if (workgroup_size <= 256)
        {
            gsize[0] = 16;
            gsize[1] = workgroup_size / 16;
        }
        else if (workgroup_size <= 1024)
        {
            gsize[0] = workgroup_size / 16;
            gsize[1] = 16;
        }
        else
        {
            gsize[0] = workgroup_size / 32;
            gsize[1] = 32;
        }
        
        local_work_size[0] = gsize[0];
        local_work_size[1] = gsize[1];
        // 每个线程处理num_pixels_per_work_item个数据
        w = (image_width + num_pixels_per_work_item - 1) / num_pixels_per_work_item;
        global_work_size[0] = ((w + gsize[0] - 1) / gsize[0]);
        global_work_size[1] = ((image_height + gsize[1] - 1) / gsize[1]);

        num_groups = global_work_size[0] * global_work_size[1];
        global_work_size[0] *= gsize[0];
        global_work_size[1] *= gsize[1];
    }

    cl_mem partial_histogram_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, num_groups*257*3*sizeof(unsigned int), NULL, &err);
    if (!partial_histogram_buffer || err)
    {
        printf("clCreateBuffer() 失败. (%d)\n", err);
        return EXIT_FAILURE;
    }

    clSetKernelArg(histogram_rgba_unorm8, 0, sizeof(cl_mem), &input_image_unorm8);
    clSetKernelArg(histogram_rgba_unorm8, 1, sizeof(int), &num_pixels_per_work_item);
    clSetKernelArg(histogram_rgba_unorm8, 2, sizeof(cl_mem), &partial_histogram_buffer);
    
    clSetKernelArg(histogram_sum_partial_results_unorm8, 0, sizeof(cl_mem), &partial_histogram_buffer);
    clSetKernelArg(histogram_sum_partial_results_unorm8, 1, sizeof(int), &num_groups);
    clSetKernelArg(histogram_sum_partial_results_unorm8, 2, sizeof(cl_mem), &histogram_buffer);

	// 用二维数据调用核函数
    err = clEnqueueNDRangeKernel(queue, histogram_rgba_unorm8, 2, NULL, global_work_size, local_work_size, 0, NULL, NULL);
    if (err)
    {
        printf("clEnqueueNDRangeKernel() 失败 histogram_rgba_unorm8 kernel. (%d)\n", err);
        return EXIT_FAILURE;
    }
    
    clGetKernelWorkGroupInfo(histogram_sum_partial_results_unorm8, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &workgroup_size, NULL);
    if (workgroup_size < 256)
    {
        printf("CL_KERNEL_WORK_GROUP_SIZE最小需要256个线程的支持. (%d)\n", (int)workgroup_size);
        return EXIT_FAILURE;
    }
	size_t partial_global_work_size[2];
	size_t partial_local_work_size[2];
    partial_global_work_size[0] = 256*3;
    partial_local_work_size[0] = (workgroup_size > 256) ? 256 : workgroup_size;
	// 用1维的方式调用核函数
    err = clEnqueueNDRangeKernel(queue, histogram_sum_partial_results_unorm8, 1, NULL, partial_global_work_size, partial_local_work_size, 0, NULL, NULL);
    if (err)
    {
        printf("clEnqueueNDRangeKernel() 失败. histogram_sum_partial_results_unorm8. (%d)\n", err);
        return EXIT_FAILURE;
    }
	unsigned int* ref_histogram_results = (unsigned int *)generate_reference_histogram_results_unorm8(image_data_unorm8, image_width, image_height);
	unsigned int* histogram_results = (unsigned int *)malloc(257 * 3 * sizeof(unsigned int));
    err = clEnqueueReadBuffer(queue, histogram_buffer, CL_TRUE, 0, 256*3*sizeof(unsigned int), histogram_results, 0, NULL, NULL);
    if (err)
    {
        printf("clEnqueueReadBuffer() 失败. (%d)\n", err);
        return EXIT_FAILURE;
    }
	// 验证CPU和opencl数据的一致性
    verify_histogram_results("Image Histogram for image type = CL_RGBA, CL_UNORM_INT8", histogram_results, ref_histogram_results, 256*3);   
    
    /************  Testing RGBA 32-bit fp histogram **********/
    clGetKernelWorkGroupInfo(histogram_rgba_fp, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &workgroup_size, NULL);
    {
        size_t  gsize[2];
        int     w;
        
        if (workgroup_size <= 256)
        {
            gsize[0] = 16;
            gsize[1] = workgroup_size / 16;
        }
        else if (workgroup_size <= 1024)
        {
            gsize[0] = workgroup_size / 16;
            gsize[1] = 16;
        }
        else
        {
            gsize[0] = workgroup_size / 32;
            gsize[1] = 32;
        }
        
        local_work_size[0] = gsize[0];
        local_work_size[1] = gsize[1];
        
        w = (image_width + num_pixels_per_work_item - 1) / num_pixels_per_work_item;
        global_work_size[0] = ((w + gsize[0] - 1) / gsize[0]);
        global_work_size[1] = ((image_height + gsize[1] - 1) / gsize[1]);

        num_groups = global_work_size[0] * global_work_size[1];    
        global_work_size[0] *= gsize[0];
        global_work_size[1] *= gsize[1];
    }
    partial_histogram_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, num_groups*257*3*sizeof(unsigned int), NULL, &err);
    if (!partial_histogram_buffer || err)
    {
        printf("clCreateBuffer() failed. (%d)\n", err);
        return EXIT_FAILURE;
    }
    clSetKernelArg(histogram_rgba_fp, 0, sizeof(cl_mem), &input_image_fp32);
    clSetKernelArg(histogram_rgba_fp, 1, sizeof(int), &num_pixels_per_work_item);
    clSetKernelArg(histogram_rgba_fp, 2, sizeof(cl_mem), &partial_histogram_buffer);
    
    clSetKernelArg(histogram_sum_partial_results_fp, 0, sizeof(cl_mem), &partial_histogram_buffer);
    clSetKernelArg(histogram_sum_partial_results_fp, 1, sizeof(int), &num_groups);
    clSetKernelArg(histogram_sum_partial_results_fp, 2, sizeof(cl_mem), &histogram_buffer);
    
    err = clEnqueueNDRangeKernel(queue, histogram_rgba_fp, 2, NULL, global_work_size, local_work_size, 0, NULL, NULL);
    if (err)
    {
        printf("clEnqueueNDRangeKernel() 失败 histogram_rgba_fp (%d)\n", err);
        return EXIT_FAILURE;
    }

    clGetKernelWorkGroupInfo(histogram_sum_partial_results_fp, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &workgroup_size, NULL);
    if (workgroup_size < 256)
    {
        printf("CL_KERNEL_WORK_GROUP_SIZE最小需要256的支持 (%d)\n", (int)workgroup_size);
        return EXIT_FAILURE;
    }
    partial_global_work_size[0] = 256*3;
    partial_local_work_size[0] = (workgroup_size > 256) ? 256 : workgroup_size;
    err = clEnqueueNDRangeKernel(queue, histogram_sum_partial_results_fp, 1, NULL, partial_global_work_size, partial_local_work_size, 0, NULL, NULL);
    if (err)
    {
        printf("clEnqueueNDRangeKernel() 失败 histogram_sum_partial_results_fp. (%d)\n", err);
        return EXIT_FAILURE;
    }

    ref_histogram_results = (unsigned int *)generate_reference_histogram_results_fp32(image_data_fp32, image_width, image_height);
    err = clEnqueueReadBuffer(queue, histogram_buffer, CL_TRUE, 0, 257*3*sizeof(unsigned int), histogram_results, 0, NULL, NULL);
    if (err)
    {
        printf("clEnqueueReadBuffer() 失败. (%d)\n", err);
        return EXIT_FAILURE;
    }
    verify_histogram_results("Image Histogram for image type = CL_RGBA, CL_FLOAT", histogram_results, ref_histogram_results, 257*3);

    free(ref_histogram_results);
    free(histogram_results);
    free(image_data_unorm8);
    free(image_data_fp32);
    
    clReleaseKernel(histogram_rgba_unorm8);
    clReleaseKernel(histogram_rgba_fp);
    clReleaseKernel(histogram_sum_partial_results_unorm8);
    clReleaseKernel(histogram_sum_partial_results_fp);
    
    clReleaseProgram(program);    
    clReleaseMemObject(partial_histogram_buffer);
    clReleaseMemObject(histogram_buffer);
    clReleaseMemObject(input_image_unorm8);
    clReleaseMemObject(input_image_fp32);
    return EXIT_SUCCESS;
}

int test_histogram()
{
    cl_device_id        device;
    cl_context          context;
    cl_command_queue    queue;
    cl_device_type      device_type = CL_DEVICE_TYPE_CPU;
    cl_platform_id platform = NULL;
    int err = clGetPlatformIDs(1, &platform, NULL);
    if(err != CL_SUCCESS)
    {
        printf("clGetPlatformIDs() failed. (%d)\n", err);
        return EXIT_FAILURE;
    }
    err = clGetDeviceIDs(platform, device_type, 1, &device, NULL);
    if(err != CL_SUCCESS)
    {
        printf("clGetDeviceIDs() failed. (%d)\n", err);
        return EXIT_FAILURE;
    }
    char    deviceName[512];
    char    deviceVendor[512];
    char    deviceVersion[512];
    err = clGetDeviceInfo(device, CL_DEVICE_VENDOR, sizeof( deviceVendor ),
                           deviceVendor, NULL);
    err |= clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof( deviceName ),
                          deviceName, NULL);
    err |= clGetDeviceInfo(device, CL_DEVICE_VERSION, sizeof( deviceVersion ),
                           deviceVersion, NULL);

    printf("OpenCL Device Vendor = %s,  OpenCL Device Name = %s,  OpenCL Device Version = %s\n", deviceVendor, deviceName, deviceVersion);

    size_t ext_size = 0;
    err |= clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, 0, NULL, &ext_size);
    if (err) 
	{
        printf("clGetDeviceInfo() failed. (%d)\n", err);
        return EXIT_FAILURE;
    }
    char* ext_string = (char*) malloc(ext_size+1);
    clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, ext_size+1, ext_string, NULL);
    if (ext_string == NULL) 
	{
        printf("clGetDeviceInfo() failed. (%d)\n", err);
        return EXIT_FAILURE;
    }

    if (!strstr(ext_string, "cl_khr_local_int32_base_atomics")) 
	{
        free(ext_string);
        printf("Skipping: histogram requires local atomics support\n");
        return EXIT_SUCCESS;
    }
    free(ext_string);
    context = clCreateContext( 0, 1, &device, NULL, NULL, &err);
    if (!context || err)
    {
        printf("clCreateContext() failed. (%d)\n", err);
        return EXIT_FAILURE;
    }
    queue = clCreateCommandQueue( context, device, CL_QUEUE_PROFILING_ENABLE, &err);
    if(!queue || err)
    {
        printf("clCreateCommandQueue() failed. (%d)\n", err);
        return EXIT_FAILURE;
    }    
    if (test_histogram(context, queue, device) == EXIT_FAILURE)
        return EXIT_FAILURE;
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    return EXIT_SUCCESS;
}