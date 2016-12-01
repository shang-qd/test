__kernel void hello_kernel(__global const float *a,
						__global const float *b,
						__global float *result)
{
    int gid = get_global_id(0);
	float sum = 0;
	for (int i = 0; i <= 1000; i++)
	{
		float f = (i) / 100.0f;
		sum += sin(f);
	}
    result[gid] =  sum + a[gid] + b[gid];
}
