#ifndef STACK_H
#define STACK_H

#define STACK_SIZE 16

typedef char item_t;
typedef struct stack stack_t;

struct stack
{
    int top;
    item_t items[STACK_SIZE];
};

#define STACK_INIT() {.top = 0}

int stack_push(stack_t *s, item_t value);

int stack_pop(stack_t *s, item_t *value);

#endif  // STACK_H
