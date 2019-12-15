#ifndef RCOMPILER_MASTER_HASHTABLE_H
#define RCOMPILER_MASTER_HASHTABLE_H

#define MAX_SIZE 1024

struct HNode_ {

    char *key;

    void *value;

    struct HNode_ *next;

};

struct HTable_ {

    struct HNode_ *table[MAX_SIZE];

};

typedef struct HNode_ HNode;

typedef struct HTable_ HTable;

HTable* createHashTable();

int containsKey(HTable *, char *);

void insert(HTable *, char *, void *);

void *get(HTable *, char *);

void delete(HTable *, char *);

void iterateKeys(HTable*, void (*)(char*));

#endif //RCOMPILER_MASTER_HASHTABLE_H
