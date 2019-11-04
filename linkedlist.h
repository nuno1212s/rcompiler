
#ifndef COMPILADORES_LINKEDLIST_H
#define COMPILADORES_LINKEDLIST_H

struct _Node {

    void *value;

    struct _Node *next;

};

typedef struct _Node Node;

struct _LinkedList {

    Node *first, *last;

    int size;

};

typedef struct _LinkedList LinkedList;

LinkedList *mkList(void *value);

LinkedList *concatValue(LinkedList *list, void *value);

LinkedList* concatValue(void *value, LinkedList *list);

#endif //COMPILADORES_LINKEDLIST_H
