#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "status.h"
#include "trie.h"

void insertStatusTrie(TrieNode* root, char* key, status_t status) {
    void* data = malloc(sizeof(status_t));
    *(status_t*)data = status;
    insert(root, key, data);
}

TrieNode* initParserTrie() {
    void* data = malloc(sizeof(status_t));
    *(status_t*)data = NONE;
    TrieNode* root = createNode(data);
    insertStatusTrie(root, "<span class=\"user\">",  USER_OPEN);
    insertStatusTrie(root, "<span class=\"meta\">",  META_OPEN);
    insertStatusTrie(root, "</span>",                SPAN_CLOSE);
    insertStatusTrie(root, "<p>",                    MESSAGE_OPEN);
    insertStatusTrie(root, "</p>",                   MESSAGE_CLOSE);
    insertStatusTrie(root, "<div class=\"thread\">", CHAT_THREAD_OPEN);
    insertStatusTrie(root, "</div>",                 DIV_CLOSE);
    return root;
}

void writeBuf(char* buf, int* buf_pos, status_t* curr_status) {
    buf[*buf_pos] = 0;
    printf("%s\n", buf);
    *buf_pos = 0;
    *curr_status = NONE;
}

status_t deref(void* data) {
    return *(status_t*)data;
}

int main() {

    TrieNode* root = initParserTrie();
    TrieNode* curr = NULL;

    printf("opening file\n");
    FILE* fp = fopen("messages.htm", "r");
    char ch;
    int buf_pos = 0;
    char buf[20000]; // char limit for facebook
    status_t curr_status = NONE;
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
        if (curr != NULL && deref(curr->data) != NONE) {
            curr_status = deref(curr->data);
        } else {
            // if status == USER_OPEN or META_OPEN or MESSAGE_OPEN, add to buffer
            switch (curr_status) {
                case CHAT_THREAD_OPEN:
                    // write thread information and reset status
                    if (ch == '<') {
                        writeBuf(buf, &buf_pos, &curr_status);
                        curr = NULL;
                        break;
                    }
                case USER_OPEN:
                case META_OPEN:
                case MESSAGE_OPEN:
                    buf[buf_pos++] = ch;
                    break;

                // read "</span>"
                case SPAN_CLOSE:
                    buf_pos = buf_pos + 1 - SPAN_CLOSE_LENGTH;

                    writeBuf(buf, &buf_pos, &curr_status);
                    /*
                    buf[buf_pos] = 0;
                    printf("%s\n", buf); // TODO write to file
                    buf_pos = 0;
                    curr_status = NONE;
                    */
                    break;

                // read </p>
                case MESSAGE_CLOSE:
                    buf_pos = buf_pos + 1 - MESSAGE_CLOSE_LENGTH;

                    writeBuf(buf, &buf_pos, &curr_status);
                    /*
                    buf[buf_pos] = 0;
                    printf("%s\n", buf); // TODO write to file
                    buf_pos = 0;
                    curr_status = NONE;
                    */

                    break;

                case DIV_CLOSE:
                
                default:
                    break;
            }

        }
    }

    return 0;
}
