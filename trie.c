#ifndef _PARSER_TRIE_C_
#define _PARSER_TRIE_C_

#include <string.h>
#include <stdlib.h>
#include "status.h"
#include "trie.h"

#include <stdio.h> // TODO debugging


int trieHash(char ch) {
    int ret = -1;
    switch(ch) {
        case 'a': ret = 0;  break;
        case 'b': ret = 1;  break;
        case 'c': ret = 2;  break;
        case 'd': ret = 3;  break;
        case 'e': ret = 4;  break;
        case 'f': ret = 5;  break;
        case 'g': ret = 6;  break;
        case 'h': ret = 7;  break;
        case 'i': ret = 8;  break;
        case 'j': ret = 9;  break;
        case 'k': ret = 10; break;
        case 'l': ret = 11; break;
        case 'm': ret = 12; break;
        case 'n': ret = 13; break;
        case 'o': ret = 14; break;
        case 'p': ret = 15; break;
        case 'q': ret = 16; break;
        case 'r': ret = 17; break;
        case 's': ret = 18; break;
        case 't': ret = 19; break;
        case 'u': ret = 20; break;
        case 'v': ret = 21; break;
        case 'w': ret = 22; break;
        case 'x': ret = 23; break;
        case 'y': ret = 24; break;
        case 'z': ret = 25; break;
        case '=': ret = 26; break;
        case '<': ret = 27; break;
        case '>': ret = 28; break;
        case '"': ret = 29; break;
        case ' ': ret = 30; break;
        case '/': ret = 31; break;
    }
    return ret;
}

TrieNode* createNode() {
    TrieNode* node = malloc(sizeof(TrieNode));
    for (int i = 0; i < TRIE_SZ; ++i) {
        node->next[i] = NULL;
    }
    node->status = NONE;
    return node;
}

void destroyTrie(TrieNode* root) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < TRIE_SZ; ++i) {
        destroyTrie(root->next[i]);
    }
    free(root);
}

void insert(TrieNode* root, const char* word, status_t status) {
    TrieNode* curr  = root;
    
    int pos;
    int len = strlen(word);
    for (int i = 0; i < len; ++i) {
        pos = trieHash(word[i]);
        if (curr->next[pos] == NULL) {
            curr->next[pos] = createNode();
        }
        curr = curr->next[pos];
    }
    curr->status = status;
}

status_t search(TrieNode* root, const char* word) {
    TrieNode* curr = root;
    int pos;
    int len = strlen(word);
    for (int i = 0; i < len; ++i) {
        pos = trieHash(word[i]);
        if (curr->next[pos] == NULL) {
            return NONE;
        }
        curr = curr->next[pos];
    }
    return curr->status;
}

TrieNode* searchChar(TrieNode* root, const char c) {
    int hash = trieHash(c);
    if (root == NULL || hash < 0)
        return NULL;
    return root->next[trieHash(c)];
}


/*
int main() {

    TrieNode* root = createNode();
    TrieNode* curr = NULL;

    insert(root, "<span class=\"user\">", USER_OPEN);
    insert(root, "<span class=\"meta\">", META_OPEN);
    insert(root, "</span>",               SPAN_CLOSE);
    insert(root, "<p>",                   MESSAGE_OPEN);
    insert(root, "</p>",                  MESSAGE_CLOSE);


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
        if (curr != NULL && curr->status != NONE) {
            curr_status = curr->status;
        } else {
            // if status == USER_OPEN or META_OPEN or MESSAGE_OPEN, add to buffer
            switch (curr_status) {
                case USER_OPEN:
                case META_OPEN:
                case MESSAGE_OPEN:
                    buf[buf_pos++] = ch;
                    break;

                // read "</span>"
                case SPAN_CLOSE:
                    buf_pos = buf_pos - SPAN_CLOSE_LENGTH + 1;
                    buf[buf_pos] = 0;

                    printf("%s\n", buf); // TODO write to file

                    buf_pos = 0;
                    curr_status = NONE;
                    break;

                // read </p>
                case MESSAGE_CLOSE:
                    buf_pos = buf_pos - MESSAGE_CLOSE_LENGTH;
                    buf[buf_pos] = 0;

                    printf("%s\n", buf); // TODO write to file

                    buf_pos = 0;
                    curr_status = NONE;
                    break;
                
                default:
                    break;
            }

        }
    }

    return 0;
}
*/


#endif // _PARSER_TRIE_C_
