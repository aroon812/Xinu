#include <xinu.h>

int visited[NRESOURCES];
int deadlockDetected = 0;
int offendersRecorded = 0;

int offending_lock;
int offending_pid;

void reset()    {
    deadlockDetected = 0;
    offendersRecorded = 0;
}

int isDeadlocked()  {
    return deadlockDetected;
}

void resetVisited(){
    int i;
    for (i = 0; i < NRESOURCES; i++){
        visited[i] = 0;
    }
}

void rag_print(){
    int i;
    int j;
    int length = NLOCK + NPROC;
    for (i = 0; i < length; i++){
        for (j = 0; j < length; j++){
            kprintf("%u ", RAG[i][j]);
        }
        kprintf("\n");
    }
}

void rag_request(int pid, int lockid){
    RAG[NPROC + pid][lockid] = 1;
}

void rag_alloc(int pid, int lockid){
    RAG[lockid][NPROC + pid] = 1;
    RAG[NPROC + pid][lockid] = 0;
}

void rag_dealloc(int pid, int lockid){
    RAG[lockid][NPROC + pid] = 0;
    RAG[NPROC + pid][lockid] = 0;
}

void deadlock_detect(int i){   
    visited[i]=1;
    int pid;
    int lockid;
    
    int j;
    for (j=0;j<NRESOURCES;j++){
        if (RAG[i][j] == 1){
            if (visited[j] == 0){
                deadlock_detect(j);
            }
            else if (visited[j] == 1){
                if (deadlockDetected == 0){
                    kprintf("DEADLOCK ");
                    deadlockDetected = 1;
                }  

                //determine index of pid and lockid
                if(i < NLOCK)	{
                	lockid = i;
                	pid = NLOCK - NPROC - (NPROC / 2) + j;
                	kprintf("pid = %u lockid = %u ", pid, lockid);

                    if(offendersRecorded == 0)  {
                        offending_pid = pid;
                        offending_lock = lockid;
                        offendersRecorded = 1;
                    }
                }
                return;
            }
        }    
    }  
}

void deadlock_recover() {
    kprintf("DEADLOCK RECOVER ");
    kprintf("killing pid %d to release lock %d\n", offending_pid, offending_lock);

    struct lockentry *lptr = &locktab[offending_lock];

    pid32 prid = dequeue(lptr->wait_queue);

    //remove pid from other lock's wait queues
    for(int i = 0; i < NLOCK; i++)  {
        struct lockentry *tmp_lptr = &locktab[i];
        if(getbypid(prid, tmp_lptr->wait_queue) != NULL) {
            remove(prid, tmp_lptr->wait_queue);
        }
    }
    //call mutex_unlock() on lock's mutex member
    mutex_unlock(&lptr->lock);

    //update the RAG and zero out all allocation and requests
    //associated with victim process
    for(int i = 0; i < NLOCK; i++)  {
        rag_dealloc(offending_pid, i);
    }

    kill(prid);
} 





