/* queue.c - enqueue, dequeue, isempty, nonempty, et al. */

#include <xinu.h>
#include <stdlib.h>

/**
 * Prints out contents of a queue
 * @param q	pointer to a queue
 */
void	printqueue(struct queue *q)
{
	//print all contents from head to tail
    struct qentry *qcurr = q->head;

    kprintf("[");
    int i;
    for(i = 0; i < q->size; i++){
      if(qcurr->pid != EMPTY){
        kprintf("(pid=%d)", qcurr->pid);
        if(i < q->size-1){
          kprintf(", ");
        }
        qcurr = qcurr->qnext;
      }
    }
    kprintf("]\n");
}

/**
 * Checks whether queue is empty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
 bool8	isempty(struct queue *q)
 {
   if(q != NULL){
     if(q->size == 0){
       return TRUE;
     }
   }
   return FALSE;
 }

 /**
 * Checks whether queue is nonempty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	nonempty(struct queue *q)
{
	//don't just check q's size because q could be NULL
  if(q != NULL){
    if(q->size == 0){
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Checks whether queue is full
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	isfull(struct queue *q)
{
	//check if there are at least NPROC processes in the queue
  if(q != NULL){
    if(q->size >= NPROC){
      return TRUE;
    }
  }
  return FALSE;
}


/**
 * Insert a process at the tail of a queue
 * @param pid	ID process to insert
 * @param q	Pointer to the queue to use
 *
 * @return pid on success, SYSERR otherwise
 */
pid32 enqueue(pid32 pid, struct queue *q)
{
        //check if queue is full and if pid is illegal, and return SYSERR if either is true
        if(isfull(q) || pid < 0 || pid >= NPROC){
          return SYSERR;
        }

        //allocate space on heap for a new QEntry
        struct qentry *newentry = (struct qentry*) malloc(sizeof(struct qentry));

        //initialize the new QEntry
        newentry->pid = pid;
        newentry->qprev = q->tail;
        newentry->qnext = NULL;

        if(q->tail == NULL && q->head == NULL){
          q->head = newentry;
        }
        else{
          q->tail->qnext = newentry;
        }

        //insert into tail of queue
        q->tail = newentry;
        q->size++;
        
        //return the pid on success
        return pid;
}


/**
 * Remove and return the first process on a list
 * @param q	Pointer to the queue to use
 * @return pid of the process removed, or EMPTY if queue is empty
 */
pid32 dequeue(struct queue *q)
{
        //return EMPTY if queue is empty
        if(isempty(q)){
          return EMPTY;
        }
        //get the head entry of the queue
        struct qentry *head = q->head;

        //unlink the head entry from the rest
        q->head = q->head->qnext;
        q->head->qprev = NULL;
        pid32 entrypid = head->pid;

        //free up the space on the heap
        free(head, sizeof(struct qentry));
        q->size--;

        //return the pid on success
        return entrypid;
}


/**
 * Finds a queue entry given pid
 * @param pid	a process ID
 * @param q	a pointer to a queue
 * @return pointer to the entry if found, NULL otherwise
 */
struct qentry *getbypid(pid32 pid, struct queue *q)
{
	//return NULL if queue is empty or if an illegal pid is given
  if(isempty(q) || pid < 0 || pid >= NPROC){
    return NULL;
  }

  struct qentry *entry = q->head;
	//find the qentry with the given pid
  //return a pointer to it
  int i;
  for(i = 0; i < q->size; i++){
    if(entry->pid == pid){
      return entry;
    }
    entry = entry->qnext;
  }

  //pid not found return NULL
  return NULL;
}

/**
 * Remove a process from the front of a queue (pid assumed valid with no check)
 * @param q	pointer to a queue
 * @return pid on success, EMPTY if queue is empty
 */
pid32	getfirst(struct queue *q)
{
	//return EMPTY if queue is empty
  if(q->size == 0){
    return EMPTY;
  }

	//remove process from head of queue and return its pid
  return dequeue(q);
}

/**
 * Remove a process from the end of a queue (pid assumed valid with no check)
 * @param q	Pointer to the queue
 * @return pid on success, EMPTY otherwise
 */
pid32	getlast(struct queue *q)
{
	//return EMPTY if queue is empty
  if(isempty(q)){
    return EMPTY;
  }

	//remove process from tail of queue and return its pid
  struct qentry *last = q->tail;
  q->tail = q->tail->qprev;
  q->tail->qnext = NULL;
  pid32 pid = last->pid;
  free(last, sizeof(struct qentry));
  q->size--;
  return pid;
}

/**
 * Remove a process from an arbitrary point in a queue
 * @param pid	ID of process to remove
 * @param q	Pointer to the queue
 * @return pid on success, SYSERR if pid is not found
 */
pid32	remove(pid32 pid, struct queue *q)
{
	//return EMPTY if queue is empty
  if(isfull(q)){
    return EMPTY;
  }
	//return SYSERR if pid is illegal
  if(pid < 0 || pid >= NPROC){
    return SYSERR;
  }

	//remove process identified by pid parameter from the queue and return its pid
  struct qentry *entry = q->head;
  //find the qentry with the given pid
  //return a pointer to it
  int i;
  for(i = 0; i < q->size; i++){
    if(entry->pid == pid){
      struct qentry *prev = entry->qprev;
      struct qentry *next = entry->qnext;

      prev->qnext = entry->qnext;
      next->qprev = entry->qprev;

      pid32 entrypid = entry->pid;
      free(entry, sizeof(struct qentry));
      q->size--;
      return entrypid;
    }
    entry = entry->qnext;
  }

  //if pid does not exist in the queue, return SYSERR
  return SYSERR;
}
