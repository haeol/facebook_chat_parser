#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "status.h"
#include "trie.h"


#define CSV_DIRECTORY "./fbthreads/"
#define HEADERS "name, date, message"
FILE *OUTFILE;
FILE *THREAD_INDEX;

void insertParserTrie(TrieNode* root, char* key, parser_status_t status, void* defaultData) {
    void* data = malloc(sizeof(parser_status_t));
    *(parser_status_t*)data = status;
    insert(root, key, data, defaultData);
}

TrieNode* initParserTrie() {
    void* defaultData = malloc(sizeof(parser_status_t));
    *(parser_status_t*)defaultData = NONE;
    TrieNode* root = createNode(defaultData);
    insertParserTrie(root, "<span class=\"user\">",  USER_OPEN, defaultData);
    insertParserTrie(root, "<span class=\"meta\">",  META_OPEN, defaultData);
    insertParserTrie(root, "</span>",                SPAN_CLOSE, defaultData);
    insertParserTrie(root, "<p>",                    MESSAGE_OPEN, defaultData);
    insertParserTrie(root, "</p>",                   MESSAGE_CLOSE, defaultData);
    insertParserTrie(root, "<div class=\"thread\">", CHAT_THREAD_OPEN, defaultData);
    insertParserTrie(root, "</div>",                 DIV_CLOSE, defaultData);
    return root;
}

/*
// Trie to decode html encoded characters
void insertDecoderTrie(TrieNode* root, char* key, parser_status_t status, void* defaultData) {

}



TrieNode* initDecoderTrie() {
    void* defaultData = NULL;
    TrieNode* root = createNode(defaultData);
    return root;
}
*/

void writeBuf(FILE* f, char* buf, int* buf_pos, parser_status_t* curr_status, parser_status_t* read_status) {
    buf[*buf_pos] = 0;

    // name, date, message
    switch(*read_status) {
        case USER_OPEN:
        case META_OPEN:
            fprintf(f, "\"%s\", ", buf); break;
        case MESSAGE_OPEN:
            fprintf(f, "\"%s\"\n", buf); break;
        default: break;
    }
    //fprintf(f, "%s", buf);
    *buf_pos = 0;
    *curr_status = NONE;
    *read_status = NONE;
}


int main() {

    TrieNode* root = initParserTrie();
    TrieNode* curr = NULL;
 
    mkdir(CSV_DIRECTORY, 0700);

    FILE* fp = fopen("messages.htm", "r");
    char ch;
    int buf_pos = 0;
    char buf[20000]; // char limit for facebook

    int thread_index = 1;
    char outfile_name[100];
    sprintf(outfile_name, "%s%05d.csv", CSV_DIRECTORY, thread_index);
    THREAD_INDEX = fopen("index.txt", "w");

    parser_status_t curr_status = NONE;
    parser_status_t read_status = NONE;
    printf("Starting loop\n");

    int count = 0;

    while ((ch = getc(fp)) != EOF) {
        // if ch == '<' start searching, assign curr = root
        if (ch == '<') {
            curr = root;
        }

        // if curr != null, search for curr char and assign curr to next via char
        // check status for leaf or status != NONE, if not NONE, reset buf pos to 0
        curr = searchChar(curr, ch);
        if (curr != NULL && *(parser_status_t*)curr->data != NONE) {
            curr_status = *(parser_status_t*)curr->data;
        } else {
            // if status == USER_OPEN or META_OPEN or MESSAGE_OPEN, add to buffer
            switch (curr_status) {
                case CHAT_THREAD_OPEN:
                    // grab only name or fb id
                    if (ch == '&') {
                        curr_status = CHAT_THREAD_STALL;
                        break;
                    }
                case USER_OPEN:
                case META_OPEN:
                case MESSAGE_OPEN:
                    read_status = curr_status;
                    buf[buf_pos++] = ch;
                    break;

                // read </span>
                // read </p>
                case SPAN_CLOSE: 
                case MESSAGE_CLOSE:
                    if (curr_status == SPAN_CLOSE) {
                        buf_pos = buf_pos + 1 - SPAN_CLOSE_LENGTH;
                    } else if (curr_status == MESSAGE_CLOSE) {
                        buf_pos = buf_pos + 1 - MESSAGE_CLOSE_LENGTH;
                    }
                    writeBuf(OUTFILE, buf, &buf_pos, &curr_status, &read_status);
                    break;

                case CHAT_THREAD_STALL:
                    if (ch == ',') {
                        curr_status = CHAT_THREAD_OPEN;
                        buf[buf_pos++] = ch;
                    }
                    // write thread information and reset status
                    else if (ch == '<') {

                        // write to file
                        buf[buf_pos] = 0;
                        fprintf(THREAD_INDEX, "%d: [%s]\n", thread_index, buf);

                        // reset buffer and status
                        buf_pos = 0;
                        curr_status = read_status = NONE;

                        // close file
                        fclose(OUTFILE);

                        // open new file
                        sprintf(outfile_name, "%s%05d.csv", CSV_DIRECTORY, thread_index++);
                        OUTFILE = fopen(outfile_name, "w");
                        fprintf(OUTFILE, "%s\n", HEADERS);

                        // reset curr
                        curr = NULL;
                        break;
                    }
                case DIV_CLOSE: 
                
                default:
                    break;
            }

        }
    }
    fclose(OUTFILE);
    fclose(THREAD_INDEX);

    return 0;
}
