#include "mex.h"
#include "redis_conversions.h"
#include "hiredis.h"

/* Params expected:
 *
 * 1) redis context pointer (uint64)
 * 2) redis command (string)
 *
 */         
mxArray * redis_parse_response(redisContext *c, redisReply *reply );

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
     
    void *redisCtx=NULL;
    redisReply *reply;        
    
    // Output
    mxArray  *result;    
        
    // Verify number of inputs
    if(nrhs != 2) {
        mexErrMsgIdAndTxt("Redis:Exec:nrhs",
                "Two inputs required (redisCtxPtr & Redis Command)");
        return;
    }
    
    // Validate redisCtx parameter is a uint64 type
    if(!mxIsUint64(prhs[0])){
        mexErrMsgIdAndTxt("Redis:Exec:notUint64",
                "Redis context pointer must be of type uint64");
        return;
    }      
    
    // Validate command parameter.
    /* check that number of rows in command input argument is 1 */
    if(mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("Redis:Exec:notRowVector",                
                "Redis command input must be a row vector.");
        return;
    }  
                
    // Get redis context
    redisCtx = mPtr_to_cPtr(prhs[0]);     
        
    if(redisCtx == NULL) {
        mexErrMsgIdAndTxt("Redis:Exec:redisCtx",
                "redisCtx is NULL");
        return;
    }   
    
    redisContext *rc = (redisContext *)redisCtx;
        
    // Get command    
    int cmd_length = mxGetN(prhs[1]); // Get length of redis command    
    char cmd[cmd_length+1];    
    mxGetString(prhs[1],&cmd[0],cmd_length+1);              

    reply = (redisReply *) redisCommand(rc, cmd);     
    
    if(reply == NULL || rc->err){            
        mexPrintf("REDIS ERROR (ERROR CODE: %d): %s\n",rc->err, rc->errstr); 
                
        // Attempt reconnection in case connection was lost.
        if(rc->err == REDIS_ERR_IO || rc->err == REDIS_ERR_EOF){                 
           if(redisReconnect(rc) == REDIS_OK){
              printf("REDIS Reconnected! Resending command: \"%s\"...\n\n",cmd);   
              reply = (redisReply *) redisCommand(rc, cmd); 
              if(rc == NULL || rc->err){
                  mexErrMsgIdAndTxt("Redis:Exec:redisCommandAfterReconnect", rc->errstr);           
                  return;
              }              
           }else{
               mexErrMsgIdAndTxt("Redis:Exec:redisCommandReconnect", rc->errstr);
               return;               
           }               
        }      
    }    
    
    // Create output    
    result = redis_parse_response(rc, reply);                    
    
    // Set output
    plhs[0] = mxCreateDoubleScalar(rc->err); // REDIS ERROR CODE
    plhs[1] = mxCreateDoubleScalar(reply->type); // REPLY TYPE
    plhs[2] = result; // REDIS RESPONSE
 
    freeReplyObject(reply);
}


mxArray * redis_parse_response(redisContext *c, redisReply *reply){
                                 
    int index; 
    int ndim = 2;
    int subs[2];
    mxArray *result;
               
    switch(reply->type){        
        case REDIS_REPLY_ERROR:             
        {            
                        
            int nsubs = 1;
            int dims[] = {nsubs, 1};                                    
            result = mxCreateCellArray(ndim, dims);                      
                                    
            mxArray *redis_str = mxCreateString(reply->str);   
                        
            subs[0] = 0;
            subs[1] = 0;
            
            index = mxCalcSingleSubscript(result, ndim, subs);         
            mxSetCell(result, index, redis_str);                   
                                     
            break;           
        }
        case REDIS_REPLY_ARRAY:    
        {
            
            int nsubs = reply->elements;            
            int dims[] = {nsubs, 1};                                    
            result = mxCreateCellArray(ndim, dims);            
            
            for (int j = 0; j < reply->elements; j++) {
                                            
                mxArray *redis_str = mxCreateString(reply->element[j]->str);
               
                subs[0] = j;
                subs[1] = 0;
                                
                index = mxCalcSingleSubscript(result, ndim, subs);                 
                               
                mxSetCell(result, index, redis_str);                                    
            }            
            
            break;
        }
        case REDIS_REPLY_INTEGER:   
        {
            
            int nsubs = 1;
            int dims[] = {nsubs, 1};                                    
            result = mxCreateCellArray(ndim, dims);                      
                                                                      
            mxArray *redis_integer = mxCreateDoubleScalar(reply->integer);
                          
            subs[0] = 0;
            subs[1] = 0;
            
            index = mxCalcSingleSubscript(result, ndim, subs);
            
            mxSetCell(result, index, redis_integer);                               
            
            break;
        }
        case REDIS_REPLY_STRING:
        case REDIS_REPLY_STATUS:
        {
            int nsubs = 1;
            int dims[] = {nsubs, 1};                                    
            result = mxCreateCellArray(ndim, dims);                                            
            
            mxArray *redis_str = mxCreateString(reply->str);                        
            
            subs[0] = 0;
            subs[1] = 0;
            
            index = mxCalcSingleSubscript(result, ndim, subs);            
            
            mxSetCell(result, index, redis_str);                                     
         
            break;
        }        
        case REDIS_REPLY_NIL:
        {            
            result = mxCreateCellArray(0, 0);                   
            break;    
        }
        default:
            result = mxCreateCellArray(0, 0);                      
    }  
    
    return result;
           
    
}
