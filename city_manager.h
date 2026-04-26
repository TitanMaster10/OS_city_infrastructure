#ifndef CITY_MANAGER_H 
#define CITY_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

#define INSPECTOR_SIZE   32
#define CATEGORY_SIZE    32
#define DESCRIPTION_SIZE 112

#define MANAGER_ROLE   "manager"
#define INSPECTOR_ROLE "inspector"
#define FILE_REPORTS   "reports.dat"
#define FILE_CONFIGURATION "district.cfg"
#define FULL_ACCESS     0750    // Managers have full access; inspectors read and execute only
#define READ_APPEND     0664   // Both roles may read; both may write (append)
#define READ_WRITE_READ 0640  // Managers may read and write; inspectors may read
#define RWRR            0644 // Anyone may read; only the manager role may write



typedef struct {
    int id;
    char inspector[INSPECTOR_SIZE];
    double latitude;
    double longitude;
    char category[CATEGORY_SIZE];
    int severity;
    time_t timestamp;
    char description[DESCRIPTION_SIZE];
}Report;

void add             ( const char* district, const char* role, const char* user);
void list            ( const char* district, const char* role);
void view            ( const char* district, const char* role,int report_id);
void remove_report   ( const char* district, const char* role, const char* user, int report_id);
void update_threshold ( const char* district, const char* role, const char*user, int value);
void print_report(const Report *r);
void build_path      ( const char* district, const char * filename, char*out, size_t out_size  );
int  check_permission( const char* path, const char* role, char action);
int  get_next_id     ( const char* district);
void mode_to_string(mode_t mode, char *out);
void set_permissions(const char *path, mode_t mode);


void filter( const char* district, const char* role, int argc, char* argv[]  );
int parse_condition(const char *input, char *field, char *op, char *value);
int match_condition(Report *r, const char *field, const char *op, const char *value);





#endif