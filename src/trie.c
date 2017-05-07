#ifndef _TRIE_C_
#define _TRIE_C_

#include <string.h>
#include <stdlib.h>
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
        case '&': ret = 32; break;
        case '#': ret = 33; break;
        case '0': ret = 34; break;
        case '1': ret = 35; break;
        case '2': ret = 36; break;
        case '3': ret = 37; break;
        case '4': ret = 38; break;
        case '5': ret = 39; break;
        case '6': ret = 40; break;
        case '7': ret = 41; break;
        case '8': ret = 42; break;
        case '9': ret = 43; break;
    }
    return ret;
}

TrieNode* createNode(void* data) {
    TrieNode* node = malloc(sizeof(TrieNode));
    for (int i = 0; i < TRIE_SZ; ++i) {
        node->next[i] = NULL;
    }
    node->data = data;
    return node;
}

void destroyTrie(TrieNode* root, void* defaultData) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < TRIE_SZ; ++i) {
        if (root->next[i] != NULL) {
            destroyTrie(root->next[i], defaultData);
        }
    }
    if (root->data != defaultData) {
        free(root->data);
        root->data = NULL;
    }
    free(root);
    root = NULL;
}

void insert(TrieNode* root, const char* word, void* data, void* defaultData) {
    TrieNode* curr  = root;
    
    int pos;
    int len = strlen(word);
    for (int i = 0; i < len; ++i) {
        pos = trieHash(word[i]);
        if (curr->next[pos] == NULL) {
            curr->next[pos] = createNode(defaultData);
        }
        curr = curr->next[pos];
    }
    curr->data = data;
}

void* search(TrieNode* root, const char* word, void* defaultData) {
    TrieNode* curr = root;
    int pos;
    int len = strlen(word);
    for (int i = 0; i < len; ++i) {
        pos = trieHash(word[i]);
        if (curr->next[pos] == NULL) {
            return defaultData;
        }
        curr = curr->next[pos];
    }
    return curr->data;
}

TrieNode* searchChar(TrieNode* root, const char c) {
    int hash = trieHash(c);
    if (root == NULL || hash < 0)
        return NULL;
    return root->next[trieHash(c)];
}

#endif // _TRIE_C_
