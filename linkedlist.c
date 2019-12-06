//
// Created by nuno on 04/11/19.
//

#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

Node *initNode(void *value, Node *next) {

    Node *node = (Node *) malloc(sizeof(Node));

    node->value = value;
    node->next = next;

    return node;
}

Node *initNodeWithPrev(void *value, Node *next, Node *prev) {
    Node *node = initNode(value, next);

    if (prev != NULL) {
        prev->next = node;
    }

    return node;
}

void eraseNode(Node *node, Node *prev, Node *next) {

    prev->next = next;

    free(node);

}

LinkedList *mkEmptyList() {
    LinkedList *list = (LinkedList *) malloc(sizeof(LinkedList));

    list->first = NULL;
    list->last = NULL;
    list->size = 0;

    return list;
}

LinkedList *mkList(void *value) {

    LinkedList *list = (LinkedList *) malloc(sizeof(LinkedList));

    list->first = initNode(value, NULL);

    list->last = list->first;

    list->size = 1;

    return list;
}

LinkedList *concatLast(LinkedList *list, void *value) {
    list->last = initNodeWithPrev(value, NULL, list->last);

    if (list->size == 0) {
        list->first = list->last;
    }

    list->size++;

    return list;
}

LinkedList *concatStart(void *value, LinkedList *list) {

    list->first = initNode(value, list->first);

    if (list->size == 0) {
        list->last = list->first;
    }

    list->size++;

    return list;
}

void *getFirst(LinkedList *list) {
    if (list->size == 0) {
        return NULL;
    }

    return list->first->value;
}

void *getLast(LinkedList *list) {
    if (list->size == 0) {
        return NULL;
    }

    return list->last->value;
}

LinkedList *concatLists(LinkedList *list1, LinkedList *list2) {

    if (list2->size == 0) {
        free(list2);

        return list1;
    }

    list1->last->next = list2->first;
    list1->last = list2->last;

    list1->size += list2->size;

    free(list2);

    return list1;
}

void iterateList(LinkedList *list, void (*printFunc)(void *)) {

    Node *current = list->first;

    while (current != NULL) {

        printFunc(current->value);

        current = current->next;
    }

}