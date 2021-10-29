#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

    #include <cstdio>
    #include <cassert>
    #include <cstdlib>
    #include <cstring>
    #include <cmath>

    #define CHECK 0

    #if CHECK >= 2
        #define STACK_USE_HASH
        #define STACK_USE_CANARY
        #define DATA_USE_CANARY
    #elif CHECK >= 1
        #define STACK_USE_CANARY
        #define DATA_USE_CANARY
    #endif // CHECK

    #define CHECKSTACK(reason)\
        if(!stack_ok(stack1)){\
        fprintf(stack1->file_with_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
        stack_dump(stack1, reason);\
    }

    const int POISON = 666;
    int* POINTER_13 = (int*)13;
    int CHANGE = 2;
    const int CANARY_VALUE = 0xBADDED;
    int previous_hash_value;
    int hash_value;

    struct Stack
    {
        #ifdef STACK_USE_CANARY
        int begin_canary = CANARY_VALUE;
        #endif

        int *data = POINTER_13; //адрес массива data[capacity]
        size_t capacity = 0; //требуемый размер массива
        size_t current_size = 0;//текущий размер массива
        FILE* file_with_errors = NULL;

        #ifdef STACK_USE_CANARY
        int end_canary = CANARY_VALUE;
        #endif
    };

    char *error_names[] = {
        "All is ok",
        "Not enough memory",
        "Two ctors",
        "Not pointer to error",
        "Bad stack pointer",
        "Negative capacity",
        "Negative size",
        "Wrong begin stack canary",
        "Wrong end stack canary",
        "Wrong begin data canary",
        "Wrong end data canary",
        "Bad data pointer",
        "Capacity smaller then size",
        "Wrong hash"
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
                        DATA_POINTER      = -11,
                        CAP_SMALLER_SIZE  = -12,
                        WRONG_HASH        = -13}
    errors_t;

    int stack_ctor(Stack* stack1);
    int stack_push(Stack* stack1, int value);
    int stack_pop(Stack* stack1);
    int stack_dtor(Stack* stack1);
    errors_t stack_ok(Stack* stack1);    //enum errors stack_ok
    int stack_dump(Stack* stack1, errors_t reason = ALL_OK);
    int stack_hash(Stack* stack1, errors_t reason = ALL_OK);

#endif // STACK_H_INCLUDED
