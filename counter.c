#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "status.h"


int main() {



    FILE *fp;
    fp = fopen("sample2.html", "r");

    //char* tag;
    char tag[100];
    char ch;
    int insideTag;
    int i;
    int pos;

    int count = 0;

    while ((ch = getc(fp)) != EOF) {

        /*
        switch(ch) {
            case '<':
                status = OPEN_TAG;
                i = 1;
                pos = ch == 's' ? 0 : 1;
                break;
            case '>': insideTag = 0; break;
            default: break;
        }

        switch(status) {
            case NONE:
                break;
            case OPEN_TAG:
                if (openingTags[pos][i++] != ch)
                    status = NONE;
                if (i == strlen(openingTags[pos]))
                    status = SPAN_TAG;
                break;
            case SPAN_TAG:
                break;
            case CLOSE_TAG:
                break;
            default: break; 
        }
        


        switch(ch) {
            case '<': insideTag = 1; break;
            case '>': insideTag = 0; break;
            default: break;
        }
        switch(insideTag) {
            case 0: break;
            case 1: break;
        }

        */
        printf("%d ", count);
        /*
        if (count%10000 == 0)
            printf("%s\n", &ch);//, count);
        */
        count++;

        //printf("%s", &ch);
    }


    return 0;
}
