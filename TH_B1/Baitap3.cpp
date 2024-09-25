#include "stdio.h"
#include "malloc.h"

#define Xmax 9
#define Ymax 4
#define goal 6
#define empty 0
#define Maxlength 100

typedef struct {
    int X = 0;
    int Y = 0;
} State;

int max (int a, int b){
    return a>b? a:b;
}

int min (int a, int b){
    return a<b? a:b;
}

int pourWaterFullX (State cur_state, State *result){
    if (cur_state.X == Xmax){
        return 0;
    }
    result->X = Xmax;
    result->Y = cur_state.Y;
    return 1;
}

int pourWaterFullY (State cur_state, State *result){
    if (cur_state.Y == Ymax){
        return 0;
    }
    result->Y = Ymax;
    result->X = cur_state.X;
    return 1;
}

int pourWaterEmptyX (State cur_state, State *result){
    if (cur_state.X == 0){
        return 0;
    }
    result->X = empty;
    result->Y = cur_state.Y;
    return 1;
}

int pourWaterEmptyY (State cur_state, State *result){
    if (cur_state.Y == 0){
        return 0;
    }
    result->Y = empty;
    result->X = cur_state.X;
    return 1;
}

int pourWaterXY (State cur_state, State *result){
    if (cur_state.X > 0 && cur_state.Y < Ymax){
        result->X = max(cur_state.X - (Ymax - cur_state.Y), empty);
        result->Y = min(cur_state.X + cur_state.Y, Ymax);
        return 1;
    }
    return 0;
}

int pourWaterYX (State cur_state, State *result){
    if (cur_state.Y > 0 && cur_state.X < Xmax){
        result->Y = max(cur_state.Y - (Xmax - cur_state.X), empty);
        result->X = min(cur_state.Y + cur_state.X, Xmax);
        return 1;
    }
    return 0;
}

int call_operater (State cur_state, State *result, int option){
    switch (option)
    {
    case 1: return pourWaterFullX(cur_state, result);
    case 2: return pourWaterFullY(cur_state, result);
    case 3: return pourWaterEmptyX(cur_state, result);
    case 4: return pourWaterEmptyY(cur_state, result);
    case 5: return pourWaterXY (cur_state, result);
    case 6: return pourWaterYX (cur_state, result);    
    default:
        printf ("Error calls operators");
        return 0;
    }
}

void print_state (State cur_state){
    printf ("\nX:%d ---- Y:%d", cur_state.X, cur_state.Y);
}

void makeNullState (State *state){
    state->X = 0;
    state->Y = 0;
}

int goalcheck (State state){
    return (state.X == goal || state.Y == goal);
}

const char* action [] = {"First State", "pour Water Full X", "pour Water Full Y",
                        "pour Water Empty X", "pour Water Empty Y", "pour Water X to Y",
                        "pour Water Y to X"};


typedef struct Node {
    State state;
    struct Node *parent;
    int no_function;
}Node;

/*---------------------------------------------------------------------------------------*/
//Khai bao cau truc Stack de luu trang thai duyet
typedef struct {
    Node *Elements[Maxlength];
    int Top_idx;
}Stack;

void makeNull_Stack (Stack *stack){
    stack->Top_idx = Maxlength;
}

int empty_Stack (Stack stack){
    return stack.Top_idx == Maxlength;
}

int full_stack (Stack stack){
    return stack.Top_idx == 0;
}

Node *top (Stack stack){
    if(!empty_Stack(stack)){
        return stack.Elements[stack.Top_idx];
    }
    return NULL;
}

void pop (Stack *stack){
    if(!empty_Stack(*stack)){
        stack->Top_idx += 1;
    }
    else {
        printf ("Erorr! Stack is empty");
    }
}

//Dua 1 phan tu len ngan xep
void push (Node *x, Stack *stack){
    if (full_stack (*stack)){
        printf ("Erorr! Stack is full");
    }
    else{
        stack->Top_idx -= 1;
        stack->Elements[stack->Top_idx] = x;
    }
}

int compareStates (State a, State b){
    return (a.X == b.X && a.Y == b.Y);
}


