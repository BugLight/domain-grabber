#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include "domain.h"

static char * full_url(const char *domain, const char *tld, bool secure)
{
    size_t domain_length = strlen(domain);
    size_t tld_length = strlen(tld);
    const char *prfx = "http://";
    if (secure)
    {
        prfx = "https://";
    }
    size_t prfx_length = strlen(prfx);
    char *url = calloc(prfx_length + domain_length + tld_length + 1, sizeof(char));
    if (url == NULL)
    {
        return NULL;
    }
    strcat(url, prfx);
    strcat(url, domain);
    strcat(url, ".");
    strcat(url, tld);
    return url;
}

int domain_available(CURL *curl, const char *domain, const char *tld)
{
    char *url = full_url(domain, tld, false);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    CURLcode res = curl_easy_perform(curl);
    free(url);
    if (res == CURLE_OK)
    {
        return 1;
    }
    url = full_url(domain, tld, true);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    res = curl_easy_perform(curl);
    free(url);
    if (res == CURLE_OK)
    {
        return 1;
    }
    return 0;
}
