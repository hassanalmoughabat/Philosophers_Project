Philosophers

"I never thought philosophy would be so deadly"

A multithreading project that simulates the classic Dining Philosophers Problem using threads, mutexes, and processes. This project teaches the fundamentals of concurrent programming and resource management.
📖 Project Overview
The Dining Philosophers Problem is a classic synchronization problem in computer science that illustrates the challenges of avoiding deadlock and resource contention in concurrent systems. In this simulation, philosophers sit around a circular table with forks between them, and they must coordinate to eat without starving.
The Problem

N philosophers sit at a round table
There is one fork between each pair of adjacent philosophers
Philosophers alternate between eating, thinking, and sleeping
To eat, a philosopher needs both forks (left and right)
Philosophers cannot communicate with each other
The simulation ends when a philosopher dies of starvation (or when all have eaten enough times)

🎯 Learning Objectives
This project teaches essential concepts in systems programming:

Threading: Creating and managing POSIX threads
Mutexes: Protecting shared resources from race conditions
Process Synchronization: Avoiding deadlocks and ensuring thread safety
Timing: Precise time management and scheduling
Resource Management: Preventing resource contention and starvation

🚀 Usage
Compilation
bash# Compile mandatory part (threads + mutexes)
make

# Compile bonus part (processes + semaphores)
make bonus
Execution
bash./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
Parameters

number_of_philosophers: Number of philosophers (and forks)
time_to_die: Time in milliseconds before a philosopher dies of starvation
time_to_eat: Time in milliseconds a philosopher takes to eat
time_to_sleep: Time in milliseconds a philosopher spends sleeping
number_of_times_each_philosopher_must_eat (optional): Simulation stops when all philosophers have eaten this many times

Examples
bash# 5 philosophers, die after 800ms, eat for 200ms, sleep for 200ms
./philo 5 800 200 200

# Same as above, but stop after each philosopher eats 3 times
./philo 5 800 200 200 3

# Edge case: 1 philosopher (should die)
./philo 1 800 200 200

# Stress test: Many philosophers
./philo 100 410 200 200
📁 Project Structure
philosophers/
├── philo/                    # Mandatory part (threads + mutexes)
│   ├── src/
│   ├── includes/
│   └── Makefile
├── philo_bonus/             # Bonus part (processes + semaphores)
│   ├── src/
│   ├── includes/
│   └── Makefile
└── README.md
🔧 Implementation Details
Mandatory Part: Threads + Mutexes

Each philosopher is a thread
Each fork is protected by a mutex
Shared data structures are protected against race conditions
Uses pthread_create, pthread_join, pthread_mutex_lock/unlock

Bonus Part: Processes + Semaphores

Each philosopher is a separate process
Semaphores control access to the forks
Uses fork(), sem_open, sem_wait, sem_post
Inter-process communication for coordination

Key Challenges Solved

Deadlock Prevention: Careful ordering of resource acquisition
Race Condition Elimination: Proper mutex/semaphore usage
Precise Timing: Accurate death detection within 10ms
Clean Termination: Proper cleanup of threads/processes and resources

📊 Program Output
The program logs every state change with precise timestamps:
142 1 has taken a fork
142 1 has taken a fork  
142 1 is eating
344 1 is sleeping
544 1 is thinking
644 2 has taken a fork
644 2 has taken a fork
644 2 is eating
Format: timestamp_in_ms philosopher_number action
⚠️ Critical Requirements

No data races - All shared data must be properly protected
No deadlocks - Philosophers must not get stuck waiting indefinitely
Accurate timing - Death detection within 10ms of actual death
No global variables - Proper encapsulation and data passing
Memory management - No memory leaks, proper cleanup

🧪 Testing
Basic Tests
bash# Should not die
./philo 5 800 200 200

# Should die  
./philo 1 800 200 200

# Should stop after eating
./philo 5 800 200 200 7
Edge Cases
bash# Large numbers
./philo 200 410 200 200

# Tight timing
./philo 4 310 200 100

# Single philosopher
./philo 1 400 100 100
Stress Testing

Run with Valgrind to check for memory leaks
Test with ThreadSanitizer to detect race conditions
Verify timing accuracy under system load
Test graceful termination scenarios

🛠️ Technical Skills Demonstrated

POSIX Threading (pthread library)
Mutex Synchronization (avoiding race conditions)
Process Management (fork, wait, signals)
Semaphore Operations (System V IPC)
Precise Timing (gettimeofday, usleep)
Memory Management (malloc/free, avoiding leaks)
Error Handling (robust system call handling)

📚 Resources

Dining Philosophers Problem - Wikipedia
POSIX Threads Programming
Semaphores in Linux

🏆 Project Goals

✅ Understand concurrency and parallelism
✅ Master thread synchronization primitives
✅ Learn to prevent deadlocks and race conditions
✅ Practice precise timing and resource management
✅ Build robust, leak-free multithreaded applications


This project is part of the 42 School curriculum, focusing on systems programming and concurrent execution.
