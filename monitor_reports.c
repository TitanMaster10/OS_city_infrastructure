#include "city_manager.h"

volatile sig_atomic_t running = 1;

void signal_handler(int sig){
    if (sig ==SIGINT){
        char msg[] = "SIGINT:inchidere monitor\n";
        write(STDOUT_FILENO, msg, sizeof(msg) - 1);
        running = 0;
    }
    else if (sig == SIGUSR1){
        char msg[] = "SIGUSR1:nou raport a fost adaugat\n";
        write(STDOUT_FILENO, msg, sizeof(msg) - 1);
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
        perror("sigaction SIGUSR1");
        exit(1);
    }
}

int check_existing_monitor(){
    int fd= open(MONITOR, O_RDONLY);
    if (fd == -1)
        return 0;

    char buf[32];
    memset(buf, 0, sizeof(buf));
    int n = read(fd, buf, sizeof(buf) - 1);
    close(fd);

    if (n <= 0)
        return 0;

    buf[strcspn(buf, "\n")] = '\0';
    pid_t existing = (pid_t)atoi(buf);
    if (existing <= 0)
        return 0;

    if (kill(existing, 0) == 0){

        char errmsg[128];
        int elen = snprintf(errmsg, sizeof(errmsg), "ERROR:monitorul deja merge pid=%d\n", existing);
        write(STDOUT_FILENO, errmsg, elen);
        return 1;
    }

    return 0;
}

int main(){
    if (check_existing_monitor()){
        exit(1);
    }

    int fd= open(MONITOR, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1){
        perror("nu sa putut deschide .monitor_pid");
        exit(1);
    }
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "%d\n", getpid());
    write(fd, buf, len);
    close(fd);

    char infomsg[128];
    int ilen = snprintf(infomsg, sizeof(infomsg), "INFO:sa inceput monitorizarea pid %d\n", getpid());
    write(STDOUT_FILENO, infomsg, ilen);

    setup_signals();

    while (running){
        pause();
    }

    unlink(MONITOR);
    return 0;
}