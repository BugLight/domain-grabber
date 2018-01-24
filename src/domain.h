#include <curl/curl.h>

#ifndef DOMAIN_H
#define DOMAIN_H

int domain_available(CURL *curl, const char *domain, const char *tld);

#endif  // DOMAIN_H
