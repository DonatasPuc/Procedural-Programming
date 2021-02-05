/**
 * Singly linked list header file
 * 
 * Author: Donatas Pucinskas
 * Date:   2020-12-15
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

// Initializes linked list
void initList(Node **head);

// Frees memory used for linked list
void destroyList(Node **head);

// Inserts a new node at the beggining of the list with provided value as data
void push(Node **head, int val);

// Deletes first occurrence of node from the list by provided value. Return 0 on success, 1 on error.
int deleteNode(Node **head, int val);

// Prints values in list from given node
void printList(Node *node);

#endif