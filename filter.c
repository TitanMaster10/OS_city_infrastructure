#include "city_manager.h"

/*
 * AI-generated function (parse_condition)
 * See ai_usage.md for details.
 */
int parse_condition(const char *input, char *field, char *op, char *value)
{
    const char *p = input;
    int i;

    // get field (up to first ':')
    i = 0;
    while (*p && *p != ':' && i < 31) {
        field[i++] = *p++;
    }
    field[i] = '\0';

    if (*p != ':') return 0;
    p++;

    // get operator
    i = 0;
    while (*p && *p != ':' && i < 7) {
        op[i++] = *p++;
    }
    op[i] = '\0';

    if (*p != ':') return 0;
    p++;

    // get value (rest of string)
    i = 0;
    while (*p && i < 63) {
        value[i++] = *p++;
    }
    value[i] = '\0';

    // validate operator
    if (strcmp(op, "==") != 0 && strcmp(op, "!=") != 0 &&
        strcmp(op, "<")  != 0 && strcmp(op, "<=") != 0 &&
        strcmp(op, ">")  != 0 && strcmp(op, ">=") != 0) {
        return 0;
    }

    if (strlen(field) == 0 || strlen(op) == 0 || strlen(value) == 0)
        return 0;

    return 1;
}

int match_condition(Report *r, const char *field, const char *op,
                    const char *value)
{
    int cmp = 0;

    if (strcmp(field, "severity") == 0) {
        int val = atoi(value);
        int diff = r->severity - val;
        if (diff < 0) cmp = -1;
        else if (diff > 0) cmp = 1;
        else cmp = 0;

    } else if (strcmp(field, "category") == 0) {
        cmp = strcmp(r->category, value);

    } else if (strcmp(field, "inspector") == 0) {
        cmp = strcmp(r->inspector, value);

    } else if (strcmp(field, "timestamp") == 0) {
        long val = atol(value);
        long diff = (long)r->timestamp - val;
        if (diff < 0) cmp = -1;
        else if (diff > 0) cmp = 1;
        else cmp = 0;

    } else {
        fprintf(stderr, "Warning: unknown field '%s'\n", field);
        return 0;
    }

    if (strcmp(op, "==") == 0) return cmp == 0;
    if (strcmp(op, "!=") == 0) return cmp != 0;
    if (strcmp(op, "<")  == 0) return cmp <  0;
    if (strcmp(op, "<=") == 0) return cmp <= 0;
    if (strcmp(op, ">")  == 0) return cmp >  0;
    if (strcmp(op, ">=") == 0) return cmp >= 0;

    return 0;
}

// my code - reads reports one by one and checks all conditions (AND logic)
void filter(const char *district, const char *role,
                int cond_count, char **cond_args)
{
    char path[256];
    Report r;
    int fd;

    if (cond_count < 1) {
        fprintf(stderr, "Error: need at least one condition (field:op:value)\n");
        exit(1);
    }

    build_path(district, FILE_REPORTS, path, sizeof(path));

    if (!check_permission(path, role, 'r'))
        exit(1);

    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open reports.dat");
        exit(1);
    }

    // parse all conditions before the loop so we don't re-parse every iteration
    char fields[16][32];
    char ops[16][8];
    char values[16][64];

    if (cond_count > 16) {
        fprintf(stderr, "Error: max 16 conditions.\n");
        close(fd);
        exit(1);
    }

    for (int i = 0; i < cond_count; i++) {
        if (!parse_condition(cond_args[i], fields[i], ops[i], values[i])) {
            fprintf(stderr, "Error: bad condition '%s' (expected field:op:value)\n",
                    cond_args[i]);
            close(fd);
            exit(1);
        }
    }

    int matches = 0;
    while (read(fd, &r, sizeof(Report)) == sizeof(Report)) {
        int ok = 1;
        for (int i = 0; i < cond_count; i++) {
            if (!match_condition(&r, fields[i], ops[i], values[i])) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            printf("--- Report #%d ---\n", r.id);
            print_report(&r);
            matches++;
        }
    }

    close(fd);
    printf("Found %d matching report(s).\n", matches);
}
