#include "async.h"

void AsyncShutdown()
{
	uv_loop_close(uv_default_loop());
}

void AsyncCall(void *data, uv_work_cb callback, uv_after_work_cb callbackDone)
{
	uv_work_t *req = (uv_work_t *)malloc(sizeof(uv_work_t));
	req->data = data;

	if (!callbackDone)
		callbackDone = &AsyncNull;
	uv_queue_work(uv_default_loop(), req, callback, callbackDone);
}

void AsyncNull(uv_work_t* req, int status)
{
	AsyncFree(req);
}

void AsyncFree(uv_work_t* req)
{
	free(req);
}
