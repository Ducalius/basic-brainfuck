typedef struct {
    int top;
    unsigned capacity;
    int *arr;
} Stack;

int stack_is_full(Stack *st){
    return st->top == st->capacity;
}

int stack_is_empty(Stack *st){
    return st->top == -1;
}

int stack_pop(Stack *st){
    if(!stack_is_empty(st)){
        return st->arr[st->top--];
    }
    else
        return 0;
}

void stack_push(Stack *st, int num){
    if (stack_is_full(st))
        return;
    st->arr[++st->top] = num;
}

Stack* make_new_stack(unsigned capacity){
    Stack* new_stack = (Stack*)malloc(sizeof(Stack));
    new_stack->capacity = capacity;
    new_stack->top = -1;
    new_stack->arr = (int*)malloc(new_stack->capacity * sizeof(int));
    return new_stack;
}
