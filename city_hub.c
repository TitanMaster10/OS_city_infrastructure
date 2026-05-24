#include "city_manager.h"

#define MAX_CMD 512
#define MAX_DISTRICTS 16
#define BUF_SIZE 4096

static pid_t hub_mon_pid = -1;

void cmd_calculate_scores(char *args){
    char *districts[MAX_DISTRICTS];
    int count = 0;

    char *tok = strtok(args, " ");
    while (tok != NULL && count < MAX_DISTRICTS){
        districts[count++] = tok;
        tok = strtok(NULL, " ");
    }

    if (count == 0){
        fprintf(stderr, "no districts specified\n");
        return;
    }

    for (int i = 0; i < count; i++){
        struct stat st;
        if (stat(districts[i], &st) == -1 || !S_ISDIR(st.st_mode)){
            fprintf(stderr, "district '%s' does not exist, skipping\n", districts[i]);
            continue;
        }

        int pipe_fd[2];
        if (pipe(pipe_fd) == -1){
            perror("pipe");
            continue;
        }

        pid_t pid = fork();
        if (pid < 0){
            perror("fork");
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            continue;
        }

        if (pid == 0){
            close(pipe_fd[0]);
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[1]);

            execl("./scorer", "scorer", districts[i], NULL);
            perror("execl scorer");
            _exit(1);
        }

        close(pipe_fd[1]);

        char buf[BUF_SIZE];
        int total_read = 0;
        int n;
        while ((n = read(pipe_fd[0], buf + total_read, BUF_SIZE - total_read - 1)) > 0){
            total_read += n;
        }
        buf[total_read] = '\0';
        close(pipe_fd[0]);

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0){
            printf("%s", buf);
        } else {
            fprintf(stderr, "scorer failed for district '%s'\n", districts[i]);
        }
    }
}

void hub_mon_process(int read_fd){
    char buf[512];
    int pos = 0;
    int n;

    while ((n = read(read_fd, buf + pos, 1)) > 0){
        if (buf[pos] == '\n'){
            buf[pos + 1] = '\0';
            if (strncmp(buf, "ERROR:", 6) == 0){
                printf("[monitor] %s", buf);
            } else if (strncmp(buf, "SIGUSR1:", 8) == 0){
                printf("[monitor] %s", buf);
            } else if (strncmp(buf, "SIGINT:", 7) == 0){
                printf("[monitor] %s", buf);
            } else if (strncmp(buf, "INFO:", 5) == 0){
                printf("[monitor] %s", buf);
            } else {
                printf("[monitor] %s", buf);
            }
            fflush(stdout);
            pos = 0;
        } else {
            pos++;
            if (pos >= (int)sizeof(buf) - 2) pos = 0; // overflow protection
        }
    }
    printf("[monitor] monitorul s-a oprit.\n");
    fflush(stdout);
    close(read_fd);
}

void cmd_start_monitor(){
    // verifica daca hub_mon deja ruleaza
    if(hub_mon_pid > 0 && kill(hub_mon_pid, 0) == 0){
        printf("monitorul deja ruleaza.\n");
        return;
    }

    pid_t hub_pid = fork();
    if (hub_pid < 0){
        perror("fork hub_mon");
        return;
    }
    if (hub_pid > 0){
        hub_mon_pid = hub_pid;
        printf("a inceput hub_mon (pid %d)\n", hub_pid);
        usleep(300000); // timp pt monitor sa porneasca
        return;
    }
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1){
        perror("pipe");
        _exit(1);
    }
    pid_t mon_pid = fork();
    if (mon_pid < 0){
        perror("fork monitor");
        _exit(1);
    }
    if (mon_pid == 0){
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        execl("./monitor_reports", "monitor_reports", NULL);
        perror("execl monitor_reports");
        _exit(1);
    }
    close(pipe_fd[1]);
    hub_mon_process(pipe_fd[0]);
    int status;
    waitpid(mon_pid, &status, 0);
    _exit(0);
}

void cleanup_monitor(){
    if(hub_mon_pid <= 0) return;
    if(kill(hub_mon_pid, 0) != 0) return;

    // trimite SIGINT la monitor prin .monitor_pid
    int fd = open(MONITOR, O_RDONLY);
    if(fd != -1){
        char pidbuf[32];
        memset(pidbuf, 0, sizeof(pidbuf));
        int nr = read(fd, pidbuf, sizeof(pidbuf) - 1);
        close(fd);
        if(nr > 0){
            pidbuf[strcspn(pidbuf, "\n")] = '\0';
            pid_t mon = (pid_t)atoi(pidbuf);
            if(mon > 0) kill(mon, SIGINT);
        }
    }
    waitpid(hub_mon_pid, NULL, 0);
    hub_mon_pid = -1;
}

int main(){
    char line[MAX_CMD];

    printf("city_hub> ");
    fflush(stdout);

    while (fgets(line, sizeof(line), stdin) != NULL){
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0){
            printf("city_hub> ");
            fflush(stdout);
            continue;
        }

        if (strcmp(line, "exit") == 0){
            cleanup_monitor();
            break;
        }

        if (strncmp(line, "calculate_scores", 16) == 0){
            char *args = line + 16;
            while (*args == ' ') args++;
            if (strlen(args) == 0){
                fprintf(stderr, "usage: calculate_scores <district1> [district2] ...\n");
            } else {
                cmd_calculate_scores(args);
            }
        }
        else if (strcmp(line, "start_monitor") == 0){
            cmd_start_monitor();
        }
        else {
            fprintf(stderr, "unknown command: %s\n", line);
        }

        printf("city_hub> ");
        fflush(stdout);
    }

    return 0;
}