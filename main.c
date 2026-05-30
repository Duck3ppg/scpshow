#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    FILE *pRawFile;
    CURL *curl;
    CURLcode res;
    char *tempfile = "/tmp/scpriptemp.html";

    int urllen = 40;
    char *url = calloc(urllen, sizeof(char));

    curl = curl_easy_init();

    if (curl) {
        pRawFile = fopen(tempfile, "wb");
        if (pRawFile == NULL) {
            printf("failed opening temp file");
            curl_easy_cleanup(curl);
            return -1;
        }
    }

    if (argc == 1) {
        printf("No arguments given, see use with scpshow -h");
        return -1;
    } else if (strcmp(argv[1], "-h") == 0) {
        printf(" Usage: scpshow <index of scp entry> \n Optional: output to a "
               "file using scpshow <index> > foo.txt\n");
        return 0;
    } else {
        //concat url with index
        char *scpindex = argv[1];

        strcat(url, "https://scp-wiki.wikidot.com/scp-");
        strcat(url, scpindex);
        //init curl
        curl_easy_setopt(curl,CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, pRawFile);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

        int res = curl_easy_perform(curl);
        if(res != CURLE_OK)
        {
            printf("Failed opening url. Connection issue? Is the SCP index valid?");
            remove(tempfile);
            curl_easy_cleanup(curl);
            free(url);
            return -1;
        }
        curl_easy_cleanup(curl);
        //convert output to plaintext
        int rs = system("xmllint --html  --xpath '//*[@id=\"page-content\"]' /tmp/scpriptemp.html | lynx -stdin -nolist -dump");
    }
}