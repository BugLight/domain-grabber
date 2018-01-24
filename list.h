#ifndef LIST_H
#define LIST_H

typedef char item_t;
typedef struct node node_t;

struct node
{
    item_t value;
    node_t *next;
};

int list_prepend(node_t **head, item_t value);

#endif  // LIST_H
