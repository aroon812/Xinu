#define NRESOURCES NLOCK+NPROC
void resetVisited();
void rag_print();
void rag_request(int pid, int lockid);
void rag_alloc(int pid, int lockid);
void rag_dealloc(int pid, int lockid);
void deadlock_detect(int i);

extern int RAG[NRESOURCES][NRESOURCES];