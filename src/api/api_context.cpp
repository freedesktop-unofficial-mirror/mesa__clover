#include <OpenCL/cl.h>

#include "core/context.h"
#include "core/device.h"
#include "cpuwinsys/cpuwinsys.h"

// Context APIs

cl_context
clCreateContext(cl_context_properties   properties,
                cl_uint                 num_devices,
                const cl_device_id *    devices,
                logging_fn              pfn_notify,
                void *                  user_data,
                cl_int *                errcode_ret)
{
    cl_context ret_context = NULL;
    cl_device_type type;
    cl_device_id device = devices[0];
    cl_int device_info;

    device_info = clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(type), &type, NULL);
    if (device_info != CL_INVALID_DEVICE) {
        ret_context =  clCreateContextFromType(properties, type,
                pfn_notify, user_data, errcode_ret);
    }

    return ret_context;
}

cl_context
clCreateContextFromType(cl_context_properties   properties,
                        cl_device_type          device_type,
                        logging_fn              pfn_notify,
                        void *                  user_data,
                        cl_int *                errcode_ret)
{
    struct pipe_context *context = NULL;

    switch (device_type) {
    case CL_DEVICE_TYPE_CPU:
        context =
            cl_create_context(cpu_winsys());

        break;
    default:
        if (errcode_ret) {
            *errcode_ret = CL_INVALID_DEVICE_TYPE;
        }
        goto fail;
    }

fail:
    return cl_convert_context(context);
}

cl_int
clRetainContext(cl_context context)
{
    cl_int ret;

    if (context) {
        context->id++;
        ret = CL_SUCCESS;
    } else {
        ret = CL_INVALID_CONTEXT;
    }

    return ret;
}

cl_int
clReleaseContext(cl_context context)
{
    cl_uint ret;

    if (context) {
        if( !context->id ) {
            context->pipe.destroy(&context->pipe);
        } else {
            context->id--;
        }
        ret = CL_SUCCESS;
    } else {
        ret = CL_INVALID_CONTEXT;
    }

    return ret;
}

cl_int
clGetContextInfo(cl_context         context,
                 cl_context_info    param_name,
                 size_t             param_value_size,
                 void *             param_value,
                 size_t *           param_value_size_ret)
{
    return 0;
}
