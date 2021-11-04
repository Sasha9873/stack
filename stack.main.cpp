#include "stack.h"
#include "stack.cpp"

/**
 * Do not forget to close the file_with_errors at the end!
 */
int main()
{
    int is_error;
    Stack st = {};

    st.file_with_errors = fopen("file_with_errors", "w+");

    stack_ctor(&st);

    stack_push(&st,10);
    stack_push(&st,659);
    stack_push(&st,7);
    stack_push(&st,77);

    stack_push(&st,89);
    stack_push(&st,744);
    stack_push(&st,89);
    stack_push(&st,744);
    stack_push(&st,10);
    stack_push(&st,659);
    stack_push(&st,7);
    stack_push(&st,77);

    int pop_element;
    pop_element = stack_pop(&st);

    stack_push(&st,192);
    pop_element = stack_pop(&st);
    stack_push(&st,8987);

    printf("pop = %d\n", pop_element);
    stack_pop(&st);
    if(stack_dtor(&st) == STACK_POINTER){
        printf("Pointer to stack consists of poison");
        return STACK_POINTER;
    }

    fclose(st.file_with_errors);

    return 0;
}
