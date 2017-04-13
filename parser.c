#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "status.h"
#include "trie.h"

void insertStatusTrie(TrieNode* root, char* key, status_t status, void* defaultData) {
    void* data = malloc(sizeof(status_t));
    *(status_t*)data = status;
    insert(root, key, data, defaultData);
}

status_t deref(void* data) {
    return *(status_t*)data;
}

TrieNode* initParserTrie() {
    void* defaultData = malloc(sizeof(status_t));
    *(status_t*)defaultData = NONE;
    TrieNode* root = createNode(defaultData);
    insertStatusTrie(root, "<span class=\"user\">",  USER_OPEN, defaultData);
    insertStatusTrie(root, "<span class=\"meta\">",  META_OPEN, defaultData);
    insertStatusTrie(root, "</span>",                SPAN_CLOSE, defaultData);
    insertStatusTrie(root, "<p>",                    MESSAGE_OPEN, defaultData);
    insertStatusTrie(root, "</p>",                   MESSAGE_CLOSE, defaultData);
    insertStatusTrie(root, "<div class=\"thread\">", CHAT_THREAD_OPEN, defaultData);
    insertStatusTrie(root, "</div>",                 DIV_CLOSE, defaultData);
    return root;
}

void writeBuf(char* buf, int* buf_pos, status_t* curr_status, status_t* read_status) {
    buf[*buf_pos] = 0;

    // name, date, content
    switch(*read_status) {

        case CHAT_THREAD_OPEN:
            printf("THREAD PEOPLE: "); break;
        case USER_OPEN:
            printf("\tUSERNAME: "); break;
        case META_OPEN:
            printf("\tDATE: "); break;
        case MESSAGE_OPEN:
            printf("\t\tMESSAGE: "); break;
        default: break;
    }
    printf("%s\n", buf);
    *buf_pos = 0;
    *curr_status = NONE;
    *read_status = NONE;
}

int main() {

    TrieNode* root = initParserTrie();
    TrieNode* curr = NULL;

    printf("Opening file\n");

    //FILE* fp = fopen("sample.html", "r");
    FILE* fp = fopen("messages.htm", "r");
    char ch;
    int buf_pos = 0;
    char buf[20000]; // char limit for facebook
    status_t curr_status = NONE;
    status_t read_status = NONE;
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
        if (curr != NULL && *(status_t*)curr->data != NONE) {
            curr_status = deref(curr->data);
        } else {
            // if status == USER_OPEN or META_OPEN or MESSAGE_OPEN, add to buffer
            switch (curr_status) {
                case CHAT_THREAD_OPEN:
                    // write thread information and reset status
                    if (ch == '<') {
                        writeBuf(buf, &buf_pos, &curr_status, &read_status);
                        curr = NULL;
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
                    writeBuf(buf, &buf_pos, &curr_status, &read_status);
                    break;

                /*
                case MESSAGE_CLOSE: 
                    writeBuf(buf, &buf_pos, &curr_status, &read_status);
                    break;
                */

                case DIV_CLOSE: 
                
                default:
                    break;
            }

        }
    }

    return 0;
}
