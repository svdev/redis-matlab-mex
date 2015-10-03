#include "mex.h"
#include "redis_conversions.h"
#include "hiredis.h"

/* Params expected:
 *
 * 1) redis context pointer 
 *
 */         

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{                     
    void *redisCtx = NULL;
    
    // Verify number of inputs
    if(nrhs != 1) {
        mexErrMsgIdAndTxt("Redis:Disconnect:nrhs",
                "Only one input allowed (redis ctx pointer)");
        return;
    }
        
    // Get pointer passed from matlab to C redis context pointer
    redisCtx = mPtr_to_cPtr(prhs[0]);
    
    if(redisCtx != NULL){
        redisFree(redisCtx);
    }            
    
}
