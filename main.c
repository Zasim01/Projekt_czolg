#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "funkcje.h"
#include "../../cJSON/cJSON.h"


//zakładam,że maksymalnie może wykonać przy jednej linii komend 10 ruchów

int main(int argc, char **argv)
{   
    //"http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info/qwerty_36"

   
//char *url = argc < 2 ? "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info/qwerty_36" : argv[1];
    
    char *token = argc < 2 ? "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info/qwerty_36" : argv[1];
    //make_request("http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info/qwerty_36");

    for(int i = 1; i<11; i++){
        if (argv[i] != NULL){
            if (strcmp(argv[i], "E") == 0)
            {
                explore(token);

            }
            else if(strcmp(argv[i], "M") == 0)
            {
                move(token);
            } 
            else if(strcmp(argv[i], "L") == 0)
            {
                rotate_left(token);
            } 
            else if(strcmp(argv[i], "R") == 0)
            {
                rotate_right(token);
            } 


        } else{
            break;
        }
    } return 0;
}