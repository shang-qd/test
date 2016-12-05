// 启动对浮点数的原子操作
#pragma OPENCL EXTENSION cl_khr_local_int32_base_atomics : enable

// ------------------操作整数------------

// 合并数据，本函数在当前任务中共启动256 个线程，每个线程处理34个数据
kernel void histogram_sum_partial_results_unorm8(global uint *partial_histogram, int num_groups, global uint *histogram)
{
    int     tid = (int)get_global_id(0);
    int     group_indx;
    int     n = num_groups;
    uint    tmp_histogram;
    tmp_histogram = partial_histogram[tid];
    group_indx = 256*3;
    while (--n > 0)
    {
        tmp_histogram += partial_histogram[group_indx + tid];
        group_indx += 256*3;
    }  
    histogram[tid] = tmp_histogram;
}

kernel void histogram_image_rgba_unorm8(image2d_t img, int num_pixels_per_workitem, global uint *histogram)
{
    int     local_size = (int)get_local_size(0) * (int)get_local_size(1);
    int     image_width = get_image_width(img);
    int     image_height = get_image_height(img);
    int     group_indx = mad24(get_group_id(1), get_num_groups(0), get_group_id(0)) * 256 * 3;
    int     x = get_global_id(0);
    int     y = get_global_id(1);
    
    local uint  tmp_histogram[256 * 3];
        
    int     tid = mad24(get_local_id(1), get_local_size(0), get_local_id(0));
    int     j = 256 * 3;
    int     indx = 0;
    
    // 在共享内存中设置初始值，每个线程只负责他们自己的数据区
    do
    {
        if (tid < j)
            tmp_histogram[indx+tid] = 0;

        j -= local_size;
        indx += local_size;
    } while (j > 0);

	// 同步函数    
    barrier(CLK_LOCAL_MEM_FENCE);
    // 每个线程处理num_pixels_per_workitem多个数据，在host端已经设置好了
    int     i, idx;
    for (i=0, idx=x; i<num_pixels_per_workitem; i++, idx+=get_global_size(0))
    {
        if ((idx < image_width) && (y < image_height))
        {                
            float4 clr = read_imagef(img, CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST, (float2)(idx, y));
    
            uchar   indx_x, indx_y, indx_z;
            indx_x = convert_uchar_sat(clr.x * 255.0f);
            indx_y = convert_uchar_sat(clr.y * 255.0f);
            indx_z = convert_uchar_sat(clr.z * 255.0f);
            atom_inc(&tmp_histogram[indx_x]);
            atom_inc(&tmp_histogram[256+(uint)indx_y]);
            atom_inc(&tmp_histogram[512+(uint)indx_z]);
        }
    }

    barrier(CLK_LOCAL_MEM_FENCE);
	// 把共享内存的数据更新到全局内存中
    j = 256 * 3;
    indx = 0;
    do
    {
		if (tid < j)
			histogram[group_indx + indx + tid] = tmp_histogram[indx + tid];        
		j -= local_size;
		indx += local_size;
     } while (j > 0);
}

// ----------------操作浮点数------------------------------
kernel void histogram_sum_partial_results_fp(global uint *partial_histogram, int num_groups, global uint *histogram)
{
    int     tid = (int)get_global_id(0);
    int     group_id = (int)get_group_id(0);
    int     group_indx;
    int     n = num_groups;
    uint    tmp_histogram, tmp_histogram_first;

    int     first_workitem_not_in_first_group = ((get_local_id(0) == 0) && group_id);
    
    tid += group_id;
    int     tid_first = tid - 1;
    if (first_workitem_not_in_first_group)
        tmp_histogram_first = partial_histogram[tid_first];
    
    tmp_histogram = partial_histogram[tid];
    
    group_indx = 257*3;
    while (--n > 0)
    {
        if (first_workitem_not_in_first_group)
            tmp_histogram_first += partial_histogram[tid_first];
            
        tmp_histogram += partial_histogram[group_indx+tid];
        group_indx += 257*3;
    }
    
    if (first_workitem_not_in_first_group)
        histogram[tid_first] = tmp_histogram_first;
    histogram[tid] = tmp_histogram;
}


kernel void histogram_image_rgba_fp(image2d_t img, int num_pixels_per_workitem, global uint *histogram)
{
    int     local_size = (int)get_local_size(0) * (int)get_local_size(1);
    int     image_width = get_image_width(img);
    int     image_height = get_image_height(img);
    int     group_indx = mad24(get_group_id(1), get_num_groups(0), get_group_id(0)) * 257 * 3;
    int     x = get_global_id(0);
    int     y = get_global_id(1);
    
    local uint  tmp_histogram[257 * 3];
        
    int     tid = mad24(get_local_id(1), get_local_size(0), get_local_id(0));
    int     j = 257 * 3;
    int     indx = 0;
    
    do
    {
        if (tid < j)
            tmp_histogram[indx+tid] = 0;

        j -= local_size;
        indx += local_size;
    } while (j > 0);
    
    barrier(CLK_LOCAL_MEM_FENCE);
    
    int     i, idx;
    for (i=0, idx=x; i<num_pixels_per_workitem; i++, idx+=get_global_size(0))
    {
        if ((idx < image_width) && (y < image_height))
        {                
            float4 clr = read_imagef(img, CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST, (float2)(idx, y));
    
            ushort   indx;
            indx = convert_ushort_sat(min(clr.x, 1.0f) * 256.0f);
            atom_inc(&tmp_histogram[indx]);

            indx = convert_ushort_sat(min(clr.y, 1.0f) * 256.0f);
            atom_inc(&tmp_histogram[257+indx]);

            indx = convert_ushort_sat(min(clr.z, 1.0f) * 256.0f);
            atom_inc(&tmp_histogram[514+indx]);
        }
    }
    
    barrier(CLK_LOCAL_MEM_FENCE);

	j = 257 * 3;
	indx = 0;
	do
	{
		if (tid < j)
                histogram[group_indx + indx + tid] = tmp_histogram[indx + tid];        
		j -= local_size;
		indx += local_size;
	} while (j > 0);
    
}