#pragma once
#include <uv.h>

/// @brief Shutdown all async operations.
void AsyncShutdown();

/// @brief Make an async call.
/// @param data - The data to pass.
/// @param callback - The async callback.
/// @param callbackDone - The callback when the async operation is completed.
void AsyncCall(void *data, uv_work_cb callback, uv_after_work_cb callbackDone);

/// @brief Default after worker callback.
/// @param req - The worker request.
/// @param status - The request status.
void AsyncNull(uv_work_t* req, int status);

/// @brief Free a worker request.
/// @param req - The worker request.
void AsyncFree(uv_work_t* req);
