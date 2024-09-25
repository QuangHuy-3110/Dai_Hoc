#include "stdio.h"
#include "malloc.h"

#define Max_tusi 3
#define Max_conquy 3
#define Max_lenght 100

typedef struct{
    int so_tusi; 
    int so_conquy;
    char vitri_thuyen;
}State;

void makeNull_State (State *state){
    state->so_conquy = 0;
    state->so_tusi = 0;
    state->vitri_thuyen = 'A';
}

//---------------------------------------------------//
// Dieu kien

int antoan (State cur_state, int so_tusi, int so_quy){
    State next_state;
    next_state.so_conquy = cur_state.so_conquy + so_quy;
    next_state.so_tusi = cur_state.so_tusi + so_tusi;
    if (next_state.so_conquy < 0 || next_state.so_tusi < 0 || next_state.so_conquy > Max_conquy || next_state.so_tusi > Max_tusi){
        return 0;
    }
    if (next_state.so_conquy > next_state.so_tusi && next_state.so_tusi > 0 ){
        return 0;
    }
    if (next_state.so_tusi < Max_tusi && next_state.so_conquy < next_state.so_tusi){
        return 0;
    }

    return 1;
}

//Check GOAL
int check_goal (State cur_state){
    return (cur_state.so_conquy == 0 && cur_state.so_tusi == 0);
}

//cac hanh dong
const char *option [] = {"Firt state","Dua 1 tu si sang B", "Dua 1 con quy sang B", "Dua 2 tu si sang B",
                         "Dua 2 con quy sang B", "Dua 1 tu si va 1 con quy sang B",
                         "Dua 1 tu si sang A", "Dua 1 con quy Sang A", "Dua 2 tu si sang A", "Dua 2 con quy sang A", 
                         "Dua 1 tu si va 1 con quy sang A", };

int dua_1_tusi_sangB (State cur_state, State *result){    
    if ((cur_state.so_tusi == 0 || cur_state.vitri_thuyen == 'B') || !antoan (cur_state, -1, 0) ){
        return 0;
    }
    result->so_tusi = cur_state.so_tusi - 1;
    result->so_conquy = cur_state.so_conquy;
    result->vitri_thuyen = 'B';
    return 1;
}

int dua_1_conquy_sangB (State cur_state, State *result){
    if ((cur_state.so_conquy == 0 || cur_state.vitri_thuyen == 'B') || !antoan (cur_state, 0, -1)){
        return 0;
    }
    result->so_conquy = cur_state.so_conquy - 1;
    result->so_tusi = cur_state.so_tusi;
    result->vitri_thuyen = 'B';
    return 1;
}

int dua_2_tusi_sangB (State cur_state, State *result){
    if ((cur_state.so_tusi <= 1 || cur_state.vitri_thuyen == 'B') || !antoan (cur_state, -2, 0)){
        return 0;
    }
    result->so_tusi = cur_state.so_tusi - 2;
    result->so_conquy = cur_state.so_conquy;
    result->vitri_thuyen = 'B';
    return 1;
}

int dua_2_conquy_sangB (State cur_state, State *result){
    if((cur_state.so_conquy <=1 || cur_state.vitri_thuyen == 'B') || !antoan (cur_state, 0, -2)){
        return 0;
    }
    result->so_conquy = cur_state.so_conquy - 2;
    result->so_tusi = cur_state.so_tusi;
    result->vitri_thuyen = 'B';
    return 1;
}

int dua_1_conquy_1_tusi_sangB (State cur_state, State *result){
    if(cur_state.so_conquy == 0 || cur_state.so_tusi == 0 || cur_state.vitri_thuyen == 'B' || !antoan (cur_state, -1, -1)){
        return 0;
    }
    result->so_conquy = cur_state.so_conquy - 1;
    result->so_tusi = cur_state.so_tusi - 1;
    result->vitri_thuyen = 'B';
    return 1;
}

int dua_1_tusi_sangA (State cur_state, State *result){
    if ((cur_state.so_tusi == 3 || cur_state.vitri_thuyen == 'A') || !antoan (cur_state, 1, 0)){
        return 0;
    }
    result->so_tusi = cur_state.so_tusi + 1;
    result->so_conquy = cur_state.so_conquy;
    result->vitri_thuyen = 'A';
    return 1;
}

int dua_1_conquy_sangA (State cur_state, State *result){
    if ((cur_state.so_conquy == 3 || cur_state.vitri_thuyen == 'A') || !antoan (cur_state, 0, 1)){
        return 0;
    }
    result->so_conquy = cur_state.so_conquy + 1;
    result->so_tusi = cur_state.so_tusi;
    result->vitri_thuyen = 'A';
    return 1;
}

int dua_2_tusi_sangA (State cur_state, State *result){
    if ((cur_state.so_tusi > 1 || cur_state.vitri_thuyen == 'A') || !antoan (cur_state, 2, 0)){
        return 0;
    }
    result->so_tusi = cur_state.so_tusi + 2;
    result->so_conquy = cur_state.so_conquy;
    result->vitri_thuyen = 'A';
    return 1;
}

int dua_2_conquy_sangA (State cur_state, State *result){
    if((cur_state.so_conquy > 1 || cur_state.vitri_thuyen == 'A') || !antoan (cur_state, 0, 2)){
        return 0;
    }
    result->so_conquy = cur_state.so_conquy + 2;
    result->so_tusi = cur_state.so_tusi;
    result->vitri_thuyen = 'A';
    return 1;
}

