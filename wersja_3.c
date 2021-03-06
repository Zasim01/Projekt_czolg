#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "funkcje.h"
#include "cJSON/cJSON.h"

static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    Memory *mem = (Memory *)userp;

    char *ptr = NULL;

    if (mem->response != NULL)
        ptr = realloc(mem->response, mem->size + realsize + 1);
    else
        ptr = malloc(mem->size + realsize + 1);

    if (ptr == NULL)
        return 0;

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

char *make_request(char *url)
{

    CURL *curl;
    CURLcode res;
    Memory chunk;
    chunk.size = 0;
    chunk.response = NULL;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "Błąd! curl_easy_perform() niepowodzenie: %s\n", curl_easy_strerror(res));
        else
        {
            printf("%s", chunk.response);

            FILE *fin = fopen("qwerty_36.json", "w+");
            fprintf(fin, "%s\n", chunk.response);
            fclose(fin);

        
        }

        //free(chunk.response);
        curl_easy_cleanup(curl);
    }

    return chunk.response;
}

char info(char *token)
{
    token = make_request("http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info/qwerty_36");
}

char move(char *token)
{
    token = make_request("http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/move/qwerty_36");
}

char rotate_left(char *token)
{
    token = make_request("http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate/qwerty_36/left");
}

char rotate_right(char *token)
{
    token = make_request("http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate/qwerty_36/right");
}

char explore(char *token)
{
    token = make_request("http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/explore/qwerty_36");
}

typedef struct Field //struktura jednego pola
{
    int x;
    int y;
    char podloze;

} field;


int zapisz_swiat(field a, field A[20][20])
{   

    A[10 + a.x][10 + a.y].x = a.x;
    A[10 + a.x][10 + a.y].y = a.y;
    A[10 + a.x][10 + a.y].podloze = a.podloze;


    return 0;
}


int wypisz_swiat(field A[20][20]){
    for(int i =0; i<20; i++){
        for(int j =0; j<20; j++){
            printf("%d ", A[i][j].x);
        }printf("\n");
    }

}

int przypisanie_pola_explore(const char *const pole)
{
    const cJSON *item = NULL;
    const cJSON *payload = NULL;
    const cJSON *status = NULL;
    int wynik = 0;
    field a;
    field A[20][20];

    cJSON *pole_json = cJSON_Parse(pole);
    if (pole_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        wynik = 0;
        goto end;
    }

    status = cJSON_GetObjectItemCaseSensitive(pole_json, "status");
    if (cJSON_IsString(status) && (status->valuestring != NULL))
    {
        printf("Czy udało się wgrać: \"%s\"\n", status->valuestring);
    }

    payload = cJSON_GetObjectItemCaseSensitive(pole_json, "payload");
    cJSON_ArrayForEach(item, payload)
    {
        cJSON *current_x = cJSON_GetObjectItemCaseSensitive(payload, "current_x");
        cJSON *current_y = cJSON_GetObjectItemCaseSensitive(payload, "current_y");
        cJSON *field_type = cJSON_GetObjectItemCaseSensitive(payload, "field_type");

        if (!cJSON_IsNumber(current_x) || !cJSON_IsNumber(current_y))
        {
            wynik = 0;
            goto end;
        }

        if (cJSON_IsNumber(current_x) || cJSON_IsNumber(current_y))
        {
            a.x = current_x->valuedouble;
            a.y = current_y->valuedouble;
            a.podloze = field_type->valuedouble;

           zapisz_swiat(a, A);

            wynik = 1;
            goto end;
        }
    }

end:
    cJSON_Delete(pole_json);
    return wynik;
}

int przypisanie_pola_zmiennym(const char *const pole)
{
    field a;
    field A[20][20] = {0};
    const cJSON *payload = NULL;
    const cJSON *status = NULL;
    int wynik = 0;

    cJSON *pole_json = cJSON_Parse(pole);
    if (pole_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        wynik = 0;
        goto end;
    }

    status = cJSON_GetObjectItemCaseSensitive(pole_json, "status");
    if (cJSON_IsString(status) && (status->valuestring != NULL))
    {
        printf("Czy sie wgrał \"%s\"\n", status->valuestring);
    }

    payload = cJSON_GetObjectItemCaseSensitive(pole_json, "payload");

    cJSON *current_x = cJSON_GetObjectItemCaseSensitive(payload, "current_x");
    cJSON *current_y = cJSON_GetObjectItemCaseSensitive(payload, "current_y");
    cJSON *field_type = cJSON_GetObjectItemCaseSensitive(payload, "field_type");

    if (!cJSON_IsNumber(current_x) || !cJSON_IsNumber(current_y))
    {
        wynik = 0;
        goto end;
    }
    else
    {
        a.x = current_x->valuedouble;
        a.y = current_y->valuedouble;
        a.podloze = field_type->valuedouble;
        printf("%d\n", a.x);


        zapisz_swiat(a,A);
        

        printf("%s\n", field_type->valuestring);


        wypisz_swiat(A);

        wynik = 1;
        goto end;

    }

end:
    cJSON_Delete(pole_json);
    return wynik;
}


int main(int argc, char **argv)
{
    field a;
    field A[20][20] = {0};
  
      

    // //przypisanie_pola_zmiennym(argv[1]);
    char *token = argc < 2 ? "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info/qwerty_36" : argv[1];
    rotate_right(token);
    char buffer[2048];

    // for(int i = 1; i<argv[i]; i++){
    //     if (argv[i] != NULL){
    //         if (strcmp(argv[i], "E") == 0)
    //         {
    //             explore(token);

    //         }
    //         else if(strcmp(argv[i], "M") == 0)
    //         {
    //             move(token);
    //         } 
    //         else if(strcmp(argv[i], "L") == 0)
    //         {
    //             rotate_left(token);
    //         } 
    //         else if(strcmp(argv[i], "R") == 0)
    //         {
    //             rotate_right(token);
    //         } 


    //     } else{
    //         break;
    //     }
    // }

    FILE *fin = fopen("qwerty_36.json", "r");
    fread(buffer,1,2048,fin);
    fclose(fin);
    przypisanie_pola_zmiennym(buffer);
    
    //wypisz_swiat(A);
    
     


    return 0;
}

