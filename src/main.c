#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <curl/curl.h>
// TODO: autotools

#include "stack.h"
#include "domain.h"

#define ERROR(...) do { fprintf(stderr, __VA_ARGS__); printf("Use -h for help\n"); } while (0)

#define EINIT 1
#define EARG 2
#define EOPT 3
#define EOUTPUT 4
#define EFILEOPEN 5

#define LENGTH 10  // Note that STACK_SIZE is 16
#define TLD "ru"  // TODO: Different top level domains

int main(int argc, char **argv)
{
    const char *help = "Usage: dgrabber [options...]\n"
                       "Options:\n"
                       "-o  Output path\n"
                       "-h  This help text\n";
    char *output_path = NULL;

    char c;
    const char *optstring = ":o:h";
    while ((c = getopt(argc, argv, optstring)) != -1)
    {
        switch (c)
        {
            case 'o':
                output_path = strdup(optarg);
                break;
            case 'h':
                puts(help);
                return 0;
            case ':':
                ERROR("Missing argument for -%c\n", optopt);
                return EARG;
            case '?':
                ERROR("Unknown option -%c\n", optopt);
                return EOPT;
        }
    }
    if (output_path == NULL)
    {
        ERROR("Output path is required!\n");
        return EOUTPUT;
    }
    FILE *fout = fopen(output_path, "w");
    free(output_path);
    if (fout == NULL)
    {
        ERROR("Failed to open output file.\n");
        return EFILEOPEN;
    }
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        ERROR("Failed to init curl.\n");
        fclose(fout);
        return EINIT;
    }
    // Stack of letters
    stack_t domain = STACK_INIT();
    // Fill the stack
    for (int i = 0; i < LENGTH; i++)
    {
        stack_push(&domain, 'a');
    }
    while (domain.top != 0)
    {
        if (domain_available(curl, domain.items, TLD))
        {
            fprintf(fout, "%s\n", domain.items);
        }
        if (domain.items[domain.top - 1] == 'z')
        {
            stack_pop(&domain, NULL);
            domain.items[domain.top - 1]++;  // next letter
        }
        if (domain.top == LENGTH)
        {
            domain.items[domain.top - 1]++;
        }
        else
        {
            stack_push(&domain, 'a');
        }
    }
    curl_easy_cleanup(curl);
    fclose(fout);
    return 0;
}
