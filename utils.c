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
