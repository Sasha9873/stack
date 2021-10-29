//Failed after seventh element(maybe during push it)
#include "stack.h"
#include "stack.cpp"

int main()
{
    int is_error;
    Stack st = {};

    st.file_with_errors = fopen("file_with_errors", "w+");

    //printf("\nfile: %p\n\n", st.file_with_errors);
    //fprintf(st.file_with_errors, "1234567\n");

    stack_ctor(&st);

    stack_push(&st,10);
    stack_push(&st,659);
    stack_push(&st,7);
    stack_push(&st,77);

    stack_push(&st,89);
    stack_push(&st,744);

    int pop_element;
    pop_element = stack_pop(&st);

    stack_push(&st,192);
    pop_element = stack_pop(&st);
    stack_push(&st,8987);


    stack_push(&st,37947);
    /**/
    printf("st = %d", st.current_size);

    //fclose(st.file_with_errors);
    //printf("\nfile: %p\n\n", st.file_with_errors);
    return 0;
}

int stack_ctor(Stack* stack1)
{
    if(stack1->data == POINTER_13){

        #ifdef DATA_USE_CANARY
            stack1->data = (int*)calloc(stack1->capacity + 2, sizeof(int));
        #else
        stack1->data = (int*)calloc(stack1->capacity, sizeof(int));
        #endif  // DATA_USE_CANARY

        if(stack1->data == NULL){
                CHECKSTACK(NOT_MEMORY);
                return NOT_MEMORY;
        }

        #ifdef DATA_USE_CANARY
            stack1->data[0] = CANARY_VALUE;
            stack1->data[1] = CANARY_VALUE;
        #endif  // DATA_USE_CANARY

        //printf("can1 = %d can2 = %d\n", stack1->data[0], stack1->data[1]);

        stack1->current_size = 0;
    }
    else{
        CHECKSTACK(TWO_CTOR);
        return TWO_CTOR;
    }

    CHECKSTACK(ALL_OK);

    return 0;
}


int stack_push(Stack* stack1, int value)
{
    CHECKSTACK(ALL_OK);
    //printf("\nfile: %p\n\n", stack1->file_with_errors);
    //printf("stok = %d can2 = %d cur size = %d data = %p, %p\n", stack_ok(stack1), stack1->data[stack1->capacity + 1], stack1->current_size), stack1->data, POINTER_13;

    int* new_memory = NULL;

    stack1->current_size++;

    printf("%p %d size = %d\n", &stack1->data[stack1->current_size - 1], stack1->data[stack1->current_size - 1], stack1->current_size);
    fflush(stdout);

    #ifdef DATA_USE_CANARY
        if(stack1->current_size > stack1->capacity){

            if((new_memory = (int*)realloc(stack1->data, stack1->current_size * CHANGE + 2)) == NULL){
                CHECKSTACK(NOT_MEMORY);
                return NOT_MEMORY;
            }
            else{
                stack1->data = new_memory;

                *(stack1->data + stack1->current_size * CHANGE + 1) = *(stack1->data + stack1->current_size); //replace end canary

                stack1->capacity = stack1->current_size * CHANGE;
                printf("cap = %d\n", stack1->current_size);

                if(CHANGE == 2)
                    CHANGE = 3;
                else
                    CHANGE = 2;
            }
        }
        *(stack1->data + stack1->current_size) = value;  //add new value in stack
        printf("val = %d, data = %d\n", value, *(stack1->data + stack1->current_size));
    #else
        if(stack1->current_size > stack1->capacity){
            if((new_memory = (int*)realloc(stack1->data, stack1->current_size * CHANGE)) == NULL){
                CHECKSTACK(NOT_MEMORY);
                return NOT_MEMORY;
            }

            else{
                stack1->data = new_memory;

                stack1->capacity = stack1->current_size * CHANGE;

                if(CHANGE == 2)
                    CHANGE = 3;
                else
                    CHANGE = 2;

            }
        }
        printf("%p %d\n", stack1->data + stack1->current_size - 1, *(stack1->data + stack1->current_size - 1));

        *(stack1->data + stack1->current_size - 1) = value;   //add new value in stack

    #endif // DATA_USE_CANARY
    printf("pointer to data %p value = %d size = %d\n", &stack1->data[stack1->current_size - 1], stack1->data[stack1->current_size - 1], stack1->current_size);
    fflush(stdout);
    //printf("Push1 %d curs = %d\n",*(stack1->data + stack1->current_size), stack1->current_size);

    //printf("Push %d curs = %d can2= %d\n",*(stack1->data + stack1->current_size), stack1->current_size, *(stack1->data + stack1->capacity + 1));

    CHECKSTACK(ALL_OK);/**/

    return EXIT_SUCCESS;
}

