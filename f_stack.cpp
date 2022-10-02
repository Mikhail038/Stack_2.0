#include "stack.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

static const int coef = 2;

enum EErrors 
{
    UnknownError            = 1,
    Capacity                = 2,
    StackNullPtr            = 4,
    SizeMoreCapacity        = 8,
    StackFrontCanaryIsDead  = 16,
    StackEndCanaryIsDead    = 32, //TODO vyronyai
    BirthFileNull           = 64,  
    BirthFuncNull           = 128,
    SourceFileNull          = 256,
    SourceFuncNull          = 512,
    DataFrontCanaryIsDead   = 1024,
    DataEndCanaryIsDead     = 2048,
    WrongStackHash          = 4096,
    WrongDataHash           = 8192   
};

typedef struct
{
    EErrors  num;
    char name[60]; //TODO read about svariable array
} StructError;

static const StructError ArrStructErr[32] =
{
    {UnknownError,           "Unknown Error"},
    {Capacity,               "Capacity < 0"},
    {StackNullPtr,           "Stack pointer is NULL"},
    {SizeMoreCapacity,       "Stack size is more than capacity"},
    {StackFrontCanaryIsDead, "Front canary is dead. Stack is corrupted"},
    {StackEndCanaryIsDead,   "End canary is dead. Stack is corrupted"},
    {BirthFileNull,          "No info about file where stack was declared"},
    {BirthFuncNull,          "No info about function where stack was declared"},
    {SourceFileNull,         "No info about file where check failed"},
    {SourceFuncNull,         "No info about function where check failed"},
    {DataFrontCanaryIsDead,  "Front canary is dead. Stack data is corrupted"},
    {DataEndCanaryIsDead,    "End canary is dead. Stack data is corrupted"},
    {WrongStackHash,         "Hash-check failed, stack is corrupted"},
    {WrongDataHash,          "Hash-check failed, data is corrupted"}
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

unsigned int HashFAQ6 (const char* Str, int Size)
{

    unsigned int hash = 0;

    for (; Size > 0; Str++)
    {
        hash += (unsigned char)(*Str);
        hash += (hash << 10);
        hash ^= (hash >> 6);

        Size--;
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

int check_stack_front_canary (StructStack* stack)
{
     if (compare_byte_by_byte (&stack->front_canary, &stack->canary, stack->canary_size) == 0)
    {
        return 0;
    }

    return 1;   
}

int check_stack_end_canary (StructStack* stack)
{
     if (compare_byte_by_byte (&stack->end_canary, &stack->canary, stack->canary_size) == 0)
    {
        return 0;
    }

    return 1;   
}

int check_data_front_canary (StructStack* stack)
{
    stack->data = (double*) stack->data -  1/**/;
    
    if (compare_byte_by_byte (stack->data, &stack->canary, stack->canary_size) == 0)
    {
        stack->data = (double*) stack->data +  1/**/;
        
        return 0;
    }
    
    stack->data = (double*) stack->data +  1/**/;

    return 1;   
}

int check_data_end_canary (StructStack* stack)
{
    stack->data = (double*) stack->data -  1/**/;
    
    if (compare_byte_by_byte (stack->data + stack->capacity + 1, &stack->canary, stack->canary_size) == 0)
    {
        stack->data = (double*) stack->data +  1/**/;
        
        return 0;
    }
    stack->data = (double*) stack->data +  1/**/;

    return 1;   
}

int check_data_hash (StructStack* stack)
{
    BGN;
    
    stack->data = (double*) stack->data -  1/**/;

    //TODO privesty k ukazately na kanareiky

    unsigned int old_hash = stack->hash_data;

    char* h_data = (char*) stack->data;

    unsigned int data_hash  = HashFAQ6 (h_data, stack->capacity * sizeof (*stack->data));

    stack->data = (double*) stack->data +  1/**/;

    stack->hash_data = data_hash;

    if (old_hash == stack->hash_data)  
    {  
        if (PROTECTION_LEVEL > 3) fprintf (stderr, "1 '%u' '%u'\n", old_hash, stack->hash_data); // TODO add global FILE* logfile; 
        
        END;

        return 1;
    }

    if (PROTECTION_LEVEL > 3) fprintf (stderr, "0 '%u' '%u'\n", old_hash, stack->hash_data);

    END;

    return 0;
}

int check_stack_hash (StructStack* stack)
{
    BGN;

    stack->data = (double*) stack->data -  1/**/;
    
    //TODO VitualQuery prava pamyati

    unsigned int old_hash =  stack->hash;

    stack->hash = 0;

    char* h_stack = (char*) stack;

    unsigned int hash_stack = HashFAQ6 (h_stack, sizeof (*stack));

    stack->data = (double*) stack->data +  1/**/;

    stack->hash = hash_stack;

    if (old_hash == hash_stack)
    {  
        if (PROTECTION_LEVEL > 3) fprintf (stderr, "1 '%u' '%u'\n", old_hash, hash_stack);
        
        END;

        return 1;
    }

    if (PROTECTION_LEVEL > 3) fprintf (stderr, "0 '%u' '%u'\n", old_hash, hash_stack);

    END;

    return 0;
}

int make_hash (StructStack* stack)
{
    BGN;

    if (PROTECTION_LEVEL < 3)
    {
        END;

        return 0;
    }

    stack->data = (double*) stack->data -  1/**/;
    //(int) ((double) stack->canary_size / (double) (sizeof (*stack->data)))
    stack->hash = 0;
   //(double*) stack->data -= 


    char* h_stack = (char*) stack;
    char* h_data = (char*) stack->data;
    
    unsigned int hash_data  = HashFAQ6 (h_data, stack->capacity * sizeof (*stack->data));    
    stack->hash_data = hash_data; 
    
    unsigned int hash_stack = HashFAQ6 (h_stack, sizeof (*stack));
    stack->hash = hash_stack; 

    stack->data = (double*) stack->data +  1/**/;

    if (PROTECTION_LEVEL > 3) fprintf (stderr, "n %u n %u\n", stack->hash, stack->hash_data);
    
    END;

    return 0;
}

static void create_canary (StructStack* stack)
{
    BGN;

    srand (time (NULL));

    //stack->canary = (char*) calloc (Size, sizeof(*stack->canary));
    stack->canary_size = sizeof (double);

    for (int i = 0; i < stack->canary_size; i++)
    {
        ((char*) &stack->canary)[i] = rand() % 256;
    }

    END;

    return;
}

static int stack_variator (StructStack* stack)
{
    BGN;

    MSA (stack != NULL, 4);
    MSA (stack->capacity >= 0, 2);
    MSA (stack->size <= stack->capacity, 8);
    

    if (PROTECTION_LEVEL > 0) 
    {   
        MSA (check_stack_front_canary (stack), 16);
        MSA (check_stack_end_canary (stack), 32);
        MSA (stack->birth->file != NULL, 64);
        MSA (stack->birth->func != NULL, 128);
        MSA (stack->source->func != NULL, 256);
        MSA (stack->source->func != NULL, 512);

        if (PROTECTION_LEVEL > 1)
        {
            MSA (check_data_front_canary (stack), 1024);
            MSA (check_data_end_canary (stack), 2048);

            if (PROTECTION_LEVEL > 2)
            {
                MSA (check_stack_hash (stack), 4096);
                MSA (check_data_hash (stack), 8192);                
            }
        }
    }   

    MCA (stack->err == 0, 1);

    END;

    return 0;
}

void stack_dump (StructStack* stack)
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

    return;
}

int initialize_info (StructInfo* info, const char* File, const char* Function, int Line, const char* Name)
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

static void print_head (StructStack* stack, FILE* stream)
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
    "NAME %s\n"
    "FILE %s\n"
    "FUNCTION %s\n"
    "LINE %d\n"
    "=============================================================================================\n"
    "front_canary '%lg'\n"
    "hash         '%u'\n"
    "hash_data    '%u'\n"
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
    __DATE__, __TIME__, stack->birth->name, stack->birth->file, stack->birth->func, stack->birth->line, 
    stack->front_canary, stack->hash, stack->hash_data, stack->size, stack->capacity,
    stack->data, stack->canary, stack->canary_size, stack->err, 
    stack->birth, stack->source, stack->end_canary);

    
    print_stack_data_double (stack);

    END;

    return;
}

