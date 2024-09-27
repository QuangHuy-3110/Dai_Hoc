#include <stdio.h>
#include <iostream>
#include <stack>
#include <queue>
#include <malloc.h>

#define so_dia 3

// Khai bao cau truc trang thai ---------------------------------------------
typedef struct State {
    int col_A[so_dia];
    int no_A;
    int col_B[so_dia];
    int no_B;
    int col_C[so_dia];
    int no_C;
}State;

State create_goal (){
    State goal;
    for (int i = 0; i < so_dia; i++){
        goal.col_C[i] = i+1;        
    }
    goal.no_C = so_dia;
    goal.no_A = 0;
    goal.no_B = 0;
    return goal;
}

State goal = create_goal();
// Cac hanh dong ------------------------------------------------------------
int move_A_to_B (State cur_state, State *result){
    if (cur_state.no_A == 0){
        return 0;
    }
    *result = cur_state;
    result->no_B = cur_state.no_B + 1;
    result->no_A = cur_state.no_A-1;
    result->col_B[cur_state.no_B] = cur_state.col_A[cur_state.no_A-1];
    return 1;    
}

int move_A_to_C (State cur_state, State *result){
    if (cur_state.no_A == 0){
        return 0;
    }
    *result = cur_state;
    result->no_A = cur_state.no_A-1;
    result->no_C = cur_state.no_C +1;    
    result->col_C[cur_state.no_C] = cur_state.col_A[cur_state.no_A-1];
    return 1;    
}

int move_B_to_C (State cur_state, State *result){
    if (cur_state.no_B == 0){
        return 0;
    }
    *result = cur_state;
    result->no_B = cur_state.no_B-1;
    result->no_C = cur_state.no_C+1; 
    result->col_C[cur_state.no_C] = cur_state.col_B[cur_state.no_B-1];
    return 1;
}

int move_B_to_A (State cur_state, State *result){
    if (cur_state.no_B == 0){
        return 0;
    }
    *result = cur_state;
    result->no_B = cur_state.no_B-1;
    result->no_A = cur_state.no_A +1; 
    result->col_A[cur_state.no_A] = cur_state.col_B[cur_state.no_B-1];
    return 1;
}

int move_C_to_A (State cur_state, State *result){
    if (cur_state.no_C == 0){
        return 0;
    }
    *result = cur_state;
    result->no_C = cur_state.no_C-1;
    result->no_A = cur_state.no_A +1; 
    result->col_A[cur_state.no_A] = cur_state.col_C[cur_state.no_C-1];
    return 1;
}

int move_C_to_B (State cur_state, State *result){
    if (cur_state.no_C == 0){
        return 0;
    }
    *result = cur_state;
    result->no_C = cur_state.no_C-1;
    result->no_B = cur_state.no_B +1; 
    result->col_B[cur_state.no_B] = cur_state.col_C[cur_state.no_C-1];
    return 1;
}

const char* action [] = {"First State", "Move 1 plate A to B", "Move 1 plate A to C",
                        "Move 1 plate B to C", "Move 1 plate B to A", "Move 1 plate C to A",
                        "Move 1 plate C to B"};

int call_operator (State cur_state, State *result, int option){
    switch (option)
    {
    case 1: return move_A_to_B(cur_state, result);
    case 2: return move_A_to_C(cur_state, result);
    case 3: return move_B_to_C(cur_state, result);
    case 4: return move_B_to_A(cur_state, result);
    case 5: return move_C_to_A (cur_state, result);
    case 6: return move_C_to_B (cur_state, result);    
    default:
        printf ("Error calls operators");
        return 0;
    }
}

// Khai bao cau truc Node-----------------------------------------------------------------------
typedef struct Node{
    State state;
    int no_action;
    struct Node *parent;
}Node;

int compareStates (State a, State b){
    // So sánh c?t A
    if (a.no_A != b.no_B){
        return 0;
    }    
    else
        for (int i = 0; i < a.no_A; i++){
            if (a.col_A[i] != b.col_B[i]){
                return 0;
            }
            
        }
    

    // So sánh c?t B
    if (a.no_B != b.no_B){
        return 0;
    }    
    else
        for (int i = 0; i < a.no_B; i++){
            if (a.col_B[i] != b.col_B[i]){
                return 0;
            }
            
        }

    // So sánh c?t C
    if (a.no_C != b.no_C){
        return 0;
    }    
    else
        for (int i = 0; i < a.no_C; i++){
            if (a.col_C[i] != b.col_C[i]){
                return 0;
            }
            
        }
    return 1;
}


