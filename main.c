#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAMELENGTH 256
typedef struct {
    int report_id;
    char inspector_name[MAXNAMELENGTH];
    float latitude;
    float longitude;
    char cathegory[MAXNAMELENGTH];
    int severity;
    char user_message[MAXNAMELENGTH*4];
}Report; //rw-rw-r-- (664) 

//add <district_id>
void AddReport (){

}
void ListReports(){

}
//list <district_id>

void View (){

}
//view <district_id> <report_id>

void RemoveReport(){

}
//remove_report <district_id> <report_id>
void UpdateThreshold(){

}
//update_threshold <district_id> <value>
void LogOperations
void init 
int checker(){
    if ( strcmp( argv[0] , "./city_manager" ) != 0 ) return 1; 
    if ( strcmp( argv[1] , "--role" ) != 0 ) return 1; 
    if ( strcmp( argv[2] , "inspector" ) != 0  || strcmp ( argv [2], "manager") != 0) return 1; 
    
    return 0;
}
int main (int argc, char argv[]){
        if (argc <6 ){
            fprintf(stderr,"usage : <project> <--role> <role> <--command> <where> <extras...>")
            exit(-1);
        }
        if ( checker ) {
            fprintf(stderr,"invalid arguments try again");
            exit(-1);
        }

         char* role  = argv[2];

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



}