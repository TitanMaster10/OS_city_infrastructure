

#include "city_manager.h"



int main (int argc, char* argv[]){

    if (argc <6 ){
        fprintf(stderr,"usage : <project> <--role> <role> <--command> <where> <extras...>");
        exit(1);
    }

    const char *role =NULL;
    const char *user =NULL;

    // role user checker 
    for(int i = 0 ; i < argc; i++ ){
        if (strcmp(argv[i], "--role") == 0 && i+1 <argc){
            role = argv[++i];
        }
        else if (strcmp(argv[i], "--user") == 0 && i+1 <argc){
            user = argv[++i];
            }
    }
        if (!role){
            fprintf(stderr,"--role is required, error.\n");
            exit(1);
        }
        if (!user){
            fprintf(stderr,"--user is required, error.\n");
        exit(1);
        }
        if (strcmp (role, MANAGER_ROLE  ) != 0 &&
            strcmp (role, INSPECTOR_ROLE) != 0 ) {
                fprintf(stderr,"Role must be either manager or inspector.\n");
                exit(1);
            }
    // command checker
    for(int i=1; i<argc;i++){
        if (strcmp(argv[i],"--remove_district") == 0 && i+1 <argc){
            const char *district = argv[++i];
            remove_district(district,role,user);

            return 0;
        }
    if (strcmp(argv[i], "--add") == 0 && i + 1 < argc) {
                    const char *district = argv[++i];
                    ensure_district(district);
                    add(district, role, user);
                    log_action(district, role, user, "add");
                    update_symlink(district);
                    

                    return 0;
                }
    
                if (strcmp(argv[i], "--list") == 0 && i + 1 < argc) {
                    list(argv[++i], role);
                    return 0;
                }
    
                if (strcmp(argv[i], "--view") == 0 && i + 2 < argc) {
                    const char *district = argv[++i];
                    int rid = atoi(argv[++i]);
                    view(district, role, rid);
                    return 0;
                }
    
                if (strcmp(argv[i], "--remove_report") == 0 && i + 2 < argc) {
                    const char *district = argv[++i];
                    int rid = atoi(argv[++i]);
                    remove_report(district, role, user, rid);
                    log_action(district, role, user, "remove_report");
                    update_symlink(district);
                    return 0;
                }
    
                if (strcmp(argv[i], "--update_threshold") == 0 && i + 2 < argc) {
                    const char *district = argv[++i];
                    int val = atoi(argv[++i]);
                    update_threshold(district, role, user, val);
                    log_action(district, role, user, "update_threshold");
                    return 0;
                }
    
                if (strcmp(argv[i], "--filter") == 0 && i + 1 < argc) {
                    const char *district = argv[++i];
                    i++;
                    filter(district, role, argc - i, &argv[i]);
                    return 0;
                }
            }
 
        fprintf(stderr,"no valid command found.\n");
        return 1;
}