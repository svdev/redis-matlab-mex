% Connect to REDIS
redisCtxPtr = redis_connect('127.0.0.1',6379);

% Redis command
redis_command = 'LPUSH mylist ITEM_1 ITEM_2 ITEM_3 ITEM_4 ITEM_5 ITEM_6 ITEM_7';

% Execute redis command
[status, replyType, results] = redis_exec(redisCtxPtr, redis_command);

fprintf('Status: %d\nReply type: %d\nResults:\n',status,replyType);
results

% Redis command
redis_command = 'LRANGE mylist 0 -1';

% Execute redis command
[status, replyType, results] = redis_exec(redisCtxPtr, redis_command);

fprintf('Status: %d\nReply type: %d\nResults:\n',status,replyType);
results

% Disconnect from redis server
redis_disconnect(redisCtxPtr);

clear redisCtxPtr ;