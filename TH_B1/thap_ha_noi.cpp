#include <stdio.h>
#include <iostream>
#include <stack>
#include <queue>

#define so_dia 3
const int A[] = {3, 2, 1};

// Khai bao cau truc trang thai ---------------------------------------------
typedef struct State {
    std::stack<int> col_A;
    std::stack<int> col_B;
    std::stack<int> col_C;
}State;

State create_goal (){
    State goal;
    for (int i = 0; i < so_dia; i++)
    {
        goal.col_C.push(A[i]);
    }
    
    return goal;
}

State goal = create_goal();
// Cac hanh dong ------------------------------------------------------------
int move_A_to_B (State cur_state, State *result){
    if (cur_state.col_A.empty()){
        return 0;
    }
    *result = cur_state;
    int top_A = cur_state.col_A.top();
    result->col_B.push(top_A);
    result->col_A.pop();
    return 1;    
}

int move_A_to_C (State cur_state, State *result){
    if (cur_state.col_A.empty()){
        return 0;
    }
    *result = cur_state;
    int top_A = cur_state.col_A.top();
    result->col_C.push(top_A);
    result->col_A.pop();
    return 1;    
}

int move_B_to_C (State cur_state, State *result){
    if (cur_state.col_B.empty()){
        return 0;
    }
    *result = cur_state;
    int top_B = cur_state.col_B.top();
    result->col_C.push(top_B);
    result->col_B.pop();
    return 1;    
}

int move_B_to_A (State cur_state, State *result){
    if (cur_state.col_B.empty()){
        return 0;
    }
    *result = cur_state;
    int top_B = cur_state.col_B.top();
    result->col_A.push(top_B);
    result->col_B.pop();
    return 1;    
}

int move_C_to_A (State cur_state, State *result){
    if (cur_state.col_C.empty()){
        return 0;
    }
    *result = cur_state;
    int top_C = cur_state.col_C.top();
    result->col_A.push(top_C);
    result->col_C.pop();
    return 1;    
}

int move_C_to_B (State cur_state, State *result){
    if (cur_state.col_C.empty()){
        return 0;
    }
    *result = cur_state;
    int top_C = cur_state.col_C.top();
    result->col_B.push(top_C);
    result->col_C.pop();
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
    // So sánh cột A
    if (a.col_A.size() != b.col_A.size()) return 0;
    std::stack<int> tmpA = a.col_A;
    std::stack<int> tmpB = b.col_A;
    while (!tmpA.empty() && !tmpB.empty()) {
        if (tmpA.top() != tmpB.top()) return 0;
        tmpA.pop();
        tmpB.pop();
    }

    // So sánh cột B
    if (a.col_B.size() != b.col_B.size()) return 0;
    tmpA = a.col_B;
    tmpB = b.col_B;
    while (!tmpA.empty() && !tmpB.empty()) {
        if (tmpA.top() != tmpB.top()) return 0;
        tmpA.pop();
        tmpB.pop();
    }

    // So sánh cột C
    if (a.col_C.size() != b.col_C.size()) return 0;
    tmpA = a.col_C;
    tmpB = b.col_C;
    while (!tmpA.empty() && !tmpB.empty()) {
        if (tmpA.top() != tmpB.top()) return 0;
        tmpA.pop();
        tmpB.pop();
    }
    return 1;
}


// Tim trang thai trong stack open/close
int find_State (State state, std::stack<Node*> openStack){
    while (!openStack.empty()){
        if (compareStates (openStack.top()->state, state)){
            return 1;
        }
        openStack.pop();
    }
    return 0;
}

// chay thuat toan --------------------------------------------------------------------------------
//Thuat toan tim theo chieu sau
Node *DFS_Algorithm (State state){
    //khai bao ngan xep Open/close 
    std::stack<Node*> Open_DFS;
    std::stack<Node*> Close_DFS;
    //tao nut trang thai cha
    Node *root = (Node*) malloc (sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_action = 0;
    Open_DFS.push(root);

    while (!Open_DFS.empty()){
        //lay mot dinh trong ngan xep
        Node *node = Open_DFS.top();
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

std::stack<int> invert_stack (std::stack<int> stack){
    std::stack<int> tmp;
    while (!stack.empty())
    {
        tmp.push(stack.top());
        stack.pop();
    }
    return tmp;
}

void print_state (State state){
    std::stack<int> tmp = invert_stack(state.col_A);
    printf("\nCot A: ");
    while (!tmp.empty())
    {
        printf("%d ", tmp.top());
        tmp.pop();
    }

    tmp = invert_stack(state.col_B);
    printf("\nCot B: ");
    while (!tmp.empty())
    {
        printf("%d ", tmp.top());
        tmp.pop();
    }

    tmp = invert_stack(state.col_C);
    printf("\nCot C: ");
    while (!tmp.empty())
    {
        printf("%d ", tmp.top());
        tmp.pop();
    }     
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
    //in ra thu tu hanh dong chuyen nuoc
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
    // cur_state.col_A.push(1);
    // cur_state.col_B.push(3);
    // cur_state.col_B.push(2);

    // printf ("Trang thai bat dau");
    // print_state(cur_state);
    // int otp;
    // for (otp = 1; otp <= 6; otp ++){
    //     int thuchien = call_operator (cur_state, &result, otp);
    //     if (thuchien ==1 ){
    //         printf ("\nHanh dong %s thanh cong", action[otp]);
    //         print_state (result);
    //     }
    //     else {
    //         printf("\nHanh dong %s KHONG thanh cong", action[otp]);
    //     }
    // }
    // print_state(goal);
    State cur_state ;
    cur_state.col_A.push(3);
    cur_state.col_A.push(2);
    cur_state.col_A.push(1);
    Node *p = DFS_Algorithm(cur_state);
    print_WayToGetGoal(p); 
    return 0;
}