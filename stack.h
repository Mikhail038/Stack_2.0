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

#define DOTXT(Message) #Message

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
            initialize_info (Var, __FILE__, __LINE__, __PRETTY_FUNCTION__);   \
        } while (0)

typedef double  StackDataType;

typedef struct
{
    char* file = NULL;
    char* func = NULL;
    int   line;
} StructInfo;

typedef struct
{
    char* file = NULL;
    char* func = NULL;
    int   line;
} StructSource;

typedef struct
{
    int num;
    const char** name =
    {
    "error",
    "Capacity < 0",
    "stack null ptr",
    "size > capacity",
    "birds are dead",
    "birth file null",
    "birth func null",
    "source file null",
    "source func null"
    };
} StructErr;

typedef struct
{
    int             size            = 0;
    int             capacity        = 0;
    StackDataType*  data            = NULL;
    char*           canary          = NULL;
    int             canary_size     = 0;
    int             err             = 0;
    StructInfo      birth           = {};
    StructInfo      source          = {};
} StructStack;


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

int initialize_info (StructInfo* info, const char* File, int Line, const char* Function);

static int print_head (StructStack* stack, FILE* stream);

static int print_error (StructStack* stack, int ErrorNumber, FILE* Stream);


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
