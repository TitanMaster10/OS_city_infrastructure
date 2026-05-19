#include "city_manager.h"

volatile sig_atomic_t running = 1;

void signal_handler(int sig){
    if (sig ==SIGINT){
        char msg[] = "SIGINT primit, inchiderea\n";
        write(STDOUT_FILENO,msg,sizeof(msg) -1);
        running = 0;
    }
    else if(sig == SIGUSR1){
        char msg[]=" nou raport a fost adaugat\n";
        write(STDOUT_FILENO,msg,sizeof(msg) -1);

    }

}

void setup_signals(){
    struct sigaction sa;
    memset(&sa,0,sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    if(sigaction( SIGINT, &sa, NULL) == -1){
       perror ("sigaction SIGINT"); 
        exit(1);
    }
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGUSR1, &sa, NULL) == -1){
        perror("sigaction SIGURSR1");
        exit(1);
    }

}

int main(){
    int fd= open(MONITOR, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1){
        perror("nu sa putut deschide .fisier");
        exit(1);
    }
    char buf[32];
    int len =snprintf(buf,sizeof(buf),"%d\n",getpid());
    write(fd,buf,len);
    close(fd);

    printf("sa inceput monitorizarea pid %d\n", getpid());

    setup_signals();
    while(running) pause();
    unlink(MONITOR);
    return 0;
}