int dua_1_conquy_1_tusi_sangA (State cur_state, State *result){
    if(cur_state.so_conquy == Max_conquy || cur_state.so_tusi == Max_tusi || cur_state.vitri_thuyen == 'A' || !antoan (cur_state, 1, 1)){
        return 0;
    }
    result->so_conquy = cur_state.so_conquy + 1;
    result->so_tusi = cur_state.so_tusi + 1;
    result->vitri_thuyen = 'A';
    return 1;
}

void print_state (State cur_state ){
    printf ("Bo A: tu si %d ------ con quy %d\n", cur_state.so_tusi, cur_state.so_conquy);
    printf ("Bo B: tu si %d ------ con quy %d\n", Max_tusi - cur_state.so_tusi, Max_conquy - cur_state.so_conquy);
    printf ("Thuyen o bo: %c\n", cur_state.vitri_thuyen);
    printf ("--------------------------------\n");
}

int compare_State (State a, State b){
    if (a.so_conquy == b.so_conquy && a.so_tusi == b.so_tusi && a.vitri_thuyen == b.vitri_thuyen){
        return 1;
    }
    return 0;
}

int test (State cur_state, State *result, int option){
    switch (option)
    {
    case 1: return dua_1_tusi_sangB (cur_state, result);
    case 2: return dua_1_conquy_sangB (cur_state, result);
    case 3: return dua_2_tusi_sangB (cur_state, result);
    case 4: return dua_2_conquy_sangB (cur_state, result);
    case 5: return dua_1_conquy_1_tusi_sangB (cur_state, result);
    case 6: return dua_1_tusi_sangA (cur_state, result);
    case 7: return dua_1_conquy_sangA (cur_state, result);
    case 8: return dua_2_tusi_sangA (cur_state, result);
    case 9: return dua_2_conquy_sangA (cur_state, result);
    case 10: return dua_1_conquy_1_tusi_sangA (cur_state, result);     
    default:
        return 0;
    }
}
//------------------------------------------------------------------------------------------------//
typedef struct Node {
    State state;
    struct Node *parent;
    int no_function;    
} Node;

typedef struct {
    Node *Elements [Max_lenght];
    int top_idx;
} Stack;

void makeNull_Stack (Stack *stack){
    stack->top_idx = Max_lenght;
}

int empty_Stack (Stack stack){
    return stack.top_idx == Max_lenght;
}

int full_Stack (Stack stack){
    return stack.top_idx == 0;
}

Node *top (Stack stack){
    if (!empty_Stack(stack))
        return stack.Elements[stack.top_idx];
    return NULL;
}

void pop (Stack *stack){
    if (!empty_Stack (*stack)){
        stack->top_idx += 1;
    }
    else {
        printf("Erorr! stack is empty");
    }    
}

void push (Stack *stack, Node *x ){
    if (!full_Stack(*stack)){
        stack->top_idx -= 1;
        stack->Elements[stack->top_idx] = x;
    }
    else
        printf ("Erorr! stack is full");
    
}

//Tim trang thai trong open/close
int find_state (State state, Stack open){
    while (!empty_Stack(open)){
        if (compare_State(state, top(open)->state)){
            return 1;
        }
        pop(&open);
    }
    return 0;
}

Node *tusi_conquy_DFS (State cur_state){
    Stack Open;
    Stack Close;
    makeNull_Stack (&Open);
    makeNull_Stack (&Close);
    Node *root = (Node*) malloc (sizeof (Node));
    root->parent = NULL;
    root->state = cur_state;
    root->no_function = 0;
    push (&Open, root);
    while(!empty_Stack(Open)){
        Node *node = top(Open);
        pop(&Open);
        push(&Close, node);
        if(check_goal(node->state)){
            return node;
        }
        int opt = 0;
        for (opt = 1; opt <= 10; opt++){
            State newstate;
            makeNull_State (&newstate);
            if (test(node->state, &newstate, opt)){
                if (find_state(newstate, Close) || find_state(newstate, Open)){
                    continue;
                }
                Node *newNode = (Node*) malloc (sizeof(Node));
                newNode->state = newstate;
                newNode->parent = node;
                newNode->no_function = opt;
                push (&Open, newNode);

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
        printf ("\nAction: %d: %s\n", no_action, option[top(Stackprint)->no_function]);
        print_state(top(Stackprint)->state);
        pop(&Stackprint);
        no_action ++;
    }
    
}

int main(){
    State cur_state, result;
    makeNull_State(&cur_state);
    makeNull_State(&result);
    cur_state.so_conquy = 3;
    cur_state.so_tusi = 3;
    cur_state.vitri_thuyen = 'A';

    // printf("%s\n",option[0]);
    // print_state(cur_state);
    // int opt = 0;
    // for (opt = 1; opt <= 10; opt ++){
    //     if (test(cur_state, &result, opt)){
    //         printf("Hanh dong %s thanh cong: \n", option[opt]);
    //         print_state(result);
    //     }
    //     else {
    //         printf("Hanh dong %s khong thanh cong!\n", option[opt]);
    //     }
    // }

    Node *node = tusi_conquy_DFS(cur_state);
    print_ToGetGoal(node);    
    return 0;
}