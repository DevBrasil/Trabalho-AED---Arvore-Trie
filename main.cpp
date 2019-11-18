// CPP program to display content of Trie 
#include <iostream> 
#include <string.h> 
#define alpha_size 26 

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0]) 

// Converts key current character into index 
// use only 'a' through 'z' and lower case 
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') 
  
using namespace std; 
  const int ALPHABET_SIZE = 26;
// Trie node 
struct TrieNode  
{ 
    struct TrieNode* children[alpha_size]; 
  
    bool isLeaf; 
}; 
  
// // Returns new trie node (initialized to NULLs) 
// struct TrieNode* createNode() 
// { 
//     struct TrieNode* pNode = new TrieNode; 
  
//     for (int i = 0; i < alpha_size; i++) 
//         pNode->children[i] = NULL; 
  
//     pNode->isLeaf = false; 
  
//     return pNode; 
// }; 
// Returns new trie node (initialized to NULLs) 
struct TrieNode *getNode(void) 
{ 
    struct TrieNode *pNode = new TrieNode; 
    pNode->isLeaf = false; 
  
    for (int i = 0; i < ALPHABET_SIZE; i++) 
        pNode->children[i] = NULL; 
  
    return pNode; 
} 
// Returns true if key presents in trie, else 
// false 
bool search(struct TrieNode* root, string key) 
{ 
    struct TrieNode* pCrawl = root; 
  
    for (int i = 0; i < key.length(); i++) { 
        int index = key[i] - 'a'; 
        if (!pCrawl->children[index]) 
            return false; 
  
        pCrawl = pCrawl->children[index]; 
    } 
  
    return (pCrawl != NULL && pCrawl->isLeaf); 
} 
  
// If not present, inserts key into trie.  If the 
// key is prefix of trie node, just marks leaf node 
void insert(struct TrieNode *root,  const string key) 
{ 
    struct TrieNode *pCrawl = root; 
  
    for (int level = 0; level < key.length(); level++) 
    { 
        int index = CHAR_TO_INDEX(key[level]); 
        if (!pCrawl->children[index]) 
            pCrawl->children[index] = getNode(); 
  
        pCrawl = pCrawl->children[index]; 
    } 
  
    // mark last node as leaf 
    pCrawl->isLeaf = true; 
} 
// function to check if current node is leaf node or not 
bool isLeafNode(struct TrieNode* root) 
{ 
    return root->isLeaf != false; 
} 
// Returns 0 if current node has a child 
// If all children are NULL, return 1. 
bool isLastNode(struct TrieNode* root) 
{ 
    for (int i = 0; i < ALPHABET_SIZE; i++) 
        if (root->children[i]) 
            return 0; 
    return 1; 
} 
// Returns true if root has no children, else false 
bool isEmpty(TrieNode* root) 
{ 
    for (int i = 0; i < ALPHABET_SIZE; i++) 
        if (root->children[i]) 
            return false; 
    return true; 
} 
  

// Recursive function to delete a key from given Trie 
TrieNode* remove(TrieNode* root, string key, int depth = 0) 
{ 
    // If tree is empty 
    if (!root) 
        return NULL; 
  
    // If last character of key is being processed 
    if (depth == key.size()) { 
  
        // This node is no more end of word after 
        // removal of given key 
        if (root->isLeaf) 
            root->isLeaf = false; 
  
        // If given is not prefix of any other word 
        if (isEmpty(root)) { 
            delete (root); 
            root = NULL; 
        } 
  
        return root; 
    } 
  
    // If not last character, recur for the child 
    // obtained using ASCII value 
    int index = key[depth] - 'a'; 
    root->children[index] =  
          remove(root->children[index], key, depth + 1); 
  
    // If root does not have any child (its only child got  
    // deleted), and it is not end of another word. 
    if (isEmpty(root) && root->isLeaf == false) { 
        delete (root); 
        root = NULL; 
    } 
  
    return root; 
} 
  
// function to display the content of Trie 
void display(struct TrieNode* root, char str[], int level) 
{ 
    // If node is leaf node, it indicates end 
    // of string, so a null character is added 
    // and string is displayed 
    if (isLeafNode(root))  
    { 
        str[level] = '\0'; 
        cout << str << endl; 
    } 
  
    int i; 
    for (i = 0; i < alpha_size; i++)  
    { 
        // if NON NULL child is found 
        // add parent key to str and 
        // call the display function recursively 
        // for child node 
        if (root->children[i])  
        { 
            str[level] = i + 'a'; 
            display(root->children[i], str, level + 1); 
        } 
    } 
} 


// Recursive function to print auto-suggestions for given 
// node. 
void suggestionsRec(struct TrieNode* root, string currPrefix) 
{ 
    // found a string in Trie with the given prefix 
    if (root->isLeaf) 
    { 
        cout << currPrefix; 
        cout << endl; 
    } 
  
    // All children struct node pointers are NULL 
    if (isLastNode(root)) 
        return; 
  
    for (int i = 0; i < ALPHABET_SIZE; i++) 
    { 
        if (root->children[i]) 
        { 
            // append current character to currPrefix string 
            currPrefix.push_back(97 + i); 
  
            // recur over the rest 
            suggestionsRec(root->children[i], currPrefix); 
        } 
    } 
} 
  
// print suggestions for given query prefix. 
int printAutoSuggestions(TrieNode* root, const string query) 
{ 
    struct TrieNode* pCrawl = root; 
  
    // Check if prefix is present and find the 
    // the node (of last level) with last character 
    // of given string. 
    int level; 
    int n = query.length(); 
    for (level = 0; level < n; level++) 
    { 
        int index = CHAR_TO_INDEX(query[level]); 
  
        // no string in the Trie has this prefix 
        if (!pCrawl->children[index]) 
            return 0; 
  
        pCrawl = pCrawl->children[index]; 
    } 
  
    // If prefix is present as a word. 
    bool isWord = (pCrawl->isLeaf == true); 
  
    // If prefix is last node of tree (has no 
    // children) 
    bool isLast = isLastNode(pCrawl); 
  
    // If prefix is present as a word, but 
    // there is no subtree below the last 
    // matching node. 
    if (isWord && isLast) 
    { 
        cout << query << endl; 
        return -1; 
    } 
  
    // If there are are nodes below last 
    // matching character. 
    if (!isLast) 
    { 
        string prefix = query; 
        suggestionsRec(pCrawl, prefix); 
        return 1; 
    } 
} 

  
// Driver program to test above functions 
int main() 
{ 
   
   struct TrieNode* root = getNode(); 

    insert(root, "hello"); 
    insert(root, "Alecs"); 
    insert(root, "hell"); 
    insert(root, "cat"); 
    insert(root, "a"); 
    insert(root, "hel"); 
    insert(root, "help"); 
    insert(root, "helps"); 
    insert(root, "helping");

    int level = 0; 
    char str[20]; 
  
    // Displaying content of Trie 
    cout << "Conteudo da trie " << endl; 

    display(root, str, level); 
    
    cout << "Removendo" << endl;
    remove(root, "hello");
    //remove(root, "Alecs");
    cout << "Conteudo da trie " << endl; 
    display(root, str, level); 

    cout << "Imprimindo os prefixos" <<endl;
    printAutoSuggestions(root, "hel");
}
