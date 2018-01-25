#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

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
                       "-d  Dictionary path\n"
                       "-v  Verbose output\n"
                       "-h  This help text\n";
    char *output_path = NULL;
    char *dict_path = NULL;
    bool verbose = false;

    char c;
    const char *optstring = ":o:d:hv";
    while ((c = getopt(argc, argv, optstring)) != -1)
    {
        switch (c)
        {
            case 'o':
                output_path = strdup(optarg);
                break;
            case 'd':
                dict_path = strdup(optarg);
                break;
            case 'v':
                verbose = true;
                break;
            case 'h':
                puts(help);
                free(output_path);
                free(dict_path);
                return 0;
            case ':':
                ERROR("Missing argument for -%c\n", optopt);
                free(output_path);
                free(dict_path);
                return EARG;
            case '?':
                ERROR("Unknown option -%c\n", optopt);
                free(output_path);
                free(dict_path);
                return EOPT;
        }
    }
    FILE *fdict = NULL;
    FILE *fout = NULL;

    if (output_path == NULL)  // Output path is required
    {
        ERROR("Output path is required!\n");
        free(dict_path);
        return EOUTPUT;
    }
    if (dict_path != NULL)  // If dictionary path is set
    {
        fdict = fopen(dict_path, "r");
        free(dict_path);
        if (fdict == NULL)
        {
            ERROR("Failed to open dictionary file.\n");
            free(output_path);
            return EFILEOPEN;
        }
    }
    fout = fopen(output_path, "w");
    free(output_path);
    if (fout == NULL)
    {
        ERROR("Failed to open output file.\n");
        if (fdict != NULL)
        {
            fclose(fdict);
        }
        return EFILEOPEN;
    }
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        ERROR("Failed to init curl.\n");
        fclose(fout);
        if (fdict != NULL)
        {
            fclose(fdict);
        }
        return EINIT;
    }
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);  // Disabling body output
    if (fdict != NULL)
    {
        // Use dictionary
        while (!feof(fdict))
        {
            char buffer[128];
            if (fscanf(fdict, "%s", buffer) == 1)
            {
                if (verbose)
                {
                    printf("Pending %s.%s...\t",buffer, TLD);
                }
                if (domain_available(curl, buffer, TLD))
                {
                    fprintf(fout, "%s\n", buffer);
                    if (verbose)
                    {
                        printf("available\n");
                    }
                }
                else if (verbose)
                {
                    printf("n/a\n");
                }
            }
        }
        fclose(fdict);
    }
    else
    {
        // Use brute-force
        // Stack of letters
        stack_t domain = STACK_INIT();
        // Fill the stack
        for (int i = 0; i < LENGTH; i++)
        {
            stack_push(&domain, 'a');
        }
        while (domain.top != 0)
        {
            if (verbose)
            {
                printf("Pending %s.%s...\t", domain.items, TLD);
            }
            if (domain_available(curl, domain.items, TLD))
            {
                fprintf(fout, "%s\n", domain.items);
                if (verbose)
                {
                    printf("available\n");
                }
            }
            else if (verbose)
            {
                printf("n/a\n");
            }
            if (domain.items[domain.top - 1] == 'z')
            {
                stack_pop(&domain, NULL);
                domain.items[domain.top - 1]++;  // next letter
            }
            else
            {
                if (domain.top == LENGTH)
                {
                    domain.items[domain.top - 1]++;
                }
                else
                {
                    stack_push(&domain, 'a');
                }
            }
        }
    }
    curl_easy_cleanup(curl);
    fclose(fout);
    return 0;
}
