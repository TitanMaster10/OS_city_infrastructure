


#include "city_manager.h"

void mode_to_string(mode_t mode, char *out)
{
    out[0] = (mode & S_IRUSR) ? 'r' : '-';
    out[1] = (mode & S_IWUSR) ? 'w' : '-';
    out[2] = (mode & S_IXUSR) ? 'x' : '-';
    out[3] = (mode & S_IRGRP) ? 'r' : '-';
    out[4] = (mode & S_IWGRP) ? 'w' : '-';
    out[5] = (mode & S_IXGRP) ? 'x' : '-';
    out[6] = (mode & S_IROTH) ? 'r' : '-';
    out[7] = (mode & S_IWOTH) ? 'w' : '-';
    out[8] = (mode & S_IXOTH) ? 'x' : '-';
    out[9] = '\0';
}

int check_permission(const char *path, const char *role, char action)
{
    struct stat st;

    if (stat(path, &st) == -1) {
        fprintf(stderr, "Error: cannot stat '%s': %s\n", path, strerror(errno));
        return 0;
    }

    mode_t bits = st.st_mode;
    int allowed = 0;

    if (strcmp(role, MANAGER_ROLE) == 0) {
        // manager = owner
        if (action == 'r') allowed = (bits & S_IRUSR) != 0;
        else if (action == 'w') allowed = (bits & S_IWUSR) != 0;
        else if (action == 'x') allowed = (bits & S_IXUSR) != 0;
    } else {
        // inspector = group
        if (action == 'r') allowed = (bits & S_IRGRP) != 0;
        else if (action == 'w') allowed = (bits & S_IWGRP) != 0;
        else if (action == 'x') allowed = (bits & S_IXGRP) != 0;
    }

    if (!allowed) {
        char ps[10];
        mode_to_string(bits, ps);
        fprintf(stderr, "Error: role '%s' doesn't have '%c' access on '%s' (%s)\n",
                role, action, path, ps);
    }

    return allowed;
}

void set_permissions(const char *path, mode_t mode)
{
    if (chmod(path, mode) == -1)
        perror("chmod");
}
