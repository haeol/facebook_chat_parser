#ifndef _TRIE_H_
#define _TRIE_H_

#define TRIE_SZ 44

typedef struct TrieNode {
    struct TrieNode* next[TRIE_SZ];
    void* data;
} TrieNode;

TrieNode* createNode(void* data);
void destroyTrie(TrieNode* root, void* defaultData);
void insert(TrieNode* root, const char* word, void* dataPtr, void* defaultData);
void* search(TrieNode* root, const char* word, void* defaultData);
TrieNode* searchChar(TrieNode* root, const char c);
int trieHash(char ch);

#endif // _TRIE_H_

