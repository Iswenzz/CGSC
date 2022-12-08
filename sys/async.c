#include "async.h"
#include <uv.h>

void Async_HandlerInitialize()
{
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}
