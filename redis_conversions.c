#include "redis_conversions.h"

// Converts a C pointer to a uint64 to be stored in MATLAB 
mxArray* cPtr_to_mPtr(void* handle) {
    mxArray* ret;
    void** output;
    
    if (sizeof(void*) == 4) {
        ret = mxCreateNumericMatrix(1, 1, mxUINT32_CLASS, mxREAL);
    } else {
        ret = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
    }

    output = (void**) mxGetData(ret);
    output[0] = handle;

    return ret;
}


// Converts a pointer passed from MATLAB to a C void pointer
void* mPtr_to_cPtr(const mxArray* param) {
    void** input = NULL;
    void* output = NULL;

    input = (void**) mxGetData(param);
    output = (void*) input[0];

    if (output == NULL) {
        mexErrMsgIdAndTxt("Redis:mPtr_to_cPtr", "ERROR: Invalid pointer!");
    }

    return output;
}