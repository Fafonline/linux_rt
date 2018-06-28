#include <stdio.h>
#include <termios.h>  //_getch*/
#include <unistd.h>   //_getch*/

#include <pthread.h>

char getch(){

    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
 }


void* thread(void* arg)
{
    int id = (int)arg;

    char val = 0;

    while(1)
    {
        #ifdef GETCHAR
        printf("Thread %d Wait for char...\n",id);
        val = getch();

        printf("thread %d get %c\n",id, val);
        #else
        printf("Thread %d Tic...\n",id);
        sleep(1);
        #endif
    }
}

// #define THREAD_01_POLICY SCHED_FIFO
#define THREAD_01_POLICY SCHED_FIFO
#define THREAD_02_POLICY SCHED_RR

 int main(int arc, char** argv)
 {
    // create the thread objs
    pthread_t thread1 = NULL;
    pthread_t thread2 = NULL;

    int fifo_max_prio, fifo_min_prio,fifo_mid_prio;   
    int rr_max_prio, rr_min_prio,rr_mid_prio;   
    
    struct sched_param fifo_param;
    struct sched_param rr_param;

    pthread_attr_t custom_sched_attr_1;
    pthread_attr_t custom_sched_attr_2;
    
    pthread_attr_init(&custom_sched_attr_1);
    pthread_attr_init(&custom_sched_attr_2);

    pthread_attr_setinheritsched(&custom_sched_attr_1, PTHREAD_EXPLICIT_SCHED); 
    //pthread_attr_setinheritsched(&custom_sched_attr_2, PTHREAD_EXPLICIT_SCHED); 
   
   

    //Set fifo priority
    pthread_attr_setschedpolicy(&custom_sched_attr_1, THREAD_01_POLICY);
    fifo_max_prio = sched_get_priority_max(THREAD_01_POLICY);   
    fifo_min_prio = sched_get_priority_min(THREAD_01_POLICY);   
    fifo_mid_prio = (fifo_min_prio + fifo_max_prio)/2;    
    fifo_param.sched_priority = fifo_mid_prio;  
    printf("=== FIFO Sched ===\n");
    printf("[max:%d,mid=%d,min=%d\n",fifo_max_prio,fifo_mid_prio,fifo_min_prio);
    
    //pthread_attr_setschedparam(&custom_sched_attr_1, &fifo_param);
    // start the threads
    int thr1 = 1;
    int err;
    err = pthread_create(&thread1, &custom_sched_attr_1, *thread, (void *) thr1);
    printf("thread=%d, err=%d\n",thread1,err);
    if(thread1 == NULL)
        printf ("Thread 1 is null\n");
#if 0        
   //Set rr priority
    pthread_attr_setschedpolicy(&custom_sched_attr_2, THREAD_02_POLICY);
    rr_max_prio = sched_get_priority_max(THREAD_02_POLICY);   
    rr_min_prio = sched_get_priority_min(THREAD_02_POLICY);   

    rr_mid_prio = (rr_min_prio + rr_max_prio)/2;    
    rr_param.sched_priority = rr_mid_prio;    
    printf("=== RR Sched ===\n");
    printf("[max:%d,mid=%d,min=%d\n",rr_max_prio,rr_mid_prio,rr_min_prio);
    
    //pthread_attr_setschedparam(&custom_sched_attr_2, &rr_param); 
    pthread_create(&thread2, &custom_sched_attr_2, *thread, (void *) thr2);
    int thr2 = 2;
#endif

    // wait for threads to finish
    if(thread1)
        pthread_join(thread1,NULL);
    //pthread_join(thread2,NULL);

    return 0;
 }