errors_t stack_ok(Stack* stack1)
{
    if(!stack1)
        return STACK_POINTER;

    #ifdef STACK_USE_CANARY
    if(stack1->begin_canary != CANARY_VALUE)
        return BEGIN_CANARY;

    if(stack1->end_canary != CANARY_VALUE)
        return END_CANARY;
    #endif // STACK_USE_CANARY

    #ifdef DATA_USE_CANARY
        if(stack1->data[0] != CANARY_VALUE)
            return DATA_BEGIN_CANARY;

        if(stack1->data[stack1->capacity + 1] != CANARY_VALUE)
            return DATA_END_CANARY;
    #endif // DATA_USE_CANARY

    if(stack1->capacity < 0)
        return NEGATIVE_CAPASITY;

    //printf(",m,m = %d\n", stack1->current_size < 0);
    if(stack1->current_size < 0)
        return NEGATIVE_SIZE;

    if(stack1->capacity < stack1->current_size)
        return CAP_SMALLER_SIZE;

    if(stack1->data == POINTER_13)
        return DATA_POINTER;

    #ifdef STACK_USE_HASH
        previous_hash_value = hash_value;
        hash_value = stack_hash(stack1, ALL_OK);
    #endif // STACK_USE_HASH

    return ALL_OK;

}

int stack_dump(Stack* stack1, errors_t reason)
{
    if(reason != 0)
        fprintf(stack1->file_with_errors, "Dump was called because %s(%d)\n", error_names[abs(reason)], reason);

    #ifdef STACK_USE_HASH
        previous_hash_value = hash_value;
        hash_value = stack_hash(stack1, ALL_OK);
        if(previous_hash_value == hash_value)
            fprintf(stack1->file_with_errors, "Hash is ok\n");
        else
            fprintf(stack1->file_with_errors, "Hash is not ok: previous hash was %d, current hash is %d\n", previous_hash_value, hash_value);
    #endif // STACK_USE_HASH

    fprintf(stack1->file_with_errors, "Stack[%p]", stack1);

    errors_t error = stack_ok(stack1);
    if(error == ALL_OK)
        fprintf(stack1->file_with_errors, "(ok)\n");
    else
        fprintf(stack1->file_with_errors, "ERROR %d %s", error, error);

    fprintf(stack1->file_with_errors, "{\n");

    #ifdef STACK_USE_CANARY
        fprintf(stack1->file_with_errors, "   stack_begin_canary = %x\n", stack1->begin_canary);
    #endif // STACK_USE_CANARY

    fprintf(stack1->file_with_errors, "   capacity = %ld\n   current size = %ld\n", stack1->capacity, stack1->current_size);

    fprintf(stack1->file_with_errors, "   data[%p] = %p\n   {\n", &stack1->data, stack1->data);

    #ifdef DATA_USE_CANARY
        fprintf(stack1->file_with_errors, "      c[o](first canary) = %x\n", stack1->data[0]);

        for(int num = 1; num <= stack1->capacity; num++){
            if(num <= stack1->current_size)
                fprintf(stack1->file_with_errors, "      *");
            else
                fprintf(stack1->file_with_errors, "      ");
            fprintf(stack1->file_with_errors, "[%d] = %d\n", num, stack1->data[num]);
        }
        fprintf(stack1->file_with_errors, "      c[%d](second canary)(%p) = %x\n", stack1->capacity + 1, &stack1->data[stack1->capacity + 1], stack1->data[stack1->capacity + 1]);
    #else
        for(int num = 0; num < stack1->capacity; num++){
            if(num <= stack1->current_size - 1)
                fprintf(stack1->file_with_errors, "      *");
            else
                fprintf(stack1->file_with_errors, "      ");
            fprintf(stack1->file_with_errors, "[%d](%p, %p) = %d\n", num, &(stack1->data[num]), stack1->data + num, stack1->data[num]);
        }
    #endif // DATA_USE_CANARY

    fprintf(stack1->file_with_errors, "   }\n");

    fprintf(stack1->file_with_errors, "   file_with_errors[%p] = %p\n", &stack1->file_with_errors, stack1->file_with_errors);

    #ifdef STACK_USE_CANARY
        fprintf(stack1->file_with_errors, "   stack_end_canary = %x\n", stack1->end_canary);
    #endif // STACK_USE_CANARY

    fprintf(stack1->file_with_errors, "}\n\n");
   //fprintf(file_with_symbols, "Error: in file %s in function %s in %d line,  \n", __FILE__, __FUNCTION__, __LINE__);

   return 0;
}

int stack_pop(Stack* stack1)
{
    CHECKSTACK(ALL_OK);
    //stack_ok(stack1);

    int pop_element = POISON;

    #ifdef DATA_USE_CANARY
        pop_element = stack1->data[stack1->current_size];
        stack1->data[stack1->current_size] = POISON;
        printf("cur_size = %ld  pop(pop) = %d\n", stack1->current_size, stack1->data[stack1->current_size]);
    #else
        pop_element = stack1->data[stack1->current_size - 1];
        stack1->data[stack1->current_size - 1] = POISON;
        printf("cur_size = %ld  pop(pop) = %d\n", stack1->current_size, stack1->data[stack1->current_size - 1]);
    #endif // DATA_USE_CANARY

    stack1->current_size--;

    if(stack1->current_size <= stack1->capacity / CHANGE){
        #ifdef DATA_USE_CANARY
            stack1->data[stack1->capacity / CHANGE + 1] = stack1->data[stack1->capacity + 1]; //end canary

            stack1->data = (int*)realloc(stack1->data, stack1->capacity / CHANGE + 2);
        #else
            stack1->data = (int*)realloc(stack1->data, stack1->capacity / CHANGE);
        #endif // DATA_USE_CANARY

        stack1->capacity = stack1->capacity / CHANGE;

        if(CHANGE == 2)
            CHANGE = 3;
        else
            CHANGE = 2;
    }

    CHECKSTACK(ALL_OK)

    return pop_element;
}

