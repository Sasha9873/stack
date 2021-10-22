#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>

#define CHECKSTACK()\
 if(!stack_ok(Stack* stack1)){\
    fprintf(file_with_errors, "Error: in file %s in function %s in %d line,  \n", __FILE__, __FUNCTION__, __LINE__);\
    stack_dump(Stack* stack1, FILE* file_with_errors);\
 }
//file_print(Stack* stack1, FILE* file_with_errors)

const int POISON = 666;
int* POINTER_13 = (int*)13;
int CHANGE = 2;
const int CANARY_VALUE = -666;

struct Stack
{
    #ifdef STACK_USE_CANARY
    int begin_canary = CANARY_VALUE;
    #endif

    int *data = POINTER_13; //адрес массива data[capacity]
    size_t capacity = 0; //требуемый размер массива
    size_t current_size = 0;//текущий размер массива
    int error = 0;

    #ifdef STACK_USE_CANARY
    int end_canary = CANARY_VALUE;
    #endif
};

typedef enum errors{ALL_OK            = 0,
                    NOT_MEMORY        = -1,
                    TWO_CTOR          = -2,
                    NOT_POINT_ERROR   = -3,
                    STACK_POINTER     = -4,
                    NEGATIVE_CAPASITY = -5,
                    NEGATIVE_SIZE     = -6,
                    BEGIN_CANARY      = -7,
                    END_CANARY        = -8,
                    DATA_BEGIN_CANARY = -9,
                    DATA_END_CANARY   = -10,
                    DATA_POINTER      = -11}
errors_t;

int stack_ctor(Stack* stack1);
int stack_push(Stack* stack1, int value);
int stack_pop(Stack* stack1, int* error = NULL);
int stack_dtor(Stack* stack1);
errors_t stack_ok(Stack* stack1);    //enum errors stack_ok
int stack_dump(Stack* stack1, FILE* file_with_errors);

int main()
{
    int is_error;
    Stack st = {};

    FILE* file_with_errors = fopen("file_with_errors", "a+");


    if((is_error = stack_ctor(&st)) == NOT_MEMORY){
        printf("You have a problem in constructor: you have not enough memory!\n");
        return NOT_MEMORY;
    }
    else if(is_error == TWO_CTOR){
        printf("You called the stack_ctor function twice!\n");
        return NOT_MEMORY;
    }


    if((is_error = stack_push(&st,10)) == NOT_MEMORY){
        printf("You have a problem in constructor: you have not enough memory!\n");
        return NOT_MEMORY;
    }

    if((is_error = stack_push(&st,80)) == NOT_MEMORY){
        printf("You have a problem in push: you have not enough memory!\n");
        return NOT_MEMORY;
    }

    if((is_error = stack_push(&st,7)) == NOT_MEMORY){
        printf("You have a problem in push: you have not enough memory!\n");
        return NOT_MEMORY;
    }

    printf("%d\n", st.current_size);
    for(size_t i = 0; i < st.current_size; i++)
        printf("%d:  %d  ",i, *(st.data + i));
    printf("\n");

    int pop_element;
    if((pop_element = stack_pop(&st, &is_error)) == NOT_POINT_ERROR){
        printf("You have a problem: you have not give a pointer to errors\n");
        return NOT_POINT_ERROR;
    }
    printf("pop = %d\n", pop_element);
    for(size_t i = 0; i < st.current_size + 1; i++)
        printf("%d  ",*(st.data + i));
    printf("\n");

    if(stack_dtor(&st) == STACK_POINTER){
        printf("Pointer ro stack consists of poison");
        return STACK_POINTER;
    }

    printf("st = %d", st.current_size);

    printf("\n");
}


int stack_ctor(Stack* stack1)
{
    if(stack1->data == POINTER_13){
        stack1->data = (int*)calloc(stack1->capacity + 2, sizeof(int));
        if(stack1->data == NULL)
            return NOT_MEMORY;
        stack1->data[0] = CANARY_VALUE;
        stack1->data[1] = CANARY_VALUE;
        printf("can1 = %d can2 = %d\n", stack1->data[0], stack1->data[1]);
        stack1->current_size = 2;
    }
    else
        return TWO_CTOR;

    return 0;
}


int stack_push(Stack* stack1, int value)
{
    printf("stok = %d can2 = %d cur size = %d data = %p, %p\n", stack_ok(stack1), stack1->data[stack1->current_size - 1], stack1->current_size), stack1->data, POINTER_13;

    int* new_memory = NULL;
    stack1->current_size++;

    if(stack1->current_size > stack1->capacity){
        if((new_memory = (int*)realloc(stack1->data, stack1->current_size * CHANGE)) == NULL)
            return NOT_MEMORY;
        else{
            stack1->data = new_memory;
            stack1->capacity = stack1->current_size * CHANGE;
            printf("cap = %d\n", stack1->current_size);

            if(CHANGE == 2)
                CHANGE = 3;
            else
                CHANGE = 2;
        }
    }

    *(stack1->data + stack1->current_size - 1) = *(stack1->data + stack1->current_size - 2);

    *(stack1->data + stack1->current_size - 2) = value;

    printf("%d curs = %d can2= %d\n",*(stack1->data + stack1->current_size - 2), stack1->current_size, *(stack1->data + stack1->current_size - 1));

    return EXIT_SUCCESS;
}

int stack_pop(Stack* stack1, int* error)
{
    stack_ok(stack1);

    int pop_element = POISON;
    pop_element = stack1->data[stack1->current_size - 2];

    stack1->data[stack1->current_size - 2] = POISON;
    printf("cur_size = %d  pop(pop) = %d\n", stack1->current_size, stack1->data[stack1->current_size - 2]);
    stack1->current_size--;
    if(stack1->current_size <= stack1->capacity * CHANGE){
        stack1->data = (int*)realloc(stack1->data, stack1->current_size * CHANGE);
        if(CHANGE == 2)
            CHANGE = 3;
        else
            CHANGE = 2;
    }

    return pop_element;
}

int stack_dtor(Stack* stack1)
{
    stack_ok(stack1);

    if(stack1->data == POINTER_13)
        return STACK_POINTER;

    memset(stack1->data, POISON, stack1->capacity*sizeof(stack1->data[0]));

    free(stack1->data);

    stack1->data = POINTER_13;
    stack1->current_size = POISON;

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

    if(stack1->data[0] != CANARY_VALUE)
        return DATA_BEGIN_CANARY;

    if(stack1->data[stack1->current_size - 1] != CANARY_VALUE)
        return DATA_END_CANARY;

    if(stack1->capacity < 0)
        return NEGATIVE_CAPASITY;

        printf(",m,m = %d\n", stack1->current_size < 0);
    if(stack1->current_size < 0)
        return NEGATIVE_SIZE;

    if(stack1->data == POINTER_13)
        return DATA_POINTER;

    return ALL_OK;

}

int stack_dump(Stack* stack1, FILE* file_with_errors)
{
   //fprintf(file_with_symbols, "Error: in file %s in function %s in %d line,  \n", __FILE__, __FUNCTION__, __LINE__);
}
