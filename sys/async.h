#pragma once
#include "../cgsc.h"
#include <uv.h>

typedef enum
{
	ASYNC_NULL,
	ASYNC_PENDING,
	ASYNC_SUCCESSFUL,
	ASYNC_FAILURE,
} async_status;

/// @brief Shutdown all async operations, this should be called when the server shutdown.
EXPORT(void, AsyncShutdown());

/// @brief Make an async call with the provided callbacks and data.
/// @param data - The data to pass.
/// @param callback - The async callback.
/// @param callbackDone - The callback when the async operation is completed.
/// @return The worker pointer.
EXPORT(uv_work_t *, AsyncCall(void *data, uv_work_cb callback, uv_after_work_cb callbackDone));

/// @brief Default worker completion callback, this default callback calls AsyncFree.
/// @param req - The worker request.
/// @param status - The request status.
EXPORT(void, AsyncNull(uv_work_t* req, int status));

/// @brief Free a worker request.
/// @param req - The worker request.
EXPORT(void, AsyncFree(uv_work_t* req));
