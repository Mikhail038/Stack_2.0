#ifndef STACK_H
#define STACK_H

#include <stdio.h>

//---------------------------------------------------------------------------------------
// 1   - error
// 2   - Capacity < 0
// 4   - stack null ptr
// 8   - size > capacity
// 16  - birds are dead
// 32  - birth file null
// 64  - birth func null
// 128 - source file null
// 256 - source func null
//---------------------------------------------------------------------------------------

#define LOCATION __FILE__,__PRETTY_FUNCTION__,__LINE__

#define DOTXT(Message) #Message

#define BGN                                                                \
        do                                                                 \
        {                                                                  \
            fprintf (stderr, "I am alive! %-80s|\n", __PRETTY_FUNCTION__); \
        } while (0)

#define END                                                                 \
        do                                                                  \
        {                                                                   \
            fprintf (stderr, "I am dead!  %-80s^ \n", __PRETTY_FUNCTION__); \
        } while (0)


#ifndef PURPLE

#define BGN
#define END

#endif


#define MCP(Message)                                                                                       \
        do                                                                                                 \
        {                                                                                                  \
            fprintf (stderr, "FAILED CHECK// " DOTXT (Message)                                              \
            " //IN FUNCTION//%s//IN LINE//%d//IN FILE//%s//\n",                                             \
             __PRETTY_FUNCTION__, __LINE__, __FILE__);                                                     \
        } while (0)


#define MCA(Condition, ReturnNum)                     \
        do                                            \
        {                                             \
            if (!(Condition))                         \
            {                                         \
                MCP (Condition);                      \
                return ReturnNum;                     \
            }                                         \
        } while (0)

#define MSA(Condition, ReturnNum)                     \
        do                                            \
        {                                             \
            if (!(Condition))                         \
            {                                         \
                stack->err += ReturnNum;              \
            }                                         \
        } while (0)

#define INIT(Var)                                                             \
        do                                                                    \
        {                                                                     \
            initialize_info (Var, LOCATION);                                  \
        } while (0)

typedef double  StackDataType;

typedef struct
{
    char* file = NULL;
    char* func = NULL;
    int   line;
} StructInfo;

#ifdef GREEN

// typedef struct
// {
//     int             size            = 0;
//     int             capacity        = 0;
//     StackDataType*  data            = NULL;
//     int             err             = 0;
//     char*           canary          = NULL;
//     int             canary_size     = 0;
//     StructInfo*     birth           = NULL;
//     StructInfo*     source          = NULL;
// } StructStack;

#endif

// #ifndef GREEN

typedef struct
{
    double          front_canary    = NULL;
    unsigned int    hash            = 0;
    int             size            = 0;
    int             capacity        = 0;
    StackDataType*  data            = NULL;
    double          canary          = 0;
    int             canary_size     = 0;
    int             err             = 0;
    StructInfo*     birth           = NULL;
    StructInfo*     source          = NULL;
    double          end_canary      = NULL;
} StructStack;

// #endif

#ifdef GREEN
    static const int PROTECTION_LEVEL = 0;
#endif

#ifdef YELLOW
    static const int PROTECTION_LEVEL = 1;
#endif

#ifdef ORANGE
    static const int PROTECTION_LEVEL = 2;
#endif

#ifdef RED
    static const int PROTECTION_LEVEL = 3;
#endif

#ifdef PURPLE
    static const int PROTECTION_LEVEL = 4;
#endif

//unsigned int hash_h31 (const void* array, int size);

//unsigned int djb33_hash(const char* s, int len);

unsigned int HashFAQ6 (const char * str);

bool stack_is_full (StructStack* stack);

bool stack_is_empty (StructStack* stack);

void swap_byte_by_byte (void* FirstData, void* SecondData, int Size);

int compare_byte_by_byte (void* FirstData, void* SecondData, int Size);

void copy_byte_by_byte (void* FirstData, void* SecondData, int Size);   

static int create_canary (StructStack* stack, int Size);

static int canaries_alive (StructStack* stack);

static int make_stack_bigger (StructStack* stack);

static int poison (void* Victim, int Size);

static int stack_variator (StructStack* stack);

int stack_dump (StructStack* stack);

int initialize_info (StructInfo* info, const char* File, const char* Function, int Line);

static int print_head (StructStack* stack, FILE* stream);

static int print_error (const char* ErrorName, StructInfo* info, FILE* Stream);

static int print_stack_data_double (StructStack* stack);

/*!
    @brief Function that initializes stack

    @param[in] Amount maximum amount of objects in stack
    @param[in] Size size of one object

    @return returns pointer to the stack structure

*/
int stack_constructor (StructStack* stack, int Capacity);

int push_in_stack (StructStack* stack, StackDataType x);

int peek_from_stack (StructStack* stack, StackDataType* x);

int pop_from_stack (StructStack* stack, StackDataType* x);

int stack_destructor (StructStack* stack);

#endif
