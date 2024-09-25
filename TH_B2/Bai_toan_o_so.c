#include "stdio.h"
#include "stdlib.h"
#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MAX_OPERATOR 4
#define Maxlenght 5000

typedef struct 
{
    int eightPuzzel [ROWS][COLS];
    int emptyRow;
    int emptyCol;
}State;

/*----------------------------------------------------------------------------*/
//in trang thai
void printState (State state)
{
    printf("------------------------------\n");
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            printf("|%d ", state.eightPuzzel[row][col]);            
        }
        printf("|\n");
    }
    printf("------------------------------\n");
}

/*----------------------------------------------------------------------------*/
//so sanh trang thai
int compareState (State state1, State state2)
{
    if (state1.emptyCol != state2.emptyCol || state1.emptyRow != state2.emptyRow)
        return 0;
    for (int row = 0; row < ROWS; row++)
        for (int col = 0; col < COLS; col++)
            if(state1.eightPuzzel[row][col] != state2.eightPuzzel[row][col])            
                return 0; 
    return 1;
}

/*----------------------------------------------------------------------------*/
//check goal
int check_goal (State state, State goal)
{
    return compareState(state, goal);
}

/*------------------------------------------------------------------------------*/
//Cac hanh dong

int upOperator (State state, State *result)
{
    *result = state;
    int empRowCurrent = state.emptyRow;
    int empColCurrent = state.emptyCol;

    if (empRowCurrent > 0)
    {
        result->emptyRow = empRowCurrent - 1;
        result->emptyCol = empColCurrent;
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent - 1][empColCurrent];
        result->eightPuzzel[empRowCurrent - 1][empColCurrent] = EMPTY;
        return 1;
    }
    return 0;
}

int downOperator (State state, State *result)
{
    *result = state;
    int empRowCurrent = state.emptyRow;
    int empColCurrent = state.emptyCol;

    if (empRowCurrent < ROWS)
    {
        result->emptyRow = empRowCurrent + 1;
        result->emptyCol = empColCurrent;
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent + 1][empColCurrent];
        result->eightPuzzel[empRowCurrent + 1][empColCurrent] = EMPTY;
        return 1;
    }
    return 0;
}

int leftOperator (State state, State *result)
{
    *result = state;
    int empRowCurrent = state.emptyRow;
    int empColCurrent = state.emptyCol;

    if (empColCurrent > 0)
    {
        result->emptyCol = empColCurrent - 1;
        result->emptyRow = empRowCurrent;
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent][empColCurrent - 1];
        result->eightPuzzel[empRowCurrent][empColCurrent - 1] = EMPTY;
        return 1;
    }
    return 0;
}

int rightOperator (State state, State *result)
{
    *result = state;
    int empRowCurrent = state.emptyRow;
    int empColCurrent = state.emptyCol;

    if (empColCurrent < COLS)
    {
        result->emptyCol = empColCurrent + 1;
        result->emptyRow = empRowCurrent;
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent][empColCurrent + 1];
        result->eightPuzzel[empRowCurrent][empColCurrent + 1] = EMPTY;
        return 1;
    }
    return 0;
}

const char *actions[] = {"First state", "Move cell EMPTY to UP", "Move cell EMPTY to  DOWN", "Move cell EMPTY to LEFT", "Move cell EMPTY to RIGHT"};

int callOperators (State state, State *result, int opt)
{
    switch (opt)
    {
        case 1: return upOperator(state, result);
        case 2: return downOperator(state, result);
        case 3: return leftOperator(state, result);
        case 4: return rightOperator(state, result); 
        default: printf("Cannot call operators");
        return 0;
    }    
}

/*----------------------------------------------------------------------------*/
//Ham heuristic 1
//Dem so trang thai khac voi muc tieu
int heuristic1 (State state1, State goal)
{
    int count = 0;
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            if (state1.eightPuzzel[row][col] != goal.eightPuzzel[row][col])
            {
                count++;
            }
            
        }
        
    }
    return count;
}

//Ham heuristic 2 - Mahathan
//Dem so buoc chuyen o sai ve o dung cua trang thai muc tieu
int heuristic2 (State state, State goal)
{
    int count = 0;
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            if (state.eightPuzzel[row][col] != EMPTY)
            {
                for (int row_g = 0; row_g < ROWS; row_g++)
                {
                    for (int col_g = 0; col_g < COLS; col_g++)
                    {
                        if (state.eightPuzzel[row][col] == goal.eightPuzzel[row_g][col_g])
                        {
                            count += abs(row - row_g) + abs(col - col_g);
                            col_g = COLS; //break loop col_g
                            row_g = ROWS; //break loop row_g
                        }
                        
                    }
                    
                }
                
            }
            
        }
        
    }
    return count;
}

/*----------------------------------------------------------------------------*/
//khai bao cau truc Node de dung cay tim kiem
typedef struct Node
{
    State state;
    struct Node *parent;
    int no_function;
    int heuristic;
}Node;

//khai bao cau truc danh sach
typedef struct 
{
    Node *Elements[Maxlenght];
    int size;
}List;

//khoi tao danh sach rong
void makeNull_List (List *list)
{
    list->size = 0;
}

//kiem tra danh sach rong
int empty_List (List list)
{
    return (list.size == 0);
}

//kiem tra danh sach full
int full_List (List list)
{
    return (list.size == Maxlenght);
}

//truy van gia tri cua phan tu tai vi tri p
Node *element_at (int p, List list)
{
    return (list.Elements[p-1]);
}

//them phan tu vao vi tri position trong danh sach
void push_List (Node *x, int position, List *list)
{
    if (!full_List(*list))
    {
        for (int i = list->size; i >= position; i--)
        {
            list->Elements[i] = list->Elements[i-1];            
        }
        list->Elements[position-1] = x;
        list->size++;        
    }
    else
        printf("List is full!\n");
}

