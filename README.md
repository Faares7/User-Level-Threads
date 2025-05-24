# 🧵 xv6 User-Level Threads
This project is an implementation of lightweight user-level threads in the xv6 operating system. The goal was to design a thread library that runs entirely in user space, allowing multiple threads of execution within a single xv6 user process.

## ✅ Features
1. User-level threading without kernel support

2. Thread creation with thread_create

3. Cooperative thread switching via thread_yield

4. Thread synchronization with thread_join

5. Round-robin thread scheduling

6. Each thread has its own independent stack

7. euses xv6's low-level context-switching via swtch.S

## 📁 File Overview
user/uthread.h — Thread library header (API)

user/uthread.c — Thread library implementation

user/thread_switch.S — Assembly code for context switching

user/uthread_test.c — Sample test program using the thread API

## 🛠️ How to Run
### Clone the repository and enter the project directory:
git clone https://github.com/yourusername/xv6-uthreads.git
cd xv6-uthreads

### Build and run xv6 with QEMU:
make qemu

### At the xv6 shell, run the test program:
$ uthread_test

### The output should be something like this:
Starting user-level thread test

<pre> ### 🖨️ Sample Output ``` Starting user-level thread test Created threads: 0, 1, 2 Main thread: 0 Thread A: 0 Main thread: 1 Thread B: 0 Main thread: 2 Thread C: 0 (count=100) Main thread: 3 Thread A: 1 Main thread: 4 Thread B: 1 Main thread: 5 Thread C: 1 (count=101) Main thread: 6 Thread A: 2 Main thread: 7 Thread B: 2 Main thread: 8 Thread C: 2 (count=102) Main thread: 9 Thread A: 3 Thread B: 3 Thread A: 4 Thread A finished Thread B: 4 Thread B finished Thread C finished All threads finished. Final count: 103 Thread test completed ``` </pre>

## 📚 Project Objective
This project was part of an Operating Systems course assignment to strengthen understanding of thread abstractions, context switching, and user-space scheduling.

