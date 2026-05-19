#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define INSPECTOR_SIZE   32
#define CATEGORY_SIZE    32
#define DESCRIPTION_SIZE 112

typedef struct {
    int id;
    char inspector[INSPECTOR_SIZE];
    double latitude;
    double longitude;
    char category[CATEGORY_SIZE];
    int severity;
    long timestamp;
    char description[DESCRIPTION_SIZE];
}Report;

#define MAX_INSPECTORS 64

typedef struct{
    char name[INSPECTOR_SIZE];
    int total_severity;
} InspectorScore;

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr,"usage: scorer <district>\n");
        exit(1);
    }

    char *district = argv[1];
    char path[256];
    snprintf(path,sizeof(path),"%s/reports.dat",district);

    int fd = open(path, O_RDONLY);
    if(fd == -1){
        fprintf(stderr,"nu se poate deschide %s\n",path);
        exit(1);
    }

    InspectorScore scores[MAX_INSPECTORS];
    int nr_inspectors = 0;
    Report r;

    while(read(fd, &r, sizeof(Report)) == sizeof(Report)){
        // cauta inspectorul in lista
        int found = -1;
        for(int i=0; i < nr_inspectors; i++){
            if(strcmp(scores[i].name, r.inspector) == 0){
                found = i;
                break;
            }
        }
        if(found == -1){
            if(nr_inspectors >= MAX_INSPECTORS){
                fprintf(stderr,"prea multi inspectori in %s\n",district);
                break;
            }
            found = nr_inspectors;
            memset(&scores[found], 0, sizeof(InspectorScore));
            strncpy(scores[found].name, r.inspector, INSPECTOR_SIZE -1);
            nr_inspectors++;
        }
        scores[found].total_severity += r.severity;
    }
    close(fd);

    printf("District: %s\n",district);
    if(nr_inspectors == 0){
        printf("  (niciun raport)\n");
    }else{
        for(int i=0;i<nr_inspectors;i++){
            printf("  %s : %d\n",scores[i].name, scores[i].total_severity);
        }
    }

    return 0;
}
