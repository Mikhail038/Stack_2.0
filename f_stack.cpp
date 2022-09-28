#include "stack.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

//-DDEFINE_NAME

//TODO name in info structure


//TODO hash

static const int coef = 2;

enum EErrors 
{
    UnknownError     = 1,
    Capacity         = 2,
    StackNullPtr     = 4,
    SizeMoreCapacity = 8,
    CanariesAreDead  = 16,
    BirthFileNull    = 32,
    BirthFuncNull    = 64,
    SourceFileNull   = 128,
    SourceFuncNull   = 256
};

typedef struct
{
    EErrors  num;
    char name[60]; //TODO read about svariable array
} StructError;

static const StructError ArrStructErr[32] =
{
    {UnknownError, "Unknown Error"},
    {Capacity, "Capacity < 0"},
    {StackNullPtr, "Stack pointer is NULL"},
    {SizeMoreCapacity, "Stack size is more than capacity"},
    {CanariesAreDead, "Canaries are dead. Stack is corrupted"},
    {BirthFileNull, "No info about file where stack was declared"},
    {BirthFuncNull, "No info about function where stack was declared"},
    {SourceFileNull, "No info about file where check failed"},
    {SourceFuncNull, "No info about function where check failed"}
};

// unsigned int hash_h31 (const void* array, int size)
// {
//     unsigned int h = 0;

//     while (size) 
//     {
//         h = 31 * h + *((char*) array++);

//         --size;
//     }

//     return h;
// }

// unsigned int djb33_hash (const char* s, int len)
// {
//     unsigned int h = 5381;
//     while (len--) 
//     {
//         h = (h << 5) + h + *s++;
//     }
//     return h;
// }

