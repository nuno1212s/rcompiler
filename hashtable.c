#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//int main() {
//
//    HTable *table = createHashTable();
//
//    char *n1 = "teste";
//
//    int i1 = 10;
//
//    insert(table, n1, &i1);
//
//    printf("%d\n", *((int*) get(table, n1)));
//
//    printf("%d\n", containsKey(table, n1));
//
//    delete(table, n1);
//
//    printf("%p\n", get(table, n1));
//
//    printf("%d\n", containsKey(table, n1));
//
//}

unsigned int
keyHash(unsigned char *str) {
    unsigned int hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % MAX_SIZE;
}

HNode *allocNode(char *key, void *value) {

    HNode *node = malloc(sizeof(HNode));

    node->key = key;

    node->value = value;

    return node;
}

void dealloc(HNode *node) {

    free(node);

}

HTable *createHashTable() {

    HTable *table = malloc(sizeof(HTable));

    return table;
}

int containsKey(HTable *table, char *key) {

    unsigned int hash = keyHash(key);

    if (table->table[hash] == NULL) {
        return 0;
    }

    HNode *node = table->table[hash];

    while (strcmp(node->key, key) != 0 && node->next != NULL) {
        node = node->next;
    }

    if (strcmp(node->key, key) == 0) {
        return 1;
    }

    return 0;
}

void insert(HTable *table, char *key, void *value) {

    unsigned int hash = keyHash(key);

    if (table->table[hash] == NULL) {

        HNode *node = allocNode(key, value);

        table->table[hash] = node;

        return;
    }

    HNode *node = table->table[hash];

    while (strcmp(key, node->key) != 0 && node->next != NULL) {
        node = node->next;
    }

    if (strcmp(key, node->key) == 0) {
        //Found an item with the same key

        node->value = value;
    } else {

        HNode *node = allocNode(key, value);

        node->next = node;
    }

}

void *get(HTable *table, char *key) {

    unsigned int hash = keyHash(key);

    if (table->table[hash] == NULL)
        return NULL;

    HNode *node = table->table[hash];

    while (strcmp(key, node->key) != 0 && node != NULL) {
        node = node->next;
    }

    if (strcmp(key, node->key) == 0) {
        return node->value;
    }

    return NULL;
}

void delete(HTable *table, char *key) {

    unsigned int hash = keyHash(key);

    if (table->table[hash] == NULL)
        return;

    HNode *node = table->table[hash],
            *prevNode = NULL;

    while (strcmp(key, node->key) != 0 && node->next != NULL) {

        prevNode = node;
        node = node->next;

    }

    if (strcmp(node->key, key) == 0) {

        if (prevNode != NULL)
            prevNode->next = node->next;
        else
            table->table[hash] = NULL;

        dealloc(node);
    }

}

void iterateKeys(HTable *table, void (*consumeKey)(char*)) {

    for (int i = 0; i < MAX_SIZE; i++) {

        if (table->table[i] == NULL) continue;

        HNode *node = table->table[i];

        while (node != NULL) {

            consumeKey(node->key);

            node = node->next;
        }


    }

}