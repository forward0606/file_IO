#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<queue>
#define HashNumber 65
#define HashSize 100000
using namespace std;

char *getword(char *ptr, char *word){
    char *qtr = word;
    while(isspace(*ptr)){++ptr;}
    while(*ptr && !isspace(*ptr)){
        *qtr = *ptr;
        ++ptr, ++qtr;
    }
    *qtr = '\0';
    if(qtr == word){
        return NULL;
    }
    return ptr;
}

typedef struct Node{
    char *key;
    int cnt;
    struct Node *next;
    bool operator < (const Node &r)const{
        if(r.cnt != cnt){
            return cnt > r.cnt;
        }
        return strcmp(key, r.key) < 0? true:false;
    }
}Node;

typedef struct Hash{
    int maxSize;
    Node **hashTable;
}Hash;

Node *linsert(Node *head, char *k){
    Node *p = (Node *)malloc(sizeof(Node));
    p -> next = head;
    p -> key = strdup(k);
    p -> cnt = 1;
    return p;
}

Node *lfind(Node *head, char *k){
    Node *p = head;
    while(p){
        if(strcmp(p -> key, k) == 0){
            return p;
        }
        p = p -> next;
    }
    return p;
}

/*void ltraverse(Node *head){
    Node *p = head;
    while(p){

        p =  p -> next;
    }
}*/

void hashInit(Hash *db, int size){
    db -> maxSize = size;
    db -> hashTable = (Node **)malloc(sizeof(Node *)*size);
    for(int i=0;i<size;i++){
        db -> hashTable[i] = NULL;
    }
}

long long hashfunction(char *key, int size){
    char *p = key;
    unsigned long long sum = 0;
    while(*p){
        sum = sum * HashNumber + *p;
        sum %= size;
        p++;
    }
    return sum % size;
}

void HashInsert(Hash *db, char *key){
    int idx = hashfunction(key, db -> maxSize);
    db -> hashTable[idx] = linsert(db -> hashTable[idx], key);
}

Node *HashFind(Hash *db, char *key){
    int idx = hashfunction(key, db -> maxSize);
    return lfind(db -> hashTable[idx], key);
}

int main(){
    const int maxLine = 100000;
    char line[maxLine+5], word[maxLine];
    char *temp;
    Hash passage, all;
    hashInit(&passage, HashSize);
    hashInit(&all, maxLine);
    while(fgets(line, maxLine+5, stdin)){
        temp = line;
        while((temp = getword(temp, word))!=NULL){
            if(HashFind(&passage, word) == NULL){
                HashInsert(&passage, word);
            }
            //printf("%s", word);
        }
        //printf("getword finished!\n");
        Node *p, *tmp;
        for(int i=0;i<passage.maxSize;i++){
            p = passage.hashTable[i];
            while(p){
                tmp = HashFind(&all, p -> key);
                if(tmp == NULL){
                    HashInsert(&all, p -> key);
                }else{
                    tmp -> cnt++;
                }
                passage.hashTable[i] = p;
                p = p -> next;
                free(passage.hashTable[i]);
                passage.hashTable[i] = NULL;
            }
        }
    }
    Node *p;
    priority_queue<Node> pq;
    for(int i=0;i<all.maxSize;i++){
        p = all.hashTable[i];
        while(p){
            //printf("%s\t%d\n", p->key, p->cnt);
            if(pq.size() <10){
                pq.push(*p);
                //printf("%s < 10\n", p->key);
            }else if(*p < pq.top()){
                //printf("%s < %s\n", pq.top().key, p->key);
                free(pq.top().key);
                pq.pop();
                pq.push(*p);
            }else{
                free(p->key);
            }
            //printf("%s\n", pq.top().key);
            all.hashTable[i] = p;
            p = p -> next;
            free(all.hashTable[i]);
            all.hashTable[i] = NULL;
        }
    }
    char *A[15];
    int i=0;
    while(!pq.empty()){
        A[i] = pq.top().key;
        pq.pop();
        i++;
    }
    //printf("%d\n", i);
    for(int j=9;j>=0;j--){
        printf("%s\n", A[j]);
        free(A[j]);
    }
    return 0;
}