unsigned int HashFAQ6 (const char* str)
{

    unsigned int hash = 0;

    for (; *str; str++)
    {
        hash += (unsigned char)(*str);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

bool stack_is_full (StructStack* stack)
{
    BGN;
    END;

    return !(stack->capacity * sizeof (stack->data) - stack->size * sizeof (stack->data) - stack->canary_size);
}

bool stack_is_empty (StructStack* stack)
{
    BGN;
    END;

    return !(stack->size);
}

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

int canaries_in_stack_alive (StructStack* stack)
{
    if (compare_byte_by_byte (&stack->front_canary, &stack->canary, stack->canary_size) == 0)
    {
        return 0;
    }

    if (compare_byte_by_byte (&stack->end_canary,   &stack->canary, stack->canary_size) == 0)
    {
        return 0;
    }

    return 1;
}

static int create_canary (StructStack* stack, int Size)
{
    BGN;

    srand (time (NULL));

    //stack->canary = (char*) calloc (Size, sizeof(*stack->canary));
    stack->canary_size = Size;

    for (int i = 0; i < stack->canary_size; i++)
    {
        ((char*) &stack->canary)[i] = rand() % 256;
    }

    END;

    return 0;
}

static int stack_variator (StructStack* stack)
{
    BGN;

    MSA (stack != NULL, 4);
    MSA (stack->capacity >= 0, 2);
    MSA (stack->size <= stack->capacity, 8);
    

    if (PROTECTION_LEVEL > 0) 
    {   
        MSA (canaries_in_stack_alive (stack), 16);
        MSA (stack->birth->file != NULL, 32);
        MSA (stack->birth->func != NULL, 64);
        MSA (stack->source->func != NULL, 128);
        MSA (stack->source->func != NULL, 256);

        if (PROTECTION_LEVEL > 1)
        {
            
        }
    }   

    MCA (stack->err == 0, 1);

    END;

    return 0;
}

int stack_dump (StructStack* stack)
{
    BGN;

    print_head (stack, stderr);

    for (int i = 0; i < sizeof (stack->err) * CHAR_BIT; i++)
    {
        if (stack->err & ((int) (ArrStructErr[i]).num))
        {
            print_error ((ArrStructErr[i]).name, stack->source, stderr);
        }
    }

    END;

    return 0;
}

int initialize_info (StructInfo* info, const char* File, const char* Function, int Line)
{
    BGN;

    MCA (info != NULL, 1);
    MCA (info->file != NULL, 1);


    info->file = strncpy (info->file, File, 50);
    info->func = strncpy (info->func, Function, 50);
    info->line = Line;

    END;

    return 0;
}

static int print_head (StructStack* stack, FILE* stream)
{
    BGN;

    fprintf (stream,
    "=============================================================================================\n"
    "=============================================================================================\n"
    "ERROR LOG\n"
    "=============================================================================================\n"
    "=============================================================================================\n"
    "BIRTH INFO\n"
    "---------------------------------------------------------------------------------------------\n"
    "%s %s\n"
    "---------------------------------------------------------------------------------------------\n"
    "FILE %s\n"
    "FUNCTION %s\n"
    "LINE %d\n"
    "=============================================================================================\n"
    "Stack\n"
    "---------------------------------------------------------------------------------------------\n"
    "front_canary '%lg'\n"
    "hash         '%d'\n"
    "size         '%d'\n"
    "capacity     '%d'\n"
    "data         '%p'\n"
    "canary       '%lg'\n"
    "canary_size  '%d'\n"
    "err          '%d'\n"
    "birth        '%p'\n"
    "source       '%p'\n"
    "end_canary   '%lg'\n"
    "---------------------------------------------------------------------------------------------\n",
    __DATE__, __TIME__, stack->birth->file, stack->birth->func, stack->birth->line,
    stack->front_canary, stack->hash, stack->size, stack->capacity,
    stack->data, stack->canary, stack->canary_size, stack->err, 
    stack->birth, stack->source, stack->end_canary);

    
    print_stack_data_double (stack);

    END;

    return 0;
}

static int print_error (const char* ErrorName, StructInfo* info, FILE* Stream)
{
    fprintf (Stream, "FAILED CHECK//IN FILE//%s//IN FUNCTION//%s//IN LINE//%-4d//"
    "ERROR:||%-50s||\n",
    info->file, info->func, info->line, ErrorName);

    return 0;
}

static int print_stack_data_double (StructStack* stack)
{
    BGN;

    stack->data -= (PROTECTION_LEVEL > 1) ? (int) ((double) stack->canary_size / (double) (sizeof (*stack->data))) : 0;

    for (int i = 0;  i < stack->capacity + ((PROTECTION_LEVEL > 1) ? 2 : 0); i++)
    {
        fprintf (stderr, "data[%d] '%lg'\n", i, (stack->data)[i], (stack->data)[i]);
    }

    fprintf (stderr, 
    "=============================================================================================\n");
    stack->data += (PROTECTION_LEVEL > 1) ? (int) ((double) stack->canary_size / (double) (sizeof (*stack->data))) : 0;

    END;

    return 0;
}

static int poison (void* Victim, int Size)
{
    BGN;

    for (int i = 0; i < Size; i++)
    {
        ((char*) Victim)[i] = 'z';
    }

    END;

    return 0;
}

static int poison_array (void* VictimArray, int ArraySize, int Size)
{
    BGN;

    for (int i = 0; i < ArraySize; i++)
    {
        poison (&(((char*) VictimArray)[i]), Size);
    }

    END;

    return 0;
}

static int make_stack_bigger (StructStack* stack)
{
    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);
        return 1;
    }

    stack->data = (StackDataType*) realloc (stack->data, (stack->capacity > 0) ? stack->capacity * coef * sizeof (*stack->data) : (stack->capacity + 1) * coef * sizeof (*stack->data) );

    stack->capacity = (stack->capacity > 0) ? stack->capacity * coef : (stack->capacity + 1) * coef;

    if (stack == NULL)
    {
        return 1;
    }

    return 0;
}

static int make_stack_bigger_with_canaries (StructStack* stack)
{
    BGN;
    
    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);
        return 1;
    }

    int OldCapasity = stack->capacity;

    stack->data -= (int) ((double) stack->canary_size / (double) (sizeof (*stack->data)));

    stack->data = (StackDataType*) realloc (stack->data, (stack->capacity > 0) ? 
                                            stack->capacity * coef * sizeof (*stack->data) + 2 * stack->canary_size :
                                            (stack->capacity + 1) * coef * sizeof (*stack->data) + 2 * stack->canary_size );

    move_canary (stack, OldCapasity);

    stack->capacity = (stack->capacity > 0) ? stack->capacity * coef : (stack->capacity + 1) * coef;

    stack->data += (int) ((double) stack->canary_size / (double) (sizeof (*stack->data)));

    if (stack == NULL)
    {
        END;

        return 1;
    }

    END;

    return 0;
}

