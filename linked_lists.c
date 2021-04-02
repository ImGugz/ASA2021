#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int value;
    struct node * next;
} Node;

Node * initialize(){
    Node * head = (Node *) malloc(sizeof(Node));
    return head;
}

int top(Node * list){
    return list->value;
}

void pop(Node * list){
    list = (*list).next;
}

Node * push(Node * head, int i){
    Node * new = initialize();
    new->value = i;
    new->next = head;
    return new;
}

void print_list(Node * head){
    if(!head)
        return;
    while(head!=NULL){
        printf(">> %d ", head->value);
        head = head->next; 
    }
    printf("\n");
}

void destroy(Node * head){
    Node * next;
    while(head != NULL){
        next = head->next;
        free(head);
        head = next;
    }
}

/*int main(void){

    Node * head = NULL;

    head = push(head, 1);
    head = push(head, 2);
    head = push(head, 4);
    head = push(head, 2);
    head = push(head, 5);
    head = push(head, 3);

    print_list(head);

    int top_node = top(head);

    printf("%d\n", top_node);

    destroy(head);

    return 0;
}*/