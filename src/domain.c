#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include "domain.h"

static char * strjoin(const char *s[], int count, const char *sep)
{
    size_t size = 0;
    for (int i = 0; i < count; i++)
    {
        size += strlen(s[i]);
    }
    char *joined = calloc(size + 1, sizeof(char));
    if (joined == NULL)
    {
        return NULL;
    }
    const char *tmp_sep = "";
    for (int i = 0; i < count; i++)
    {
        strcat(joined, tmp_sep);
        strcat(joined, s[i]);
        tmp_sep = sep;
    }
    return joined;
}

static char * full_url(const char *domain, const char *tld, bool secure)
{
    const char *prfx = "http://";
    if (secure)
    {
        prfx = "https://";
    }
    const char *s[4];
    s[0] = prfx;
    s[1] = domain;
    s[2] = ".";
    s[3] = tld;
    return strjoin(s, 4, "");
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
