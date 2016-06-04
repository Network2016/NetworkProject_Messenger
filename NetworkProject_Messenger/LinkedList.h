#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct node
{
    char IP[1024];
    int port;
    char ID[1024];
    int room;
    struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

//display the list
void printList()
{
    struct node *ptr = head;
    printf("\n[ ");
    
    //start from the beginning
    while(ptr != NULL)
    {
        printf("%s ",ptr->ID);
        ptr = ptr->next;
    }
    
    printf(" ]");
}

//insert link at the first location
void insertFirst(char* IP, int port, char* ID, int room)
{
    //create a link
    struct node *link = (struct node*) malloc(sizeof(struct node));
    
    strcpy(link->IP, IP);
    strcpy(link->ID, ID);
    link->port = port;
    link->room = room;
    
    //point it to old first node
    link->next = head;
    
    //point first to new first node
    head = link;
}

//delete first item
struct node* deleteFirst()
{
    //save reference to first link
    struct node *tempLink = head;
    
    //mark next to first link as first
    head = head->next;
    
    //return the deleted link
    return tempLink;
}

//is list empty
bool isEmpty()
{
    return head == NULL;
}

int length()
{
    int length = 0;
    struct node *current;
    
    for(current = head; current != NULL; current = current->next)
    {
        length++;
    }
    
    return length;
}

//find a link with given key
struct node* find(char* IP, int port){
    
    //start from the first link
    struct node* current = head;
    
    //if list is empty
    if(head == NULL)
    {
        return NULL;
    }
    
    //navigate through list
    while(!strcmp(current->IP, IP) && current->port == port){
        
        //if it is last node
        if(current->next == NULL){
            return NULL;
        }else {
            //go to next link
            current = current->next;
        }
    }
    
    //if data found, return the current Link
    return current;
}

//delete a link with given key
struct node* delete(char* IP, int port){
    
    //start from the first link
    struct node* current = head;
    struct node* previous = NULL;
    
    //if list is empty
    if(head == NULL){
        return NULL;
    }
    
    //navigate through list
    while(strcmp(current->IP, IP) || current->port != port){
        
        //if it is last node
        if(current->next == NULL){
            return NULL;
        }else {
            //store reference to current link
            previous = current;
            //move to next link
            current = current->next;
        }
        
    }
    
    //found a match, update the link
    if(current == head) {
        //change first to point to next link
        head = head->next;
    }else {
        //bypass the current link
        previous->next = current->next;
    }
    
    return current;
}


void reverse(struct node** head_ref) {
    struct node* prev   = NULL;
    struct node* current = *head_ref;
    struct node* next;
    
    while (current != NULL) {
        next  = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    
    *head_ref = prev;
}