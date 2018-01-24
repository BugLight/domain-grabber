#include <stdio.h>

#include <curl/curl.h>

#define ERROR(...) fprintf(stderr, __VA_ARGS__)

#define EINIT 1

int main(int argc, char **argv)
{
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        ERROR("Failed to init curl.\n");
        return EINIT;
    }
    curl_easy_cleanup(curl);
    return 0;
}
