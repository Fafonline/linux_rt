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
        printf("Thread %d Wait for char...\n",id);
        val = getch();

        printf("thread %d get %c\n",id, val);
    }
}

 int main(int arc, char** argv)
 {
    // create the thread objs
    pthread_t thread1, thread2;
    int thr1 = 1;
    int thr2 = 2;
    // start the threads
    pthread_create(&thread1, NULL, *thread, (void *) thr1);
    pthread_create(&thread2, NULL, *thread, (void *) thr2);
    // wait for threads to finish
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);

    return 0;
 }