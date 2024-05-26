#pragma once
#include "CGSC/cgsc.h"

#include <uv.h>

typedef enum
{
	ASYNC_NULL,
	ASYNC_PENDING,
	ASYNC_SUCCESSFUL,
	ASYNC_FAILURE,
	ASYNC_CANCEL
} async_status;

typedef struct async_worker_s
{
	async_status status;
	uv_work_t *req;
	uv_loop_t *loop;
	uv_thread_t thread;
	qboolean running;
	void *data;
	struct async_worker_s *next;
} async_worker;

typedef struct
{
	async_worker *workers;
	uv_loop_t *loop;
} async_handler;

/// @brief Initialize the async handler.
EXPORT(async_handler *, AsyncInit());

/// @brief Create an async loop.
/// @return
EXPORT(uv_loop_t *, AsyncLoopCreate());

/// @brief Run the async loop.
/// @param loop - The loop.
/// @return
EXPORT(int, AsyncLoopRun(uv_loop_t *loop));

/// @brief Stop the async loop.
/// @param loop - The loop.
EXPORT(void, AsyncLoopStop(uv_loop_t *loop));

/// @brief Free and stop the async loop.
/// @param loop - The loop.
EXPORT(void, AsyncLoopFree(uv_loop_t *loop));

/// @brief Create a new async worker.
/// @param handler - The async handler.
/// @param data - The data.
/// @param callback - The async loop.
/// @param afterCallback - The after callback.
/// @return
EXPORT(async_worker *,
	AsyncWorker(async_handler *handler, void *data, uv_work_cb callback, uv_after_work_cb afterCallback));

/// @brief Get the worker data.
/// @param req - The worker.
/// @return
EXPORT(void *, AsyncWorkerData(uv_work_t *req));

/// @brief Set the worker status and stop running.
/// @param req - The worker.
/// @param status - The worker done status.
EXPORT(void, AsyncWorkerDone(uv_work_t *req, async_status status));

/// @brief Cancel a worker.
/// @param req - The worker.
EXPORT(void, AsyncWorkerCancel(async_worker *worker));

/// @brief Free a worker.
/// @param req - The worker.
EXPORT(void, AsyncWorkerFree(async_worker *worker));

/// @brief Shutdown the async handler by canceling any pending requests and
/// waiting for all threads to complete.
/// @param handler - The async handler.
EXPORT(void, AsyncShutdown(async_handler *handler));
