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
	kprintf("[");
	if (nonempty(q))
	{
		struct qentry *curr = q->head;
		while (curr != NULL)
		{
			kprintf("(pid=%u)", curr->pid);
			curr = curr->next;
		}
	}
	kprintf("]\n");
}

/**
 * Checks whether queue is empty
 * @param q	Pointer to a queue
 * @return 1 if true, 0 otherwise
 */
bool8	isempty(struct queue *q)
{
	return (q->size == 0);
}

/**
 * Checks whether queue is nonempty
 * @param q	Pointer to a queue
 * @return 1 if true, 0 otherwise
 */
bool8	nonempty(struct queue *q)
{
	return (q == NULL || q->size > 0);
}


/**
 * Checks whether queue is full
 * @param q	Pointer to a queue
 * @return 1 if true, 0 otherwise
 */
bool8	isfull(struct queue *q)
{
	//check if there are at least NPROC processes in the queue
	return (q->size >= NPROC);
}


/**
 * Insert a process at the tail of a queue
 * @param pid	ID process to insert
 * @param q	Pointer to the queue to use
 *
 * @return pid on success, SYSERR otherwise
 */
pid32 enqueue(pid32 pid, struct queue *q, int32 key)
{
	if (isfull(q) || isbadpid(pid)) {
		return SYSERR;
	}
	//allocate space on heap for a new qentry
	struct qentry *newEntry = (struct qentry*) malloc(sizeof(struct qentry));

	//initialize the new QEntry
	newEntry->pid = pid;
	newEntry->prev = NULL;
	newEntry->next = NULL;
	newEntry->key = key;

	//if there are no processes on the queue
	if (q->head == NULL){
		q->head = newEntry;
		q->tail = newEntry;
	}
	//if priority for newEntry is greater than the head
	else if (newEntry->key > q->head->key){
		newEntry->next = q->head;
		newEntry->next->prev = newEntry;
		q->head = newEntry;
	}
	else{
		//otherwise, search for the place to put the new entry
		struct qentry *current = q->head;
		if (current->next != NULL){	
			while (current->next->key >= newEntry->key){
				current = current->next;
			}
		}

		
		newEntry->next = current->next;
		if (current->next != NULL){
			newEntry->next->prev = newEntry;
		}

		current->next = newEntry;
		newEntry->prev = current;

		if (newEntry->next == NULL){
			q->tail = newEntry;
		}
		
	}

	q->size++;
	return pid;
}

/**
 * Remove and return the first process on a list
 * @param q	Pointer to the queue to use
 * @return pid of the process removed, or EMPTY if queue is empty
 */
pid32 dequeue(struct queue *q)
{
	pid32 pid;	

	if (isempty(q)) {
		return EMPTY;
	}

	//save pointer to head entry
	struct qentry *head = q->head;
	struct qentry *newHead = head->next;

	//save pid of head entry
	pid = head->pid;

	//unlink head from queue
	if (newHead != NULL)
		newHead->prev = NULL;
	else
		q->tail = NULL;

	//update queue to point head pointer at newhead
	q->head = newHead;

	//decrement size of queue
	q->size--;

	//deallocate head entry
	free(head, sizeof(struct qentry));

	return pid;
}


/**
 * Finds a queue entry given pid
 * @param pid	a process ID
 * @param q	a pointer to a queue
 * @return pointer to the entry if found, NULL otherwise
 */
struct qentry *getbypid(pid32 pid, struct queue *q)
{
	if (isempty(q))
		return NULL;

	//find the qentry with the given pid
	struct qentry *currEntry = q->head;
	while(currEntry != NULL && currEntry->pid != pid)
		currEntry = currEntry->next;

	//return a pointer to it
	return currEntry;
}

/**
 * Remove a process from the front of a queue (pid assumed valid with no check)
 * @param q	pointer to a queue
 * @return pid on success, EMPTY if queue is empty
 */
pid32	getfirst(struct queue *q)
{
	if (isempty(q)) {
		return EMPTY;
	}

	return dequeue(q);
}

/**
 * Remove a process from the end of a queue (pid assumed valid with no check)
 * @param q	Pointer to the queue
 * @return pid on success, EMPTY otherwise
 */
pid32	getlast(struct queue *q)
{
	if (isempty(q)) {
		return EMPTY;
	}

	return remove(q->tail->pid, q);
}



/**
 * Remove a process from an arbitrary point in a queue
 * @param pid	ID of process to remove
 * @param q	Pointer to the queue
 * @return pid on success, SYSERR if pid is not found
 */
pid32	remove(pid32 pid, struct queue *q)
{
	if (isempty(q))
	{
		return EMPTY;
	}

	//find the entry with pid
	struct qentry *currEntry = q->head;
	while (currEntry != NULL)
	{
		//found it!
		if (currEntry->pid == pid)
		{
			//unlink: find next and prev entries
			struct qentry *next = currEntry->next;
			struct qentry *prev = currEntry->prev;
			if (next != NULL)
				next->prev = prev;
			if (prev != NULL)
				prev->next = next;

			//update queue structure
			if (currEntry == q->head)
				q->head = prev;
			if (currEntry == q->tail)
				q->tail = next;
			q->size--;

			//deallocate current entry
			free(currEntry, sizeof(struct qentry));
			return pid;
		}
		currEntry = currEntry->next;
	}

	//if pid does not exist in the queue, return SYSERR
	return SYSERR;
}