static int make_stack_smaller (StructStack* stack)
{
    BGN;
    
    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);

        END;

        return 1;
    }

    //printf ("-------'%p' '%d' '%d' '%d'\n", stack->data, stack->capacity, sizeof (*stack->data), coef);
    stack->data = (StackDataType*) realloc (stack->data, sizeof (*stack->data) * ( stack->capacity / coef));
    //printf ("-------'%p' '%d' '%d' '%d'\n", stack->data, stack->capacity, sizeof (*stack->data), coef);


    stack->capacity /= coef;

    if (stack->data == NULL)
    {
        stack_dump (stack);

        END;

        return 1;
    }

    END;

    return 0;
}

static int make_stack_smaller_with_canaries (StructStack* stack)
{
    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);
        return 1;
    }

    stack->data -= (int) ((double) stack->canary_size / (double) (sizeof (*stack->data)));

    stack->data = (StackDataType*) realloc (stack->data, 2 * stack->canary_size +  sizeof (*stack->data) * ( stack->capacity / coef) );

    stack->capacity /= coef;

    stack->data += (int) ((double) stack->canary_size / (double) (sizeof (*stack->data)));

    copy_byte_by_byte (&stack->canary, stack->data + stack->capacity, stack->canary_size);

    if (stack->data == NULL)
    {
        stack_dump (stack);

        return 1;
    }

    return 0;
}

void move_canary (StructStack* stack, int OldCapasity)
{
    int i = 0;

    for (char* c = (char*) (OldCapasity * sizeof (*stack->data) + stack->data + stack->canary_size); i < stack->canary_size;)
    {
        c[i] = '\0';
        i++;
    }
    
    copy_byte_by_byte (&stack->canary, stack->data                  , stack->canary_size);
    stack->data += (int) ((double) stack->canary_size / (double) (sizeof (*stack->data)));
    copy_byte_by_byte (&stack->canary, stack->data + stack->capacity, stack->canary_size);
    stack->data -= (int) ((double) stack->canary_size / (double) (sizeof (*stack->data)));

   
    return;
}

int stack_constructor (StructStack* stack, int Capacity)
{
    BGN;

    MSA (Capacity >= 0, 2);
    MSA (stack != NULL, 4);
    MCA (stack != NULL, 1);

    stack->capacity = Capacity;

    if (PROTECTION_LEVEL > 0) 
    {
       create_canary (stack, sizeof (stack->canary));
    }
    
    stack->data = (StackDataType*) calloc (1, stack->capacity * sizeof (*stack->data) + 2 * stack->canary_size);

    //print_stack_data_double (stack);

    stack->source = (StructInfo*) calloc (1, sizeof (*stack->source));
    stack->birth  = (StructInfo*) calloc (1, sizeof (*stack->birth));

    copy_byte_by_byte (&stack->canary, &stack->front_canary, stack->canary_size);
    copy_byte_by_byte (&stack->canary, &stack->end_canary,   stack->canary_size);

    if (PROTECTION_LEVEL > 1) 
    {
        copy_byte_by_byte (&stack->canary, stack->data,                   stack->canary_size);
        stack->data += (int) ((double) stack->canary_size / (double) (sizeof (*stack->data)));
        //print_stack_data_double (stack);
        copy_byte_by_byte (&stack->canary, stack->data + stack->capacity, stack->canary_size);
        //print_stack_data_double (stack);
    }
    
    // stack->data = stack->data + stack->canary_size;

    stack->source->file = (char*) calloc (50, sizeof (*stack->source->file));
    stack->source->func = (char*) calloc (50, sizeof (*stack->source->func));

    stack->birth->file = (char*) calloc (50, sizeof (*stack->birth->file));
    stack->birth->func = (char*) calloc (50, sizeof (*stack->birth->func));


    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);

        END;

        return 1;

    }

    INIT (stack->source);

    END;

    return 0;
}

