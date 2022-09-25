#include "stack.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


//TODO hash

bool stack_is_full (StructStack* stack)
{
    return !(stack->capacity * sizeof (stack->data) - stack->size * sizeof (stack->data) - stack->canary_size);
}

bool stack_is_empty (StructStack* stack)
{
    return !(stack->size);
}

// void swap_byte_by_byte (void* FirstData, void* SecondData, int Size)
// {
//     char buf = '\0';
//
//     for (int i = 0; i < Size; i++)
//     {
//         buf = ((char*) FirstData)[i];
//         ((char*) FirstData)[i] = ((char*) SecondData)[i];
//         ((char*) SecondData)[i] = buf;
//     }
//
//     return;
// }

void copy_byte_by_byte (void* FirstData, void* SecondData, int Size)
{
    for (int i = 0; i < Size; i++)
    {
        ((char*) SecondData)[i] = ((char*) FirstData)[i];
    }

    return;
}

int compare_byte_by_byte (void* FirstData, void* SecondData, int Size)
{
    for (int i = 0; i < Size; i++)
    {
        if (((char*) SecondData)[i] != ((char*) FirstData)[i])
        {
            return 0;
        }
    }

    return 1;
}

int canaries_alive (StructStack* stack)
{
    if (compare_byte_by_byte (stack->data - stack->canary_size, stack->canary, stack->canary_size) == 0)
    {
        return 0;
    }

    if (compare_byte_by_byte (stack->data + stack->capacity,    stack->canary, stack->canary_size) == 0)
    {
        return 0;
    }

    return 1;
}

static int create_canary (StructStack* stack, int Size)
{
    srand (time (NULL));

    stack->canary = (char*) calloc (Size, sizeof(*stack->canary));
    stack->canary_size = Size;

    for (int i = 0; i < Size; i++)
    {
        stack->canary[i] = rand() % 256;
    }

    return 0;
}

static int stack_variator (StructStack* stack)
{
    MSA (stack != NULL, 4);
    MSA (stack->capacity >= 0, 2);
    MSA (stack->size <= stack->capacity, 8);
    //MSA (canaries_alive (stack), 16);
    MSA (stack->birth.file != NULL, 32);
    MSA (stack->birth.func != NULL, 64);
    MSA (stack->source.func != NULL, 128);
    MSA (stack->source.func != NULL, 256);

    MCA (stack->err == 0, 1);

    return 0;
}

int stack_dump (StructStack* stack)
{
    print_head (stack, stderr);
    for (int i = 1; i < sizeof (stack->err) * 256; i *= 2)
    {
        if (stack->err & i)
        {
            print_error (stack, i, stderr);
        }
    }

    return 0;
}

int initialize_info (StructInfo* info, const char* File, int Line, const char* Function)
{
    info->file = strncpy (info->file, File, 50);
    info->func = strncpy (info->func, Function, 50);
    info->line = Line;
    printf ("%d", Line);

    return 0;
}

static int print_head (StructStack* stack, FILE* stream)
{
    fprintf (stream,
    "================================================\n"
    "================================================\n"
    "ERROR LOG\n"
    "================================================\n"
    "================================================\n"
    "Stack\n"
    "capacity %d\n"
    "size %d\n", stack->capacity, stack->size);

    fprintf (stream,
    "================================================\n"
    "BIRTH INFO\n"
    "================================================\n"
    "FILE %s\n"
    "FUNCTION %s\n"
    "LINE %d\n"
    "================================================\n",
    stack->birth.file, stack->birth.func, stack->birth.line);

    return 0;
}

static int print_error (StructStack* stack, int ErrorNumber, FILE* Stream)
{
    fprintf (Stream, "FAILED CHECK//""ERROR CODE %d"
    "//IN FUNCTION//%s//IN LINE//%d//IN FILE//%s//\n",
    ErrorNumber, stack->source.func, stack->source.file, stack->source.file);

    return 0;
}

static int poison (void* Victim, int Size) //TODO static
{
    for (int i = 0; i < Size; i++)
    {
        ((char*) Victim)[i] = 'z';
    }

    return 0;
}

static int poison_array (void* VictimArray, int ArraySize, int Size)
{
    for (int i = 0; i < ArraySize; i++)
    {
        poison (&(((char*) VictimArray)[i]), Size);
    }
    return 0;
}

static int make_stack_bigger (StructStack* stack)
{
    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);
        return 1;
    }

    stack->data = stack->data - stack->canary_size;

    stack->data = (StackDataType*) realloc (stack->data, stack->capacity * 2);

    //move_canary (stack);

    if (stack == NULL)
    {
        return 1;
    }

    return 0;
}

