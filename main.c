#include <stdio.h>

#include <curl/curl.h>
// TODO: autotools

#include "stack.h"
#include "domain.h"

#define ERROR(...) fprintf(stderr, __VA_ARGS__)

#define EINIT 1

#define LENGTH 10  // Note that STACK_SIZE is 16
#define TLD "ru"  // TODO: Different top level domains

int main(int argc, char **argv)
{
    // TODO: Command line arguments interface
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        ERROR("Failed to init curl.\n");
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
        // TODO: File output
        printf("%s ", domain.items);
        if (domain_available(curl, domain.items, TLD))
        {
            printf("Available\n");
        }
        else
        {
            printf("N/A\n");
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
    return 0;
}