// Tim trang thai trong stack open/close
int find_State (State state, std::queue<Node*> openStack){
    while (!openStack.empty()){
        if (compareStates (openStack.front()->state, state)){
            return 1;
        }
        openStack.pop();
    }
    return 0;
}

// chay thuat toan --------------------------------------------------------------------------------
//Thuat toan tim theo chieu sau
Node *BFS_Algorithm (State state){
    //khai bao ngan xep Open/close 
    std::queue<Node*> Open_DFS;
    std::queue<Node*> Close_DFS;
    //tao nut trang thai cha
    Node *root = (Node*) malloc (sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_action = 0;
    Open_DFS.push(root);

    while (!Open_DFS.empty()){
        //lay mot dinh trong ngan xep
        Node *node = Open_DFS.front();
        Open_DFS.pop();
        Close_DFS.push(node);
        //kiem tra 1 dinh lay ra co phai trang thai muc tieu?
        if (compareStates(node->state, goal)){
            return node;
        }
        int opt;
        //goi cac phep toan trang thai
        for(opt = 1; opt <= 6; opt++){
            State newstate;
            if (call_operator(node->state, &newstate, opt)){
                //neu trang thai moi da ton tai thi bo qua
                if (find_State(newstate, Close_DFS) || find_State(newstate, Open_DFS)){
                    continue;
                }
                Node *newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->parent = node;
                newNode->no_action = opt;
                Open_DFS.push(newNode);
            }
        }
    }
    return NULL;
}

void print_state (State state){
    printf("\nCot A: ");
    if (state.no_A == 0)
    {
        printf("0");
    }
    else    
        for (int i = 0; i < state.no_A; i++){
            printf("%d ", state.col_A[i]);
        }

    printf("\nCot B: ");
    if (state.no_B == 0)
    {
        printf("0");
    }
    else 
        for (int i = 0; i < state.no_B; i++){
            printf("%d ", state.col_B[i]);
        }

    printf("\nCot C: ");
    if (state.no_C == 0)
    {
        printf("0");
    }
    else 
        for (int i = 0; i < state.no_C; i++){
            printf("%d ", state.col_C[i]);
        }    
    printf("\n");
}

//in ket qua chuyen nuoc de dat den muc tieu
void print_WayToGetGoal (Node *node){
    std::stack<Node*> stackPrint;
    //duyet nguoc ve parent 
    while (node->parent != NULL){
        stackPrint.push(node);
        node = node->parent;
    }
    stackPrint.push(node);

    int no_action = 0;
    while (!stackPrint.empty()){
        printf("Action %d: %s", no_action, action[stackPrint.top()->no_action]);
        print_state (stackPrint.top()->state);
        stackPrint.pop();
        no_action++;
    }
}

int main () {
    // State cur_state, result;
    // cur_state.col_A[0] = 1;
    // cur_state.no_A = 1;
    // cur_state.col_B[0] = 3;
    // cur_state.col_B[1] = 2;
    // cur_state.no_B = 2;
    // cur_state.no_C = 0;

    // printf ("Trang thai bat dau");
    // print_state(cur_state);
    //  int otp;
    //  for (otp = 1; otp <= 6; otp ++){
    //      int thuchien = call_operator (cur_state, &result, otp);
    //      if (thuchien ==1 ){
    //          printf ("\nHanh dong %s thanh cong", action[otp]);
    //          print_state (result);
    //      }
    //      else {
    //          printf("\nHanh dong %s KHONG thanh cong", action[otp]);
    //      }
    //  }
    // print_state(goal);
    State cur_state ;
    cur_state.col_A[0] = 3;
    cur_state.col_A[1] = 2;
    cur_state.col_A[2] = 1;
    cur_state.no_A = so_dia;
    cur_state.no_B = 0;
    cur_state.no_C = 0;
    Node *p = BFS_Algorithm(cur_state);
    print_WayToGetGoal(p); 
    return 0;
}