int push_in_stack (StructStack* stack, StackDataType x)
{
    BGN;

    INIT (stack->source);
    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);

        END;

        return 1;
    }

    if (stack_is_full (stack))
    {
        if (PROTECTION_LEVEL > 1 ? (make_stack_bigger_with_canaries (stack) != 0) : (make_stack_bigger (stack) != 0))
        {
            stack_dump (stack);

            END;

            return 1;
        }
    }

    //printf ("%d!!\n", stack->size);
    stack->data[stack->size] = x;
    stack->size++;

    END;

    return 0;
}

int peek_from_stack (StructStack* stack, StackDataType* x)
{
    BGN;

    INIT (stack->source);
    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);

        END;

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

    END;

    return 0;
}

int pop_from_stack (StructStack* stack, StackDataType* x)
{
    BGN;
    
    INIT (stack->source);
    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);

        END;

        return 1;
    }

    if (stack_is_empty (stack))
    {
        printf ("Stack is empty!\n");
    }
    else
    {
        *x = stack->data[stack->size - 1];

        poison (&(stack->data[stack->size - 1]), sizeof (*stack->data));

        stack->size--;
    }

    if ((stack->size <= 4 * stack->capacity) && (stack->size > 10))
    {
        if (PROTECTION_LEVEL > 1 ? (make_stack_smaller_with_canaries (stack) != 0) : (make_stack_smaller (stack) != 0))
        {
            fprintf (stderr, "Error %s %s %d !\n", LOCATION);
        }
    }
    
    END;

    return 0;
}

int stack_destructor (StructStack* stack)
{
    BGN;

    //TODO delete it
    INIT (stack->source);
    if (stack_variator (stack) == 0)
    {
        //stack->err = 1 + 2 + 8 + 16 + 32 + 64 + 128 + 256;
        stack_dump (stack);
        if (stack->err & 4)
        {
            return 1;
        }
    }
    //stack->data = stack->data - stack->canary_size;

    // if (stack->canary != NULL)
    // {
    //     poison_array (stack->canary, stack->canary_size, sizeof (*stack->canary));
    //     free (stack->canary);
    // }

    //TODO delete &

    stack->data -= (PROTECTION_LEVEL > 1) ? (int) ((double) stack->canary_size / (double) (sizeof (*stack->data))) : 0;

    if (stack->data != NULL)
    {
        poison_array (stack->data, stack->capacity, sizeof (*stack->data));
        free (stack->data);
        //poison (&stack->data, sizeof (stack->data));
    }

    if (stack->source->file != NULL)
    {
        poison_array (stack->source->file, 50, sizeof(*stack->source->file));
        free (stack->source->file);
        poison (&(stack->source->file), sizeof (stack->source->file));
    }
    if (stack->source->func != NULL)
    {
        poison_array (stack->source->func, 50, sizeof(*stack->source->func));
        free (stack->source->func);
        poison (&(stack->source->func), sizeof (stack->source->func));
        free (stack->source);
    }

    if (stack->birth->file != NULL)
    {
        poison_array (stack->birth->file, 50, sizeof(*stack->birth->file));
        free (stack->birth->file);
        poison (&(stack->birth->file), sizeof (stack->birth->file));
    }
    if (stack->birth->func != NULL)
    {
        poison_array (stack->birth->func, 50, sizeof(*stack->birth->func));
        free (stack->birth->func);
        poison (&(stack->birth->func), sizeof (stack->birth->func));
        free (stack->birth);
    }

    // poison (&(stack->birth), sizeof (stack->birth));
    // poison (&(stack->source), sizeof (stack->source));
    // poison (&(stack->canary_size), sizeof (stack->canary_size));
    // poison (&(stack->canary), sizeof (stack->canary));
    // poison (&(stack->capacity), sizeof (stack->capacity));
    // poison (&(stack->size), sizeof (stack->size));
    // poison (&(stack->err), sizeof (stack->err));

    poison (stack, sizeof (*stack));

    stack = NULL;

    END;

    return 0;
}
