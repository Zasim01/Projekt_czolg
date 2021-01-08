#ifndef DEFINICJE_CZOLG
#define DEFINICJE_CZOLG

//funkcje deklaracje

typedef struct _Memory
{
    char *response;
    size_t size;
} Memory;

static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp);

char *make_request(char *url);
int info(char *token);
int move(char *token);
int rotate_left(char *token);
int rotate_right(char *token);
int explore(char *token);





#endif