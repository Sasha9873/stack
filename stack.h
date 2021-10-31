#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

    #include <cstdio>
    #include <cassert>
    #include <cstdlib>
    #include <cstring>
    #include <cmath>

    #define CHECK 2

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

        size_t capacity = 1; //��������� ������ �������
        size_t current_size = 0;//������� ������ �������
        int *data = POINTER_13; //����� ������� data[capacity]

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

    /**
     * Creates(allocates memory) data(massif with numbers which we push and pop from stack). Check if someone have called this function
     * twice.
     *
     * @param [in] stack1 Pointer to stack
     *
     * @returns 0 if all is normal, NOT_MEMORY if we have not enough memory, TWO_CTOR if we have called this function twice.
     */
    int stack_ctor(Stack* stack1);

    /**
     * Puts a new value in the massif data if it is need allocates new memory. Changes(+ 1) current_size in stack, changes capacity
     * if we have allocated new memory.
     *
     * @param [in] stack1 Pointer to stack
     * @param [in] value Value we need to push in stack
     *
     * @returns 0 if all is normal, NOT_MEMORY if we have not enough memory.
     */
    int stack_push(Stack* stack1, int value);

    /**
     * Deletes last value in the massif data if it is need allocates new memory. Changes(- 1) current_size in stack, changes capacity
     * if we have allocated new memory.
     *
     * @param [in] stack1 Pointer to stack
     *
     * @returns 0 if all is normal, NOT_MEMORY if we have not enough memory.
     */
    int stack_pop(Stack* stack1);

    /**
     * Puts poison in the data, free data's memory. Puts poison in the pointer to data, in the current_size of data, in the capacity.
     *
     * @param [in] stack1 Pointer to stack
     *
     * @returns 0 if all is normal, STACK_POINTER if stack pointer already consists of poison.
     */
    int stack_dtor(Stack* stack1);

    /**
     * Checks all in stack.
     *
     * @param [in] stack1 Pointer to stack
     *
     * @returns 0 if all is normal, code of the error if something is wrong.
     */
    errors_t stack_ok(Stack* stack1);    //enum errors stack_ok

     /**
     * Prints all infornation about the stack in file_with_errors.
     *
     * @param [in] stack1 Pointer to stack
     *
     * @returns 0 if all is normal.
     */
    int stack_dump(Stack* stack1, errors_t reason = ALL_OK);

    /**
     * Counts massif data hash.
     *
     * @param [in] stack1 Pointer to stack
     *
     * @returns counted hash value.
     */
    int stack_hash(Stack* stack1, errors_t reason = ALL_OK);

#endif // STACK_H_INCLUDED
