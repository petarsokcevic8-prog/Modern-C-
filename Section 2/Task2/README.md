# ThreadPool Module

## 1. Architectural Overview

This module implements a fixed-size thread pool for concurrent task execution using C++ threading primitives.

The system consists of:
- A shared task queue storing callable tasks (std::function<void()>)
- A fixed number of worker threads
- A mutex to protect shared queue access
- A condition variable to avoid busy-waiting

### Execution Flow:
1. Main thread submits tasks via an enqueue method
2. Tasks are stored in a shared queue
3. Worker threads sleep when no tasks are available
4. When a task is added, one worker thread is notified
5. The worker thread executes the task and returns to waiting state

This ensures efficient CPU usage by avoiding spinning and unnecessary resource consumption.

---

## 2. Assumptions and Trade-offs

- Fixed number of worker threads (no dynamic scaling)
- FIFO task execution (no prioritization mechanism)
- Tasks are assumed to be independent
- Uses blocking synchronization (mutex + condition_variable)

Trade-off:
Simplicity or correctness and safety were prioritized over advanced scheduling or performance optimizations.

---

## 3. Verification Steps

1. Build the project:
make
2. Run the program:
./ThreadPool.exe
3. Expected behavior:
- Multiple tasks are executed in parallel by different threads
- Output order may vary due to concurrency (this is expected)
- Threads remain idle when no tasks are available (no CPU spinning)
- Program terminates cleanly after all tasks are completed

---

## 4. Build Instructions

Build:
make

Clean:
make clean

Compiler flags used:
-std=c++20 -Wall -Wextra -Werror -O2