/*---------------------------------------------------------------------------------------*/
//Khai bao cau truc Queue de luu trang thai duyet
typedef struct {
    Node *Elements[Maxlength];
    int front, rear;
}Queue;

void makeNull_Queue (Queue *queue){
    queue->front = -1;
    queue->rear = -1;
}

int empty_queue (Queue queue){
    return queue.front == -1;
}

int full_Queue (Queue queue){
    return ((queue.rear - queue.front +1)%Maxlength) == 0;
}

Node *get_front (Queue queue){
    if(empty_queue(queue)){
        printf("Queue is empty");
    }
    else {
        return queue.Elements [queue.front];
    }
}

void del_Queue (Queue *queue){
    if (!empty_queue (*queue)){
        if (queue->front == queue->rear)
            makeNull_Queue (queue);
        else 
            queue->front = (queue->front +1 )%Maxlength;
    }
    else printf ("Error, Delete");
}

void push_Queue (Node *x, Queue *queue){
    if (!full_Queue (*queue)){
        if (empty_queue (*queue)){
            queue->front = 0;
        }
        queue->rear = (queue->rear +1)%Maxlength;
        queue->Elements[queue->rear] = x;
    }
    else printf ("Error, Push");
}

int find_State (State state, Queue openQueue){
    while (!empty_queue(openQueue)){
        if(compareStates(get_front(openQueue)->state, state))
            return 1;
        del_Queue(&openQueue);
    }
    return 0;
}

//Thuat toan tim theo chieu sau
Node *BFS_Algorithm (State state){
    //khai bao ngan xep Open/close 
    Queue Open_BFS;
    Queue Close_BFS;
    makeNull_Queue (&Open_BFS);
    makeNull_Queue (&Close_BFS);
    //tao nut trang thai cha
    Node *root = (Node*) malloc (sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    push_Queue(root, &Open_BFS);

    while (!empty_queue(Open_BFS)){
        //lay mot dinh trong ngan xep
        Node *node = get_front(Open_BFS);
        del_Queue(&Open_BFS);
        push_Queue (node, &Close_BFS);
        //kiem tra 1 dinh lay ra co phai trang thai muc tieu?
        if (goalcheck(node->state)){
            return node;
        }
        int opt;
        //goi cac phep toan trang thai
        for(opt = 1; opt <= 6; opt++){
            State newstate;
            makeNullState(&newstate);
            if (call_operater(node->state, &newstate, opt)){
                //neu trang thai moi da ton tai thi bo qua
                if (find_State (newstate, Close_BFS) || find_State(newstate, Open_BFS)){
                    continue;
                }
                Node *newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->parent = node;
                newNode->no_function = opt;
                push_Queue(newNode, &Open_BFS);
            }
        }
    }
    return NULL;
}

//in ket qua chuyen nuoc de dat den muc tieu
void print_WayToGetGoal (Node *node){
    Stack stackPrint;
    makeNull_Stack (&stackPrint);
    //duyet nguoc ve parent 
    while (node->parent != NULL){
        push (node, &stackPrint);
        node = node->parent;
    }
    push (node, &stackPrint);
    //in ra thu tu hanh dong chuyen nuoc
    int no_action = 0;
    while (!empty_Stack(stackPrint)){
        printf("\nAction %d: %s", no_action, action[top(stackPrint)->no_function]);
        print_state (top(stackPrint)->state);
        pop(&stackPrint);
        no_action++;
    }
}


int main () {
    // State cur_state = {5, 4}, result;
    // printf ("Trang thai bat dau");
    // print_state(cur_state);
    // int otp;
    // for (otp = 1; otp <= 6; otp ++){
    //     int thuchien = call_operater (cur_state, &result, otp);
    //     if (thuchien ==1 ){
    //         printf ("\nHanh dong %s thanh cong", action[otp]);
    //         print_state (result);
    //     }
    //     else {
    //         printf("\nHanh dong %s KHONG thanh cong", action[otp]);
    //     }
    // }

    State cur_state = {0, 0};
    Node *p = BFS_Algorithm(cur_state);
    print_WayToGetGoal(p); 
    return 0;
}