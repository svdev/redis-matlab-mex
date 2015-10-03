#include "mex.h"
#include "redis_conversions.h"
#include "hiredis.h"


/* Params expected:
 *
 * 1) HOST
 * 2) PORT
 *
 */         

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
      
    uint32_T port;    
    int host_length;
    mxArray *host_arr;
    void **mexReturn;
    
    // Verify number of inputs
    if(nrhs != 2) {
        mexErrMsgIdAndTxt("Redis:Connect:nrhs",
                "Two inputs required (HOST & PORT)");
        return;
    }
    
    // Validate Host parameter.
    /* check that number of rows in host input argument is 1 */
    if(mxGetM(prhs[0]) != 1) {
        mexErrMsgIdAndTxt("Redis:Connect:notRowVector",                
                "Host input must be a row vector.");
        return;
    } 
    
    // Validate that PORT is scalar
    if( !mxIsDouble(prhs[1]) ||
            mxIsComplex(prhs[1]) ||
            mxGetNumberOfElements(prhs[1]) != 1 ) {
        
            mexErrMsgIdAndTxt("Redis:Connect:notScalar",
                "Port Input  must be a scalar.");
            return;
    }
    
    // Get port value
    port = mxGetScalar(prhs[1]);        
    
    // Validate PORT range
    if( (port > 65536) || 
            (port < 1000) ){
                
            mexErrMsgIdAndTxt("Redis:Connect:invalidPortRange",
                "Port range must be between 1000 and 65536");
            return;
        
    }
            
    // Get host input as C string
    host_length = mxGetN(prhs[0]); // Get length of host parameter
    
    char host[host_length+1];    
    mxGetString(prhs[0],&host[0],host_length+1);            
    
    redisContext *c = redisConnect(host, port);
           
    // Return redis context pointer 
    plhs[0] = cPtr_to_mPtr((void *)c);    
    
    if (c == NULL || c->err) {
        mexPrintf("Error connecting to REDIS SERVER %s:%d: %s\n",host,port,c->errstr);    
      
        return;
    }else{        
        mexPrintf("Connected successfully!\n");
    }
    
}

