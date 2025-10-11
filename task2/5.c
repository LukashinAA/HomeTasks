#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    char* word;
    struct Node* next;
};

struct Node* CreateNode(char* val) 
{
    struct Node* NewNode = NULL;
    NewNode = calloc(1, sizeof(struct Node));
    if (NewNode == NULL) 
    {
        fprintf(stderr, "No enough memory\n");
        return NULL;
    }
    NewNode->word = malloc(strlen(val) + 1);
    if (NewNode->word == NULL) {
        fprintf(stderr, "No enough memory\n");
        return NULL;
    }
    NewNode->next = NULL;
    strcpy(NewNode->word, val);
    return NewNode;
}

void InsertInEnd(struct Node** head, char* val)
{
    struct Node* NewNode = CreateNode(val);
    if (NewNode != NULL) 
    {
        if (*head == NULL)
        {
            *head = NewNode;
            return;
        }
        struct Node* tmp = *head;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = NewNode;
    }
}

char* GetLastWord(struct Node* head) 
{
    if (head == NULL)
        return NULL;
    struct Node* tmp = head;
    while (tmp->next != NULL) 
        tmp = tmp->next;
    return tmp->word;
}

void DeleteWords(struct Node** head, char* LastWord)
{
    struct Node* tmp = *head;
    struct Node* prev = NULL;
    while (tmp->next != NULL) 
    {
        if (strcmp(tmp->word, LastWord) == 0) 
        {
            if (tmp == *head) 
            {
                *head = tmp->next;
                free(tmp);
                tmp = *head;
            }
            else
            {
            prev->next = tmp->next;
            free(tmp);
            tmp = prev->next;
            }
        }
        else 
        {
        prev = tmp;
        tmp = tmp->next;
        }
    }
}

void printNode(struct Node* head) 
{
    struct Node* tmp = head;
    while (tmp != NULL) 
    {
        printf("%s ",tmp->word);
        tmp = tmp->next;
    }
}

int main() {
    char result[1000] = "";
    char word[100];
    struct Node* list = NULL;
    while (scanf("%s",word) == 1) {
        InsertInEnd(&list, word);
    }
    printf("\n");
    if (list != NULL) 
    {
        char *LastWord = GetLastWord(list);
        DeleteWords(&list, LastWord);  
    }
    printNode(list);
}
