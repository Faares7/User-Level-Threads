#include "types.h"
#include "stat.h"
#include "user/user.h"
#include "user/uthread.h"

static thread_t threads[MAX_THREADS];
static int current_thread = 0;
static int thread_count = 0;

void thread_init(void) {
    // Initialize main thread
    threads[0].tid = 0;
    threads[0].state = THREAD_RUNNING;
    threads[0].func = 0;
    threads[0].arg = 0;
    current_thread = 0;
    thread_count = 1;
    
    // Initialize other threads as unused
    for(int i = 1; i < MAX_THREADS; i++) {
        threads[i].state = THREAD_UNUSED;
        threads[i].tid = i;
    }
}

int thread_create(void (func)(void), void* arg) {
    // Find unused thread slot
    int tid = -1;
    for(int i = 0; i < MAX_THREADS; i++) {
        if(threads[i].state == THREAD_UNUSED) {
            tid = i;
            break;
        }
    }
    
    if(tid == -1) return -1; // No available threads
    
    // Initialize thread
    threads[tid].state = THREAD_READY;
    threads[tid].func = func;
    threads[tid].arg = arg;
    
    // Set up stack - stack grows downward
    char* stack_top = threads[tid].stack + STACK_SIZE;
    
    // Push function wrapper address onto stack
    stack_top -= sizeof(void*);
    (void)stack_top = (void)thread_wrapper;
    
    // Set stack pointer
    threads[tid].context.sp = (int)stack_top;
    threads[tid].context.bp = (int)stack_top;
    
    thread_count++;
    return tid;
}

void thread_wrapper(void) {
    thread_t* t = &threads[current_thread];
    t->func(t->arg);
    thread_exit();
}

void thread_yield(void) {
    thread_schedule();
}

void thread_exit(void) {
    threads[current_thread].state = THREAD_FINISHED;
    thread_count--;
    thread_schedule();
}

void thread_schedule(void) {
    thread_t* current = &threads[current_thread];
    
    // Mark current thread as ready if it's still running
    if(current->state == THREAD_RUNNING) {
        current->state = THREAD_READY;
    }
    
    // Find next ready thread (round-robin)
    int next = (current_thread + 1) % MAX_THREADS;
    while(next != current_thread) {
        if(threads[next].state == THREAD_READY) {
            break;
        }
        next = (next + 1) % MAX_THREADS;
    }
    
    // If no ready thread found, keep current
    if(threads[next].state != THREAD_READY) {
        current->state = THREAD_RUNNING;
        return;
    }
    
    // Switch to next thread
    threads[next].state = THREAD_RUNNING;
    int old_thread = current_thread;
    current_thread = next;
    
    // Perform context switch
    thread_switch(&threads[old_thread].context, &threads[next].context);
}

int thread_join(int tid) {
    if(tid < 0 || tid >= MAX_THREADS) return -1;
    
    // Wait for thread to finish
    while(threads[tid].state != THREAD_FINISHED && 
          threads[tid].state != THREAD_UNUSED) {
        thread_yield();
    }
    
    // Clean up finished thread
    threads[tid].state = THREAD_UNUSED;
    return 0;
}
