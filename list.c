#include <stdlib.h>

#include "list.h"

int list_prepend(node_t **head, item_t value)
{
    node_t *new = malloc(sizeof(item_t));
    if (new == NULL)
    {
        return 1;
    }
    new->value = value;
    new->next = *head;
    *head = new;
    return 0;
}
