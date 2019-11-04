//
// Created by nuno on 04/11/19.
//

#include "linkedlist.h"
#include <stdio.h>

Node *initNode(void *value, Node *next) {

    Node *node = (Node *) malloc(sizeof(Node));

    node->value = value;
    node->next = next;

    return node;
}

Node *initNode(void *value, Node *next, Node *prev) {
    Node *node = initNode(value, next);

    prev->next = node;

    return node;
}

void eraseNode(Node *node, Node *prev, Node *next) {

    prev->next = next;

    free(node);

}

LinkedList *mkList(void *value) {

    LinkedList *list = (LinkedList *) malloc(sizeof(LinkedList));

    list->first = initNode(value, NULL);

    list->last = list->first;

    list->size = 1;

    return list;
}

LinkedList* concatValue(LinkedList *list, void *value) {

    list->last = initNode(value, NULL, list->last);

}

LinkedList* concatValue(void *value, LinkedList* list) {

    list->first = initNode(value, list->first, NULL);

}