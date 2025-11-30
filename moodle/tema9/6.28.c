#include <stdio.h>
#include <stdlib.h>

struct Node {
    double* elem;
    struct Node* next;
};

struct Node* CreateNode(double *elem) {
    struct Node* list = calloc(1,sizeof(struct Node));
    list->next = NULL;
    list->elem = malloc(sizeof(double));
    *list->elem = *elem;
    return list;
}

void Insert(struct Node** head, double* elem) {
    struct Node* NewNode = CreateNode(elem);
    if (*head == NULL) {
        *head = NewNode;
        return;
    }
    else {
        struct Node* tmp = *head;
        while (tmp->next != NULL) 
            tmp = tmp->next;
        tmp->next = NewNode;
    }   
}

void Swap(struct Node** head) {
    struct Node* tmp = *head;
    struct Node* q = NULL;
    struct Node* a = NULL;
    while (tmp->next->next != NULL) {
        tmp = tmp->next;
    }
    q = tmp->next;
    a = (*head)->next;
    tmp->next = *head;
    (*head)->next = NULL;
    *head = q;
    (*head)->next = a;
}

int main() {
    struct Node* list = NULL;
    double q;
    while (scanf("%lf",&q) == 1) {
        Insert(&list, &q);
    }
    struct Node* tmp = list;
    while (tmp != NULL) {
        printf("%lf ",*tmp->elem);
        tmp = tmp->next;
    }   
    Swap(&list);
    printf("\n");
    tmp = list;
    while (tmp != NULL) {
        printf("%lf ",*tmp->elem);
        tmp = tmp->next;
    }   
    
}