//xoa phan tu tai vi tri position ra khoi danh sach
void delete_List (int position, List *list)
{
    if (empty_List(*list))
    {
        printf("List is empty!\n");
    }
    else if (position < 1 || position > list->size)
    {
        printf("Position is not possible to delete\n");
    }
    else{
        for (int i = position-1; i < list->size; i++)
        {
            list->Elements[i] = list->Elements[i+1];            
        }
        list->size--;
    }   
    
}

//tim trang thai state co thuoc Open Close hay khong?
//luu trang tahai tim duoc vao bien *position
Node *find_State (State state, List list, int *position)
{
    int i;
    for (i = 1; i <= list.size; i++)
        if (compareState(element_at(i, list)->state, state))
        {
            *position = i;
            return element_at(i, list);
        }
    return NULL;    
}

//sap xep danh sach theo trong so heuristic
void sort_List (List *list)
{
    for (int i = 0; i < list->size-1; i++)
    {
        for (int j = i+1; j < list->size; j++)
        {
            if (list->Elements[i]->heuristic > list->Elements[j]->heuristic)
            {
                Node *node = list->Elements[i];
                list->Elements[i] = list->Elements[j];
                list->Elements[j] = node;
            }            
        }        
    }    
}

//thuat toan tim kiem tot nhat dau tien
//Ham f = h
Node *best_first_search(State state, State goal)
{
    List Open_BFS;
    List Close_BFS;
    makeNull_List(&Open_BFS);
    makeNull_List(&Close_BFS);
    Node *root = (Node*) malloc (sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    root->heuristic = heuristic1(root->state, goal);
    push_List(root, Open_BFS.size + 1, &Open_BFS);
    while (!empty_List(Open_BFS))
    {
        Node *node = element_at(1, Open_BFS);
        delete_List(1, &Open_BFS);
        push_List(node, Close_BFS.size+1, &Close_BFS);
        if (check_goal(node->state, goal))
        {
            return node;
        }
        for (int opt = 1; opt <= MAX_OPERATOR; opt++)
        {
            State newstate;
            newstate = node->state;
            if (callOperators(node->state, &newstate, opt))
            {
                Node *newnode = (Node*) malloc (sizeof(Node));
                newnode->state = newstate;
                newnode->parent = node;
                newnode->no_function = opt;
                newnode->heuristic = heuristic1(newstate, goal);
                // kiem tra trang thai moi sinh ra co thuoc open_bfs/close_bfs
                int pos_Open = 0;
                int pos_Close = 0;
                Node *nodeFoundOpen = find_State(newstate, Open_BFS, &pos_Open);
                Node *nodeFoundClose = find_State(newstate, Close_BFS, &pos_Close);
                if (nodeFoundOpen == NULL && nodeFoundClose == NULL)
                {
                    push_List(newnode, Open_BFS.size + 1, &Open_BFS);
                }
                else if (nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newnode->heuristic)
                {
                    delete_List(pos_Open, &Open_BFS);
                    push_List(newnode, pos_Open, &Open_BFS);
                }
                else if (nodeFoundClose != NULL && nodeFoundClose->heuristic > newnode->heuristic)
                {
                    delete_List(pos_Close, &Close_BFS);
                    push_List(newnode, Open_BFS.size + 1, &Open_BFS);
                }
                sort_List(&Open_BFS);                
            }                  
        }                
    }
    return NULL;    
}

// ham in ket ua cua thuat toan BFS
void print_WayToGetGoal (Node *node){
    List listPrint;
    makeNull_List(&listPrint);
    // duyet nguoc ve parent
    while (node->parent != NULL)
    {
        push_List(node, listPrint.size + 1, &listPrint);
        node = node->parent;
    }
    push_List(node, listPrint.size + 1, &listPrint);
    int no_action;
    for (int i = listPrint.size; i > 0; i--)
    {
        printf ("Action %d: %s\n", no_action, actions[element_at(i, listPrint)->no_function]);
        printState(element_at(i, listPrint)->state);
        no_action ++;
    } 
}

int main()
{
    State state, result;
    state.emptyRow = 1;
    state.emptyCol = 1;
    state.eightPuzzel[0][0] = 3;
    state.eightPuzzel[0][1] = 4;
    state.eightPuzzel[0][2] = 5;
    state.eightPuzzel[1][0] = 1;
    state.eightPuzzel[1][1] = 0;
    state.eightPuzzel[1][2] = 2;
    state.eightPuzzel[2][0] = 6;
    state.eightPuzzel[2][1] = 7;
    state.eightPuzzel[2][2] = 8;

    State goal;
    goal.emptyRow = 0;
    goal.emptyCol = 0;
    goal.eightPuzzel[0][0] = 0;
    goal.eightPuzzel[0][1] = 1;
    goal.eightPuzzel[0][2] = 2;
    goal.eightPuzzel[1][0] = 3;
    goal.eightPuzzel[1][1] = 4;
    goal.eightPuzzel[1][2] = 5;
    goal.eightPuzzel[2][0] = 6;
    goal.eightPuzzel[2][1] = 7;
    goal.eightPuzzel[2][2] = 8;

    // printf ("Trang thai bat dau \n");
    // printState(state);
    // for (int opt = 1; opt <= 4; opt++)
    // {
    //     callOperators(state, &result, opt);
    //     if(!compareState(state, result))
    //     {
    //         printf("Hanh dong %s thanh cong:\n", actions[opt]);
    //         printState(result);
    //     }
    //     else{
    //         printf("Hanh dong %s khong thanh cong!\n", actions[opt]);
    //     }
    // }    

    Node *p = best_first_search(state, goal);
    print_WayToGetGoal(p);
    return 0;
}