static void print_error (const char* ErrorName, StructInfo* info, FILE* Stream)
{
    fprintf (Stream, "FAILED CHECK!  IN FILE %s  IN FUNCTION %s  IN LINE %-4d  ERROR:||%-50s||\n",
    info->file, info->func, info->line, ErrorName);

    return;
}
// TODO void print_el (int a) { printf("%d", a);}
// void print_el (double a) { printf("%lg", a);}
// void print_el (char a) { printf("%c", a);}

static void print_stack_data_double (StructStack* stack) // print_el (int) // print_el(char) // print_el(double) //  
{
    BGN;

    if (PROTECTION_LEVEL > 1) stack->data = (double*) stack->data - 1;

    for (int i = 0;  i < stack->capacity + ((PROTECTION_LEVEL > 1) ? 2 : 0); i++)
    {
        fprintf (stderr, "data[%3d] '%lg'\n", i, (stack->data)[i], (stack->data)[i]);
        //fprintf (stderr, "data[!!!] '%x'\n", (stack->data)[i]);
        //print_el((stack->data)[i]);

        for (int j = 0; j < sizeof ((stack->data)[i]); j++)
            fprintf (stderr, "[%x]", ((char*)&(stack->data)[i])[j] ); //TODO osoznai
        fprintf (stderr, "\n");

    }


    

    fprintf (stderr, 
    "=============================================================================================\n");

    if (PROTECTION_LEVEL > 1) stack->data = (double*) stack->data +  1;


    END;

    return;
}

static void poison (void* Victim, int Size)
{
    BGN;

    for (int i = 0; i < Size; i++)
    {
        ((char*) Victim)[i] = 'z';
    }

    END;

    return;
}

