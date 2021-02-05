/**
 * Implements singly linked list and some operations to work with it
 * 
 * Author: Donatas Pucinskas
 * Date:   2020-12-15
 */
#include "linked_list.h"

struct Node
{
    int data;
    struct Node *next;
};

void initList(Node **head)
{
    *head = NULL;
}

void destroyList(Node **head)
{
    Node *current = *head;
    Node *next;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

void push(Node **head, int val) 
{
    Node *new_node = (Node *) malloc(sizeof(Node));
    new_node->data = val;
    new_node->next = *head;
    *head = new_node;
}

int deleteNode(Node **head, int val)
{
    Node *current = *head;
    Node *previous;

    if (current == NULL) 
    {
        return 1;
    }

    // Delete node at the start of the list
    if (current->data == val)
    {
        *head = (*head)->next;
        free(current);
        return 0;
    }

    // Search for node to be deleted
    while (current != NULL)
    {
        if (current->data == val)
        {
            // Delete node
            previous->next = current->next;
            free(current);
            return 0;
        }
        previous = current;
        current = current->next;
    }
    return 1;
}

void printList(Node *node)
{
    while (node != NULL)
    {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");
}