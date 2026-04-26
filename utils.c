#include "city_manager.h"

void build_path(const char *district, const char *filename,
                char *out, size_t out_size)
{
    snprintf(out, out_size, "%s/%s", district, filename);
}

int get_next_id(const char *district)
{
    char path[256];
    int max_id = 0;
    Report r;

    build_path(district, FILE_REPORTS, path, sizeof(path));
    int index;
    index = open(path, O_RDONLY);
    if (index == -1)
        return 1;

    while (read(index, &r, sizeof(Report)) == sizeof(Report)) {
        if (r.id > max_id)
            max_id = r.id;
    }

    close(index);
    return max_id + 1;
}
void print_report(const Report *r)
{
    char tbuf[64];
    struct tm *tinfo = localtime(&r->timestamp);
    strftime(tbuf, sizeof(tbuf), "%Y-%m-%d %H:%M:%S", tinfo);

    printf("  Report ID  : %d\n", r->id);
    printf("  Inspector  : %s\n", r->inspector);
    printf("  GPS        : (%.6f, %.6f)\n", r->latitude, r->longitude);
    printf("  Category   : %s\n", r->category);
    printf("  Severity   : %d\n", r->severity);
    printf("  Timestamp  : %s\n", tbuf);
    printf("  Description: %s\n", r->description);
    printf("\n");
}

void ensure_district(const char *district)
{
    struct stat st;
    char path[256];

    if (stat(district, &st) == -1) {
        if (mkdir(district, FULL_ACCESS) == -1) {
            perror("mkdir");
            exit(1);
        }
        chmod(district, FULL_ACCESS);
    }

    
    build_path(district, FILE_CONFIGURATION, path, sizeof(path));
    if (stat(path, &st) == -1) {
        int index = open(path, O_WRONLY | O_CREAT | O_TRUNC, READ_WRITE_READ);
        if (index == -1) {
            perror("opening district.cfg error.");
            exit(1);
        }
        write(index, DEFAULT_SEVERITY, strlen(DEFAULT_SEVERITY));
        close(index);
        chmod(path, READ_WRITE_READ);
    }

    // logged_district (empty initially)
    build_path(district, LOGS, path, sizeof(path));
    if (stat(path, &st) == -1) {
        int index = open(path, O_WRONLY | O_CREAT | O_TRUNC, READ_WRITE_READ);
        if (index == -1) {
            perror("opening logged_district error.");
            exit(1);
        }
        close(index);
        chmod(path, READ_WRITE_READ);
    }
}

void log_action(const char *district, const char *role,
                const char *user, const char *action)
{
    char path[256];

    build_path(district, LOGS, path, sizeof(path));
    
    int index;
    index = open(path, O_WRONLY | O_APPEND | O_CREAT, READ_WRITE_READ);
    if (index == -1) {
        perror("open logged_district");
        return;
    }

    char line[512];
    snprintf(line, sizeof(line), "%ld\t%s\t%s\t%s\n",
             (long)time(NULL), user, role, action);
    write(index, line, strlen(line));
    close(index);
}

void update_symlink(const char *district)
{
    char link_name[256];
    char target[256];
    struct stat lst;

    snprintf(link_name, sizeof(link_name), "%s%s", SYMLINK_PREFIX, district);
    build_path(district, FILE_REPORTS, target, sizeof(target));

    // remove old one if exists
    if (lstat(link_name, &lst) == 0) {
        if (S_ISLNK(lst.st_mode))
            unlink(link_name);
    }

    // make sure target exists before linking
    if (stat(target, &lst) == -1) {
        fprintf(stderr, "Warning: '%s' doesn't exist, skipping symlink.\n", target);
        return;
    }

    if (symlink(target, link_name) == -1)
        perror("symlink");
}

void remove_symlink(const char *district)
{
    char link_name[256];
    struct stat lst;

    snprintf(link_name, sizeof(link_name), "%s%s", SYMLINK_PREFIX, district);

    if (lstat(link_name, &lst) == 0 && S_ISLNK(lst.st_mode))
        unlink(link_name);
}