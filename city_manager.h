#ifndef CITY_MANAGER_H 
#define CITY_MANAGER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<errno.h>

#define INSPECTOR_SIZE   32
#define CATEGORY_SIZE    32
#define DESCRIPTION_SIZE 120

#define MANAGER_ROLE   "manager"
#define INSPECTOR_ROLE "inspector"


typedef struct {
    int id;
    char insepctor[INSPECTOR_SIZE];
    double latitude;
    double longitude;
    char category[CATEGORY_SIZE];
    int severity;
    time_t timestamp;
    char description[DESCRIPTION_SIZE];
}Report;

#endif