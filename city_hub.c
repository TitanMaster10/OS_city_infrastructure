#include "city_manager.h"

#define MAX_CMD 512
#define MAX_DISTRICTS 16
#define BUF_SIZE 4096

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

void cmd_start_monitor(){
    // TODO: implement hub_mon with pipe to monitor
    printf("start_monitor: not implemented yet\n");
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
