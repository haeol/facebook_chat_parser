#ifndef _PARSER_TRIE_H_
#define _PARSER_TRIE_H_

#define TRIE_SZ 32

typedef struct TrieNode {
    struct TrieNode* next[TRIE_SZ];
    status_t status;
} TrieNode;

TrieNode* createNode();
void destroyTrie(TrieNode* root);
void insert(TrieNode* root, const char* word, status_t status);
status_t search(TrieNode* root, const char* word);
TrieNode* searchChar(TrieNode* root, const char c);
int trieHash(char ch);

#endif // _PARSER_TRIE_H_

