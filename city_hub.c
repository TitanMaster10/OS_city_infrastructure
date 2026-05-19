#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define MONITOR ".monitor_pid"
#define MAX_CMD 512

void start_monitor(){
    printf("start_monitor\n");
}

void calculate_scores(char *args){
    printf("calculate_scores pentru: %s\n", args);
}

int main(){
    char line[MAX_CMD];

    printf(" City Hub \n");
    printf("Comenzi: start_monitor, calculate_scores district1 district2 ..., exit\n");

    while(1){
        printf("hub> ");
        fflush(stdout);

        if(fgets(line,sizeof(line),stdin) == NULL) break;
        line[strcspn(line,"\n")] = '\0';

        if(strlen(line)==0)continue;

        if(strcmp(line,"exit") == 0){
            printf("iesire din hub.\n");
            break;
        }

        if(strcmp(line,"start_monitor") == 0){
            start_monitor();
        }
        else if(strncmp(line,"calculate_scores ",17) == 0){
            char *args = line + 17;
            if(strlen(args) == 0){
                fprintf(stderr,"usage: calculate_scores <district1> <district2> ...\n");
                continue;
            }
            calculate_scores(args);
        }
        else if(strcmp(line,"calculate_scores") == 0){
            fprintf(stderr,"usage: calculate_scores <district1> <district2> ...\n");
        }
        else{
            fprintf(stderr,"comanda necunoscuta: %s\n",line);
        }
    }

    return 0;
}
