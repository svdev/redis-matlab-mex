mex -I./hiredis -L./hiredis redis_connect.c redis_conversions.c -lhiredis
mex -I./hiredis -L./hiredis redis_disconnect.c redis_conversions.c -lhiredis
mex -I./hiredis -L./hiredis redis_exec.c redis_conversions.c -lhiredis