struct t_thread
{
    pthread_t thread;
    int id;
    int sched_policy;
    int shced_scope;
    float priority;
    void (*cb)(int);
};