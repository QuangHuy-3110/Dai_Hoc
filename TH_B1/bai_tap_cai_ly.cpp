#include "stdio.h"
#include "malloc.h"

#define Maxlengh 100
#define so_ly 6
const char goal[] = {'N', 'N', 'N', 'N', 'N', 'N'};
const char start[] = {'N', 'U', 'N', 'U', 'N', 'U'};

/*------------------------------------------------------------*/
//Tao State
typedef struct 
{
    int n;
    char A[so_ly];
}State;

void makeNull_state (State *state)
{
    state->n = so_ly;    
}

void set_state (State *state)
{
    for (int i = 0; i < state->n; i++)
    {
        state->A[i] = start[i];
    }
    
}

/*--------------------------------------------------------------*/
//Cac hanh dong
char dao_ly (char C)
{
    if (C == 'U')
    {
        return 'N';
    }
    return 'U';
    
}

int up_ly (State cur_state, int vitri, State *result)
{
    if (vitri == 0 || vitri == cur_state.n - 1 || cur_state.A[vitri] == 'U')
    {
        return 0;
    }
    for (int i = 0; i < cur_state.n; i++)
    {
        if (i == vitri - 1 || i == vitri || i == vitri + 1)
        {
            result->A[i] = dao_ly (cur_state.A[i]);
        }
        else
        {
            result->A[i] = cur_state.A[i];
        }        
    }
    return 1;    
}

int ngua_ly (State cur_state, int vitri, State *result)
{
    if (vitri == 0 || vitri == cur_state.n - 1 || cur_state.A[vitri] == 'N')
    {
        return 0;
    }
    for (int i = 0; i < cur_state.n; i++)
    {
        if (i == vitri - 1 || i == vitri || i == vitri + 1)
        {
            result->A[i] = dao_ly (cur_state.A[i]);
        }
        else
        {
            result->A[i] = cur_state.A[i];
        }        
    }
    return 1;    
}

const char *action_name[] = {"First state", "up ly", "ngua ly"};

int action (State cur_state, State *result, int vitri, int opt)
{
    switch (opt)
    {
        case 1: return up_ly (cur_state, vitri, result);
        case 2: return ngua_ly (cur_state, vitri, result);
    }

    return 0;
}

void print_state (State cur_state)
{
    for (int i = 0; i < cur_state.n ; i++)
    {
        printf("    %c  ", cur_state.A[i]);
    }
    printf("\n ------------------------------------------------ \n");
    
}
//Check goal
int check_goal (State cur_state)
{
    for (int i = 0; i < cur_state.n; i++)
    {
        if (cur_state.A[i] != goal[i])
        {
            return 0;
        }
        
    }
    return 1;
    
}

/*--------------------------------------------------------------------------------------*/
typedef struct Node 
{
    State state;
    Node *parent;
    int no_action;    
}Node;

typedef struct 
{
    Node *Elements[Maxlengh];
    int top_idx;
}Stack;

void makeNull_Stack (Stack *stack)
{
    stack->top_idx = Maxlengh;
}

int empty_Stack (Stack stack)
{
    return stack.top_idx == Maxlengh;
}

int full_stack (Stack stack)
{
    return stack.top_idx == 0;
}

Node *top (Stack stack)
{
    if (!empty_Stack(stack))    
    {
        return stack.Elements[stack.top_idx];
    }
    return NULL;
    
}

void pop (Stack *stack)
{
    if (!empty_Stack(*stack))
    {
        stack->top_idx ++;
    }
    else
        printf("Erorr, stack is empty!");  
}

void push (Stack *stack, Node *node)
{
    if (!full_stack(*stack))
    {
        stack->top_idx --;
        stack->Elements[stack->top_idx] = node;
    }
    else
        printf ("Erorr, stack is full!");
}

/*---------------------------------------------------------------------*/
//So sanh trang thai
int compare (State a, State b)
{
    for (int i = 0; i < a.n; i++)
    {
        if (a.A[i] != b.A[i])
        {
            return 0;
        }
        
    }
    return 1;
}

//Tim trang thai
int find_state (Stack stack, State cur_state)
{
    while (!empty_Stack(stack))
    {
        if (compare(top(stack)->state, cur_state))
        {
            return 1;
        }
        
    }
    return 0;
}

//DFS cai ly
Node *DFS_cai_ly (State cur_state)
{
    //Tao open close
    Stack Open;
    Stack Close;
    makeNull_Stack(&Close);
    makeNull_Stack(&Open);
    Node *root = (Node*) malloc (sizeof (Node));
    root->no_action = 0;
    root->parent = NULL;
    root->state = cur_state;
    push(&Open, root);
    while (!empty_Stack(Open))
    {
        Node *node = top(Open);
        pop(&Open);
        push(&Close, node);
        if (check_goal(node->state))
        {
            return node;
        }
        for (int i = 0; i < so_ly; i++)
        {
            for (int j = 1; j <= 2; j++)
            {
                State newstate;
                if (action(node->state, &newstate, i, j))
                {
                    if (find_state(Open, newstate) || find_state(Close, newstate))
                    {
                        continue;
                    }
                    Node *newNode = (Node*) malloc (sizeof(Node));
                    newNode->state = newstate;
                    newNode->parent = node;
                    newNode->no_action = j;
                    push (&Open, newNode);
                }
                
            }
            
        }
        
        
    }
    return NULL;
}

void print_ToGetGoal (Node *Node){
    Stack Stackprint;
    makeNull_Stack (&Stackprint);
    while (Node->parent != NULL){
        push(&Stackprint, Node);
        Node = Node->parent;        
    }
    push(&Stackprint, Node);
    int no_action = 0;
    while (!empty_Stack(Stackprint)){
        printf ("Hanh dong %d tai vi tri %s thanh cong\n", no_action, action[top(Stackprint)->no_action]);
        print_state(top(Stackprint)->state);
        pop(&Stackprint);
        no_action ++;
    }
    
}

int main ()
{
    State state, result;
    makeNull_state(&state);
    makeNull_state(&result);
    set_state(&state);
    // // set_state(&result);
    // printf(action_name[0]);
    // print_state(state);
    // for (int i = 0; i < state.n; i++)
    // {
    //     for (int j = 1; j <= 2; j++)
    //     {
    //         if ( action(state, &result, i, j) )
    //         {
    //             printf ("Hanh dong %s tai vi tri %d thanh cong\n", action_name[j], i+1);
    //             print_state(result);
    //         }
    //         else
    //             printf("Hanh dong %s tai vi tri %d KHONG thanh cong\n", action_name[j], i+1);
    //     }
        
        
    // }
    
    Node *node = DFS_cai_ly(state);
    print_ToGetGoal(node);
    return 0;
}