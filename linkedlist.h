
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

LinkedList *mkEmptyList();

LinkedList *mkList(void *value);

void *getFirst(LinkedList *);

void *getLast(LinkedList *);

LinkedList *concatLists(LinkedList *list1, LinkedList*list2);

LinkedList *concatLast(LinkedList *list, void *value);

LinkedList *concatStart(void *value, LinkedList *list);

void iterateList(LinkedList *, void (*)(void *));

#endif //COMPILADORES_LINKEDLIST_H
