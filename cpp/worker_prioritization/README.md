We have some worker, which can handle some task in 20-50ms

We must provide a wrapper over a pool of worker which will always choose the worker with the least number of active tasks
