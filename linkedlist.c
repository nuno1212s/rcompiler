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

    prev->next = node;

    return node;
}

void eraseNode(Node *node, Node *prev, Node *next) {

    prev->next = next;

    free(node);

}

LinkedList *mkEmptyList() {
    LinkedList *list = (LinkedList *) malloc(sizeof(LinkedList));

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
    list->size++;

}

LinkedList *concatStart(void *value, LinkedList *list) {

    list->first = initNode(value, list->first);
    list->size++;

}

void iterateList(LinkedList *list, void (*printFunc)(void *)) {

    Node *current = list->first;

    while (current != NULL) {

        printFunc(current->value);

        current = current->next;
    }

}