# redis-mex

MEX functions to interact with a Redis server from MATLAB.

## Installation

Install HIREDIS.

- cd hiredis
- make & make install

## Compile Mex functions

(From matlab prompt)

- compile

## Usage

1) Connect to Redis

redisCtxPtr = redis_connect('127.0.0.1',6379);

2) Execute Redis command

[status, replyType, results] = redis_exec(redisCtxPtr, 'SET myKey myValue');

3) Disconnect from Redis

redis_disconnect(redisCtxPtr);


