# Async

### ``AsyncShutdown``
Shutdown the async handler, this should be called when the server shutdown.

```c
AsyncShutdown();
```
<hr>

### ``AsyncCall``
Make an async call with the provided callbacks and data.

```c
uv_work_t *worker = AsyncCall(data, callback, completionCallback);
```
<hr>

### ``AsyncNull``
Default worker completion callback, this default callback calls AsyncFree.

```c
uv_work_t *worker = AsyncCall(data, callback, &AsyncNull);
```
<hr>

### ``AsyncFree``
Free a worker request.

```c
AsyncFree(worker);
```
<hr>