static void poison_array (void* VictimArray, int ArraySize, int Size)
{
    BGN;

    for (int i = 0; i < ArraySize; i++)
    {
        poison (&(((char*) VictimArray)[i]), Size);
    }

    END;

    return;
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

    stack->data = (double*) stack->data -  1/**/;

    stack->data = (StackDataType*) realloc (stack->data, (stack->capacity > 0) ? 
                                            stack->capacity * coef * sizeof (*stack->data) + 2 * stack->canary_size :
                                            (stack->capacity + 1) * coef * sizeof (*stack->data) + 2 * stack->canary_size );

    move_canary (stack, OldCapasity);

    stack->capacity = (stack->capacity > 0) ? stack->capacity * coef : (stack->capacity + 1) * coef;

    stack->data = (double*) stack->data +  1/**/;

    if (stack == NULL)
    {
        END;

        return 1;
    }

    make_hash (stack);

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
    BGN;

    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);
        return 1;
    }

    stack->data = (double*) stack->data -  1/**/;

    stack->data = (StackDataType*) realloc (stack->data, 2 * stack->canary_size +  sizeof (*stack->data) * ( stack->capacity / coef) );

    stack->capacity /= coef;

    stack->data = (double*) stack->data +  1/**/;

    copy_byte_by_byte (&stack->canary, stack->data + stack->capacity, stack->canary_size);

    if (stack->data == NULL)
    {
        stack_dump (stack);

        return 1;
    }

    make_hash (stack);

    END;

    return 0;
}

void move_canary (StructStack* stack, int OldCapasity)
{
    BGN;

    int i = 0;

    for (char* c = (char*) (OldCapasity * sizeof (*stack->data) + stack->data + stack->canary_size); i < stack->canary_size;)
    {
        c[i] = '\0';
        i++;
    }
    
    copy_byte_by_byte (&stack->canary, stack->data                  , stack->canary_size);
    stack->data = (double*) stack->data +  1/**/;
    copy_byte_by_byte (&stack->canary, stack->data + stack->capacity, stack->canary_size);
    stack->data = (double*) stack->data -  1/**/;

    make_hash (stack);

    END;

    return;
}

int stack_constructor (StructStack* stack, int Capacity, const char* Name)
{
    BGN;

    MSA (Capacity >= 0, 2);
    MSA (stack != NULL, 4);
    MCA (stack != NULL, 1);

    stack->capacity = Capacity;

    if (PROTECTION_LEVEL > 0) 
    {
       create_canary (stack);
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
        stack->data = (double*) stack->data +  1/**/;
        //print_stack_data_double (stack);
        copy_byte_by_byte (&stack->canary, stack->data + stack->capacity, stack->canary_size);
        //print_stack_data_double (stack);
    }
    
    // stack->data = stack->data + stack->canary_size;

    stack->source->file = (char*) calloc (50, sizeof (*stack->source->file));
    stack->source->func = (char*) calloc (50, sizeof (*stack->source->func));
    stack->source->name = (char*) calloc (50, sizeof (*stack->source->name));
    stack->source->name = strncpy (stack->source->name, Name, 50);
    
    stack->birth->file = (char*) calloc (50, sizeof (*stack->birth->file));
    stack->birth->func = (char*) calloc (50, sizeof (*stack->birth->func));
    stack->birth->name = (char*) calloc (50, sizeof (*stack->birth->name));
    stack->birth->name = strncpy (stack->birth->name, Name, 50);

    make_hash (stack);

    if (stack_variator (stack) != 0)
    {
        stack_dump (stack);

        END;

        return 1;

    }

    INIT (stack->source);

    make_hash (stack);

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

    make_hash (stack);

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

    make_hash (stack);

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

    make_hash (stack);

    if ((stack->size <= 4 * stack->capacity) && (stack->capacity > 10))
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

    INIT (stack->source);
    if ((stack_variator (stack) != 0) || (PROTECTION_LEVEL > 3))
    {
        stack_dump (stack);
        if (stack->err & 4)
        {
            return 1;
        }
    }
    // stack->data = stack->data - stack->canary_size;

    // if (stack->canary != NULL)
    // {
    //     poison_array (stack->canary, stack->canary_size, sizeof (*stack->canary));
    //     free (stack->canary);
    // }

    //TODO delete &

    //(double*) stack->data -= (PROTECTION_LEVEL > 1) ? 1/**/ : 0;
    if (PROTECTION_LEVEL > 1) stack->data = (double*) stack->data -  1;

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
    }
    if (stack->source->name != NULL)
    {
        poison_array (stack->source->name, 50, sizeof(*stack->source->name));
        free (stack->source->name);
        poison (&(stack->source->name), sizeof (stack->source->name));
    }
    free (stack->source);

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
    }
    if (stack->birth->name != NULL)
    {
        poison_array (stack->birth->name, 50, sizeof(*stack->birth->name));
        free (stack->birth->name);
        poison (&(stack->birth->name), sizeof (stack->birth->name));
    }
    free (stack->birth);

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
