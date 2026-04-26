#include "city_manager.h"

void add(const char* district, const char* role, const char* user){

    Report r;
    char path[256];
    struct stat st;

    build_path(district, FILE_REPORTS, path, sizeof(path));

    if (stat(path,&st) ==0){
        if (!check_permission(path, role, 'w'))exit(1);    
    }
    memset(&r, 0, sizeof(Report));
    r.id = get_next_id(district);
    strncpy(r.inspector, user, INSPECTOR_SIZE -1);
    r.timestamp = time(NULL);

    double latitude,longitude;
    printf("X: "); scanf("%lf",&latitude);
    printf("Y: "); scanf("%lf",&longitude);
    getchar();
    char category[CATEGORY_SIZE];
    printf("Category: road/lighting/flooding/other: ");
    fgets(category, CATEGORY_SIZE, stdin);
    category[strcspn(category, "\n")] = '\0';

    int severity;
    printf("Severity level: 1/2/3/4/: ");
    scanf("%d",&severity);
    getchar();

    char description[DESCRIPTION_SIZE];
    printf("Description: ");
    fgets(description,DESCRIPTION_SIZE,stdin);
    description[strcspn(description, "\n")] = '\0';

    r.latitude=latitude;
    r.longitude=longitude;
    r.severity=severity;
    strncpy(r.category,category,CATEGORY_SIZE -1);
    strncpy(r.description,description,DESCRIPTION_SIZE -1);

    int index;
    index = open(path, O_WRONLY | O_CREAT | O_APPEND, READ_APPEND );
    if (index == -1){
        perror("opening reports.dat error");
        exit(1);
    }
    if (write(index,&r, sizeof(Report))!= sizeof(Report)){
        perror("write error");
        close(index);
        exit(1);
    }
    close(index);
    set_permissions(path, READ_APPEND);

    printf("Report %d added to %s. \n",r.id,district);

}
void list(const char* district, const char* role){
    struct stat st;
    char path[256];
    Report r;
    
    build_path(district, FILE_REPORTS, path, sizeof(path));
    if (stat(path, &st) == -1){
        fprintf(stderr, "No reports file in %s, error.\n",district);
        exit(1);
    }
    if(!check_permission(path, role, 'r')) exit(1);
    char perm_str[10];

    mode_to_string(st.st_mode, perm_str);
    char time[64];
    struct tm *tinfo=localtime(&st.st_mtime);
    strftime(time,sizeof(time), "%Y-%m-%d %H:%M:%S",tinfo);

    printf("File: %s\n",path);
    printf("Permissions: %s Size: %ld, bytes Modified :%s\n", perm_str,st.st_size,time);

    int index = open(path, O_RDONLY);
    if (index == -1) {
        perror("opening file error");
        exit(1);
    }
    int count = 0;
    while (read(index, &r, sizeof(Report)) == sizeof(Report)) {
        printf("Report:%d\n", r.id);
        print_report(&r);
        count++;
    }
    close(index);

    if (count == 0)
        printf("No reports in district: %s.\n", district);
    else
        printf("In total: %d reports\n", count);

}

void view(const char *district, const char *role, int report_id)
{
    char path[256];
    Report r;

    build_path(district, FILE_REPORTS, path, sizeof(path));
    if (!check_permission(path, role, 'r'))
        exit(1);
   
    int index;
    index = open(path, O_RDONLY);
    if (index == -1) {
        perror("opening reports.dat error");
        exit(1);
    }

    while (read(index, &r, sizeof(Report)) == sizeof(Report)) {
        if (r.id == report_id) {
            printf("Report:%d\n", r.id);
            print_report(&r);
            close(index);
            return;
        }
    }

    close(index);
    fprintf(stderr, "Error: report #%d not found in '%s'.\n",
            report_id, district);
    exit(1);
}

void remove_report(const char *district, const char *role, const char *user, int report_id){
    char path[256];
    struct stat st;
    Report r;

    if (strcmp(role, MANAGER_ROLE) != 0) {
        fprintf(stderr, "only managers can remove reports.\n");
        exit(1);
    }

    build_path(district, FILE_REPORTS, path, sizeof(path));

    if (!check_permission(path, role, 'w')) exit(1);

    int index;
    index = open(path, O_RDWR);
    if (index == -1) {
        perror("opening reports.dat error.");
        exit(1);
    }

    if (fstat(index, &st) == -1) {
        perror("fstat");
        close(index);
        exit(1);
    }

    off_t file_size = st.st_size;
    int num_records = file_size / sizeof(Report);
    off_t found_pos = -1;

    // seeking for the record

    for (int i = 0; i < num_records; i++) {
        lseek(index, i * sizeof(Report), SEEK_SET);
        if (read(index, &r, sizeof(Report)) != sizeof(Report)) {
            perror("read");
            close(index);
            exit(1);
        }
        if (r.id == report_id) {
            found_pos = i * sizeof(Report);
            break;
        }
    }
    // if not found 
    if (found_pos == -1) {
        fprintf(stderr, "Report: %d not found.\n", report_id);
        close(index);
        exit(1);
    }

    // shifting back the records

    off_t rpos = found_pos + sizeof(Report);
    off_t wpos = found_pos;

    while (rpos < file_size) {
        lseek(index, rpos, SEEK_SET);
        if (read(index, &r, sizeof(Report)) != sizeof(Report))
            break;

        lseek(index, wpos, SEEK_SET);
        write(index, &r, sizeof(Report));

        rpos += sizeof(Report);
        wpos += sizeof(Report);
    }

    ftruncate(index, file_size - sizeof(Report));
    close(index);


    printf("Report #%d removed from '%s'.\n", report_id, district);
}


void update_threshold(const char *district, const char *role,
                          const char *user, int value)
{
    char path[256];
    struct stat st;

    if (strcmp(role, MANAGER_ROLE) != 0) {
        fprintf(stderr, "Error: only managers can update threshold.\n");
        exit(1);
    }

    build_path(district, FILE_CONFIGURATION, path, sizeof(path));

    if (stat(path, &st) == -1) {
        fprintf(stderr, "Error: cannot stat '%s': %s\n", path, strerror(errno));
        exit(1);
    }

    // spec says: before writing, verify permissions are 640
    mode_t actual = st.st_mode & 0777;
    if (actual != READ_WRITE_READ) {
        char pstr[10];
        mode_to_string(st.st_mode, pstr);
        fprintf(stderr, "%s has permissions %s, expected rw-r-----. \n", path, pstr);
        exit(1);
    }

    if (!check_permission(path, role, 'w'))exit(1);
    int index;
    index = open(path, O_WRONLY | O_TRUNC);
    if (index == -1) {
        perror("opening district.cfg error.");
        exit(1);
    }

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d\n", value);
    write(index, buffer, strlen(buffer));
    close(index);

    printf("updated threshold for %s to %d.\n", district, value);
}
