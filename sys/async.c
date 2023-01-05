#include "async.h"

async_handler asyncHandler = { 0 };

void AsyncInit()
{
	if (asyncHandler.workers) return;
	asyncHandler.workers = (async_worker*)malloc(sizeof(async_worker));
	asyncHandler.workers->next = NULL;
}

uv_loop_t* AsyncLoopCreate()
{
	uv_loop_t* loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
	uv_loop_init(loop);
	return loop;
}

int AsyncLoopRun(uv_loop_t* loop)
{
	return uv_run(loop, UV_RUN_DEFAULT);
}

void AsyncLoopStop(uv_loop_t* loop)
{
	if (!loop || !uv_loop_alive(loop)) return;
	uv_stop(loop);
}

void AsyncLoopFree(uv_loop_t* loop)
{
	if (!loop) return;
	free(loop);
}

async_worker* AsyncWorker(void* data, uv_work_cb callback, uv_after_work_cb afterCallback, uv_loop_t *loop)
{
	uv_work_t* req = (uv_work_t*)malloc(sizeof(uv_work_t));
	async_worker* worker = (async_worker*)malloc(sizeof(async_worker));

	worker->status = ASYNC_PENDING;
	worker->req = req;
	worker->loop = loop ? loop : uv_default_loop();
	worker->thread = NULL;
	worker->data = data;
	worker->running = qtrue;
	worker->next = asyncHandler.workers->next;

	asyncHandler.workers->next = worker;
	req->data = worker;

	uv_queue_work(worker->loop, req, callback, afterCallback);

	return worker;
}

void* AsyncWorkerData(uv_work_t* req)
{
	async_worker* worker = (async_worker*)req->data;
	return worker->data;
}

void AsyncWorkerDone(uv_work_t* req, async_status status)
{
	async_worker* worker = (async_worker*)req->data;
	worker->status = status;
	worker->running = qfalse;
}

void AsyncWorkerCancel(async_worker* worker)
{
	if (!worker)
		return;
	worker->status = ASYNC_CANCEL;
}

void AsyncWorkerFree(async_worker* worker)
{
	if (!worker)
		return;
	if (worker->data)
		free(worker->data);
	if (worker->req)
		free(worker->req);
}

void AsyncShutdown()
{
	if (!asyncHandler.workers) return;

	async_worker* worker = asyncHandler.workers->next;
	while (worker)
	{
		AsyncLoopStop(worker->loop);
		AsyncWorkerCancel(worker);
		worker = worker->next;
	}

	worker = asyncHandler.workers->next;
	while (worker)
	{
		while (worker->running)
			uv_sleep(50);

		AsyncWorkerFree(worker);

		async_worker* prev = worker;
		worker = worker->next;
		free(prev);
	}
	free(asyncHandler.workers);
	asyncHandler.workers = NULL;
}
