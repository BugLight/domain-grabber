#include <stdlib.h>

#include "stack.h"

int stack_push(stack_t *s, item_t value)
{
    if (s->top == STACK_SIZE)
    {
        return 1;
    }
    s->items[s->top] = value;
    s->top++;
    return 0;
}

int stack_pop(stack_t *s, item_t *value)
{
    if (s->top == 0)
    {
        return 1;
    }
    s->top--;
    if (value != NULL)
    {
        *value = s->items[s->top];
    }
    return 0;
}
