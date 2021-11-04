#include "stack.h"

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
            stack1->data[stack1->capacity + 1] = CANARY_VALUE;
        #endif  // DATA_USE_CANARY

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
    int* new_memory = NULL;

    stack1->current_size++;

    #ifdef DATA_USE_CANARY
        if(stack1->current_size > stack1->capacity){

            if((new_memory = (int*)realloc(stack1->data, (stack1->current_size * stack1->push_change + 2) * sizeof(stack1->data[0]))) == NULL){
                CHECKSTACK(NOT_MEMORY);
                return NOT_MEMORY;
            }
            else{
                stack1->data = new_memory;

                *(stack1->data + stack1->current_size * stack1->push_change + 1) = *(stack1->data + stack1->current_size); //replace end canary

                stack1->capacity = stack1->current_size * stack1->push_change;

                if(stack1->push_change == 2)
                    stack1->pop_change = 3;
                else
                    stack1->pop_change = 2;
            }
        }
        *(stack1->data + stack1->current_size) = value;  //add new value in stack
    #else
        if(stack1->current_size > stack1->capacity){
            if((new_memory = (int*)realloc(stack1->data, stack1->current_size * stack1->push_change * sizeof(stack1->data[0]))) == NULL){
                CHECKSTACK(NOT_MEMORY);
                return NOT_MEMORY;
            }

            else{
                stack1->data = new_memory;

                stack1->capacity = stack1->current_size * stack1->push_change;

                if(stack1->push_change == 2)
                    stack1->pop_change = 3;
                else
                    stack1->pop_change = 2;

            }
        }

        *(stack1->data + stack1->current_size - 1) = value;   //add new value in stack

    #endif // DATA_USE_CANARY

    CHECKSTACK(ALL_OK);

    return EXIT_SUCCESS;
}

int stack_pop(Stack* stack1)
{
    CHECKSTACK(ALL_OK);

    int pop_element = POISON;

    #ifdef DATA_USE_CANARY
        pop_element = stack1->data[stack1->current_size];
        stack1->data[stack1->current_size] = POISON;
    #else
        pop_element = stack1->data[stack1->current_size - 1];
        stack1->data[stack1->current_size - 1] = POISON;
    #endif // DATA_USE_CANARY

    stack1->current_size--;

    if(stack1->current_size <= stack1->capacity / stack1->pop_change){
        #ifdef DATA_USE_CANARY
            stack1->data[stack1->capacity / stack1->pop_change + 1] = stack1->data[stack1->capacity + 1]; //end canary

            stack1->data = (int*)realloc(stack1->data, (stack1->capacity / stack1->pop_change + 2) * sizeof(stack1->data[0]));
        #else
            stack1->data = (int*)realloc(stack1->data, stack1->capacity / stack1->pop_change * sizeof(stack1->data[0]));
        #endif // DATA_USE_CANARY

        stack1->capacity = stack1->capacity / stack1->pop_change;

        if(stack1->pop_change == 2)
            stack1->push_change = 3;
        else
            stack1->push_change = 2;
    }

    CHECKSTACK(ALL_OK)

    return pop_element;
}

int stack_dtor(Stack* stack1)
{
    stack_ok(stack1);

    if(stack1->data == POINTER_13)
        return STACK_POINTER;

    #ifdef DATA_USE_CANARY
        memset(stack1->data, POISON, (stack1->capacity + 2) * sizeof(stack1->data[0]));
    #else
        memset(stack1->data, POISON, stack1->capacity * sizeof(stack1->data[0]));
    #endif // DATA_USE_CANARY


    free(stack1->data);

    stack1->data = POINTER_13;
    stack1->current_size = POISON;
    stack1->capacity = POISON;

    return 0;
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
        fprintf(stack1->file_with_errors, "ERROR %d %s", error, error_names[abs(error)]);

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

   return 0;
}

int stack_hash(Stack* stack1, errors_t reason)
{
    const int coeff = 13;
    long long Hash = 0, coeff_pow = 1;
    for (int index = 0; index < stack1->current_size; index++)
    {
        Hash += stack1->data[index] * coeff_pow;
        coeff_pow *= coeff;
    }
    return Hash;
}

