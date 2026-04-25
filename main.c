

#include "city_manager.h"
 const char *role =NULL;
 const char *user =NULL;

int checker(char* argv[]){
    if ( strcmp( argv[0] , "./city_manager" ) != 0 ) return 1;
    if ( strcmp( argv[1] , "--role" ) != 0 ) return 1;
    if ( strcmp( argv[2] , "inspector" ) != 0  || strcmp ( argv [2], "manager") != 0) return 1;
    else role = argv[2];
    return 0;
}


int main (int argc, char* argv[]){

        if (argc <6 ){
            fprintf(stderr,"usage : <project> <--role> <role> <--command> <where> <extras...>");
            exit(1);
        }


        if ( checker(argv) ) {
            fprintf(stderr,"invalid arguments try again");
            exit(1);
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




        /*
city_manager --role inspector --add downtown
city_manager --role manager --remove_report downtown 17

*/

/*
. For example, before reading district.cfg, check that the owner-read bit is set;

*/

/*
 before writing to logged_district as an inspector, detect the restriction and refuse.
 
 */

        return 0;

}