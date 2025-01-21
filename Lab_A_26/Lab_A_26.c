#pragma warning(disable: 4996)

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#define M 12 // месяцев в году

typedef struct List {
    int day;
    int month;
    int year;
    float t;
    struct List* next;
}
List;

void PrintList(struct List* node) {
    for (; node != NULL; node = node->next)
        printf("%d/%d/%d %.1f\n", node->day, node->month, node->year, node->t);
}

void PrintListBelowZero(struct List* node) {
    for (; node != NULL && node->t < 0.0; node = node->next)
        printf("%d/%d/%d %.1f\n", node->day, node->month, node->year, node->t);
}

List* Clear(List* node) {
    while (node != NULL) {
        List* temp = node;
        node = node->next;

        free(temp);
    }

    return node;
}

int Search(List* node, float t) {

    while (node != NULL) {

        if (node->t == t)
            return 1;

        node = node->next;
    }
    return 0;
}

struct List* CreateNode(int day, int month, int year, float temperature) {
    struct List* temp = malloc(sizeof(struct List));

    if (temp != NULL) {
        temp->day = day;
        temp->month = month;
        temp->year = year;
        temp->t = temperature;
        temp->next = NULL;
    }

    return temp;
}

void InsertBefore(struct List* node, int day, int month, int year, float temperature) {
    struct List* temp = malloc(sizeof(struct List));

    if (temp != NULL && node != NULL) {
        temp->day = node->day;
        temp->month = node->month;
        temp->year = node->year;
        temp->t = node->t;

        temp->next = node->next;
        node->next = temp;

        node->day = day;
        node->month = month;
        node->year = year;
        node->t = temperature;
    }
}

int main(void) {
    FILE* fp = fopen("data.txt", "r");

    List* root = NULL;
    List** node;

    int day, month, year;
    float t;

    if (fp) {
        while ((fscanf(fp, "%d/%d/%d %f\n", &day, &month, &year, &t)) == 4) {

            node = &root;

            while (*node != NULL) {
                // условие сортировки по температуре и по дате если температуры совпадают
                if ((*node)->t > t || ((*node)->t == t &&
                    ((((*node)->month + (*node)->year * M) > (month + year * M)) ||
                        ((((*node)->month + (*node)->year * M) == (month + year * M)) &&
                            ((*node)->day > day)))

                    )) {
                    // вставляем элемент перед текущим 
                    InsertBefore(*node, day, month, year, t);
                    break;
                }
                else
                    node = &(*node)->next;
            }
            // если список дошел до конца или список изначально пустой то создаём элемент
            if (*node == NULL) {
                *node = CreateNode(day, month, year, t);
            }
        }
        fclose(fp);
    }

    printf("Sorted list:\n");
    PrintList(root);

    printf("\nList of days with negative average temperature:\n");
    PrintListBelowZero(root);

    printf("\n");
    float search_temp;
    while (printf("Type temperature to search for: ") && scanf("%f", &search_temp)) {
        if (Search(root, search_temp))
            printf("%.1f is on the list\n", search_temp);
        else
            printf("%.1f is NOT on the list\n", search_temp);
    }
    root = Clear(root);

    return 0;
}