int stack_constructor (StructStack* stack, int Capacity)
{
    MSA (Capacity >= 0, 2);
    MSA (stack != NULL, 4);
    MCA (Capacity >= 0, 1);
    MCA (stack != NULL, 1);

    stack->capacity = Capacity;

    //create_canary (stack, 12);

    stack->data = (StackDataType*) calloc (1, stack->capacity * sizeof (*stack->data) );//+ 2 * stack->canary_size);

    //copy_byte_by_byte (stack->canary, stack->data,                                           stack->canary_size);
    //copy_byte_by_byte (stack->canary, stack->data + stack->capacity * sizeof (*stack->data), stack->canary_size);

    //stack->data = stack->data + stack->canary_size;
    stack->size = 0;

    stack->source.file = (char*) calloc (50, sizeof (*stack->source.file));
    stack->source.func = (char*) calloc (50, sizeof (*stack->source.func));

    stack->birth.file = (char*) calloc (50, sizeof (*stack->birth.file));
    stack->birth.func = (char*) calloc (50, sizeof (*stack->birth.func));

    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);
        return 1;
    }
    //initialize_source_info (&(stack->source), __FILE__, __LINE__, __PRETTY_FUNCTION__); //DO macros to all this func
    INIT (&(stack->source));

    return 0;
}

int push_in_stack (StructStack* stack, StackDataType x)
{
    INIT (&(stack->source));
    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);
        return 1;
    }

    if (stack_is_full (stack))
    {
        if (make_stack_bigger (stack) != 0)
        {
            stack_dump (stack);
            return 1;
        }
    }

    stack->size++;
    stack->data[stack->size - 1] = x;

    return 0;
}

int peek_from_stack (StructStack* stack, StackDataType* x)
{
    INIT (&(stack->source));
    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);
        return 1;
    }

    if (stack_is_empty (stack))
    {
        printf ("Stack is empty!\n");
    }
    else
    {
        *x = stack->data[stack->size - 1];
    }

    return 0;
}

int pop_from_stack (StructStack* stack, StackDataType* x)
{
    INIT (&(stack->source));
    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);
        return 1;
    }

    if (stack_is_empty (stack))
    {
        printf ("Stack is empty!\n");
    }
    else
    {
        *x = stack->data[stack->size - 1];

        poison (&(stack->data[stack->size]), sizeof (*stack->data));

        stack->size--;
    }

    return 0;
}

int stack_destructor (StructStack* stack)
{
    INIT (&(stack->source));
    if (stack_variator (stack) == 0)
    {
        stack->err = 1 + 2 + 8 + 16 + 32 + 64 + 128 + 256;
        stack_dump (stack);
        if (stack->err & 4)
        {
            return 1;
        }
    }
    //stack->data = stack->data - stack->canary_size;

    if (stack->data != NULL)
    {
        poison_array (stack->data, stack->capacity, sizeof (*stack->data));
    }

    poison (&(stack->capacity), sizeof (stack->capacity));
    poison (&(stack->size), sizeof (stack->size));
    poison (&(stack->err), sizeof (stack->err));

    if (stack->canary != NULL)
    {
        poison_array (stack->canary, stack->canary_size, sizeof (*stack->canary));
    }

    if (stack->birth.file != NULL)
    {
        poison_array (stack->birth.file, 50, sizeof(*stack->birth.file));
    }
    if (stack->birth.func != NULL)
    {
        poison_array (stack->birth.func, 50, sizeof(*stack->birth.func));
    }
    if (stack->source.file != NULL)
    {
        poison_array (stack->source.file, 50, sizeof(*stack->source.file));
    }
    if (stack->source.func != NULL)
    {
        poison_array (stack->source.func, 50, sizeof(*stack->source.func));
    }

    //free (stack->canary);

    if (stack->data != NULL)
    {
        free (stack->data);
        poison (&stack->data, sizeof (stack->data));
    }

    if (stack->source.file != NULL)
    {
        free (stack->source.file);
        poison (&(stack->source.file), sizeof (stack->source.file));
    }
    if (stack->source.func != NULL)
    {
        free (stack->source.func);
        poison (&(stack->source.func), sizeof (stack->source.func));
    }


    if (stack->birth.file != NULL)
    {
        free (stack->birth.file);
        poison (&(stack->birth.file), sizeof (stack->birth.file));
    }
    if (stack->birth.func != NULL)
    {
        free (stack->birth.func);
        poison (&(stack->birth.func), sizeof (stack->birth.func));
    }

    poison (&(stack->birth), sizeof (stack->birth));
    poison (&(stack->source), sizeof (stack->source));
    poison (&(stack->canary_size), sizeof (stack->canary_size));
    poison (&(stack->canary), sizeof (stack->canary));

    stack->canary = NULL;
    stack->data = NULL;
    stack = NULL;

    return 0;
}
