#ifndef list_h
#define list_h

typedef struct data{
    char IP[16];
    char ID[11];
    int port;
    int room;
}Data;

typedef struct list{
    struct list* pre;
    struct list* next;
    Data data;
}List;


List* createList(){
    List* head = (List*)malloc(sizeof(List));
    head->pre = NULL;
    head->next = NULL;
    
    return head;
}

void insert(List* list, Data data){
    while(list->next != NULL)
        list = list->next;
    
    List* new_node = (List*)malloc(sizeof(List));
    new_node->pre = list;
    new_node->next = NULL;	// make new node & initialization
    
    list->data = data;		// copy data
    
    list->next = new_node;	// link
    list = list->next;
}

#endif /* list_h */