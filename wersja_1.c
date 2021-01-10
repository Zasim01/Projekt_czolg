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
    char *buffor[2245];

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

            FILE *fin = fopen("test.txt", "w+");
            fprintf(fin, "%s\n", chunk.response);
            fclose(fin);
        }

        free(chunk.response);
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

int zapisz_swiat(field a, field *A[50][50])
{
    A[50 + a.x][50 + a.y]->x = a.x;
    A[50 + a.x][50 + a.y]->y = a.y;
    A[50 + a.x][50 + a.y]->podloze = a.podloze;

    return 0;
}

int przypisanie_pola_explore(const char *const pole)
{
    const cJSON *resolution = NULL;
    const cJSON *payload = NULL;
    const cJSON *status = NULL;
    int wynik = 0;
    field a;
    field *A[50][50];

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
    cJSON_ArrayForEach(resolution, payload)
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
    field *A[50][50];
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

        printf(" %d\n%d\n%d\n", a.x, a.y, a.podloze);

        wynik = 1;
        goto end;

        wynik = 1;
        goto end;
    }

end:
    cJSON_Delete(pole_json);
    return wynik;
}

char wypisz_swiat(field *A[50][50])
{
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            //printf("%s", A[i][j]->podloze);
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    char *token = argc < 2 ? "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info/qwerty_36" : argv[1];
    move(token);
    return 0;
}

