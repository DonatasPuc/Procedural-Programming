/**
 * Tests singly linked list implementation
 * 
 * Author: Donatas Pucinskas
 * Date:   2020-12-15
 */

#include "linked_list.h"

int main()
{
    Node *head;
    initList(&head);

    // insertion of values into the list
    printf("Enter data into linked list. Enter 0 to finish\n");
    int value = -1;
    while (value != 0)
    {
        char c;
        printf("Enter an integer value: ");
        if (scanf("%d%c", &value, &c) != 2 || c != '\n')
        {
            printf("Illegal input!\n");
            while (getchar() != '\n')
                ;
        } 
        else if (value != 0)
        {
            push(&head, value);
        }
    }
    printList(head);

    // deletion of values from the list
    printf("\nEnter elements to be deleted from list. Enter 0 to finish\n");
    value = -1;
    while (value != 0)
    {
        char c;
        printf("Enter an integer value: ");
        if (scanf("%d%c", &value, &c) != 2 || c != '\n')
        {
            printf("Illegal input!\n");
            while (getchar() != '\n')
                ;
        } 
        else if (value != 0)
        {
            int err_code = deleteNode(&head, value);
            if (err_code) 
            {
                printf("Deletion error\n");
            }
        }
    }
    printList(head);

    destroyList(&head);
    return 0;
}