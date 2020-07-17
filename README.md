Iteration 4:
Implemented deadlock detection and recovery using DFS on a resource allocation graph.

Iteration 3:
Extended Xinu to support a time-sharing environment and provide the OS with an atomic locking mechanism

Iteration 2: 
Turned the ready queue into a priority queue, with the head of the queue being the highest priority element. Also implemented a process aging policy so there is not starvation among processes. Files modified are ready.c, queue.c, resched.c, newqueue.c, main.c, and xinu.h.

Iteration 1:
Implemented a ready queue for processes in Xinu. Files touched are queue.c, newqueue.c, ready.c, resched.c, and xinu.h. The implementation is backed by a linked list.