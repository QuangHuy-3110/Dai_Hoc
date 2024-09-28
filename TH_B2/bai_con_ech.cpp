#include <stdio.h>
#include <iostream>
#include <vector>
#include <malloc.h>
#include <algorithm>

#define no_rock 7
#define max_jump 2

/*khai bao trang thai*/
typedef struct {
    int name;
    int position;
    int h;
}frog;

frog create_empty_rock (){
    frog empty_rock;
    empty_rock.name = 0;
    empty_rock.position = (no_rock)/2;
    empty_rock.h = 0;
    return empty_rock;
}

frog empty_rock = create_empty_rock();


typedef struct {
    frog map[no_rock];
    int pos_empty_rock;
}State;

void init_State (State *state){
    int name = -1 * (no_rock-1)/2;
    for (int i = 0; i < no_rock; i++){     
        frog tmp;
        tmp.name = name;
        tmp.position = i;
        tmp.h = (no_rock+1)/2;
        state->pos_empty_rock = (no_rock)/2;
        if (i == (no_rock)/2){
            state->map[i] = empty_rock;
        }
        else {
            state->map[i] = tmp;
        }
        name++;
    }
    
}

/*cac hanh dong */
int jump_A_to_B_1_step (State cur_state, frog frog_inCur, State *result){
    if (frog_inCur.position+1 != cur_state.pos_empty_rock || frog_inCur.position+1 >= no_rock){
        return 0;
    }
    *result = cur_state;
    std::swap(result->map[frog_inCur.position], result->map[cur_state.pos_empty_rock]);
    //cap nhat lai vi tri cuc da trong
    result->pos_empty_rock = frog_inCur.position;
    //cap nhat lai h
    result->map[cur_state.pos_empty_rock].h -=1;
    //thay doi vitri cho empty rock va frog current
    result->map[result->pos_empty_rock].position = result->pos_empty_rock;
    result->map[cur_state.pos_empty_rock].position = cur_state.pos_empty_rock;
    return 1;
}

int jump_A_to_B_2_step(State cur_state, frog frog_inCur, State *result) {
    // Kiểm tra điều kiện nhảy 2 bước sang phải
    if (frog_inCur.position + 2 != cur_state.pos_empty_rock || frog_inCur.position + 2 >= no_rock) {
        return 0; // Không hợp lệ, trả về thất bại
    }
    *result = cur_state;
    std::swap(result->map[frog_inCur.position], result->map[cur_state.pos_empty_rock]);
    // Cập nhật vị trí của tảng đá trống
    result->pos_empty_rock = frog_inCur.position;
    // Cập nhật h (số bước nhảy đã thực hiện)
    result->map[cur_state.pos_empty_rock].h -= 2;
    // Cập nhật vị trí cho con ếch đã nhảy
    result->map[result->pos_empty_rock].position = result->pos_empty_rock;
    result->map[cur_state.pos_empty_rock].position = cur_state.pos_empty_rock;
    return 1;
}


int jump_B_to_A_1_step (State cur_state, frog frog_inCur, State *result){
    if (frog_inCur.position-1 != cur_state.pos_empty_rock || frog_inCur.position-1 < 0){
        return 0;
    }
    *result = cur_state;
    std::swap(result->map[frog_inCur.position], result->map[cur_state.pos_empty_rock]);
    //cap nhat lai vi tri cuc da trong
    result->pos_empty_rock = frog_inCur.position;
    //cap nhat lai h
    result->map[cur_state.pos_empty_rock].h -= 1;
    //thay doi vitri cho empty rock
    result->map[result->pos_empty_rock].position = result->pos_empty_rock;
    result->map[cur_state.pos_empty_rock].position = cur_state.pos_empty_rock;
    return 1;
}

int jump_B_to_A_2_step (State cur_state, frog frog_inCur, State *result){
    if (frog_inCur.position-2 != cur_state.pos_empty_rock || frog_inCur.position-2 < 0){
        return 0;
    }
    *result = cur_state;
    std::swap(result->map[frog_inCur.position], result->map[cur_state.pos_empty_rock]);
    //cap nhat lai vi tri cuc da trong
    result->pos_empty_rock = frog_inCur.position;
    //cap nhat lai h
    result->map[cur_state.pos_empty_rock].h -= 2;
    //thay doi vitri cho empty rock
    result->map[result->pos_empty_rock].position = result->pos_empty_rock;
    result->map[cur_state.pos_empty_rock].position = cur_state.pos_empty_rock;
    return 1;
}

const char* actions [] = {"First State", "Jump to right one step", "Jump to right two step",
                        "Jump to left one step", "Jump to left two step"};

int callOperators (State cur_state, State *result, int pos_frog_A, int option){
    switch (option)
    {
    case 1: return jump_A_to_B_1_step(cur_state, cur_state.map[pos_frog_A], result);
    case 2: return jump_A_to_B_2_step(cur_state, cur_state.map[pos_frog_A], result);
    case 3: return jump_B_to_A_1_step(cur_state, cur_state.map[pos_frog_A], result);
    case 4: return jump_B_to_A_2_step(cur_state, cur_state.map[pos_frog_A], result);
    default:
        printf ("Error calls operators");
        return 0;
    }
}

//so sanh trang thai
int compareState (State state1, State state2)
{
    for (int i = 0; i < no_rock; i++){
        if (state1.map[i].h != state2.map[i].h){
            return 0;
        }

        if (state1.map[i].name != state2.map[i].name){
            return 0;
        }

        if (state1.map[i].position != state2.map[i].position){
            return 0;
        }
        
    }
    
    return 1;
}

/*----------------------------------------------------------------------------*/
//check goal
int check_goal (State state, State goal)
{
    return compareState(state, goal);
}

void printState (State state)
{
    printf("------------------------------\n");
    for (int i= 0; i < no_rock; i++)
    {
        printf("%d  ", state.map[i].name);
    }
    printf("\n------------------------------\n");
}

int main()
{
    State state, result;
    init_State(&state);
    init_State(&result);

    printf("Trang thai bat dau:\n");
    printState(state);

    // Vòng lặp qua từng con ếch trong trạng thái hiện tại
    for (int pos = 0; pos < no_rock; pos++) {
        printf("Con ech ten %d:\n", state.map[pos].name);
        
        // Vòng lặp qua các tùy chọn nhảy
        for (int opt = 1; opt <= 4; opt++) {
            // Gọi hàm để thực hiện hành động
            if (callOperators(state, &result, pos, opt)) {
                printf("Hanh dong %d %s thanh cong:\n", state.map[pos].name, actions[opt]);
                printState(result); // In ra trạng thái mới
            } else {
                printf("Hanh dong %d %s khong thanh cong!\n", state.map[pos].name, actions[opt]);
            }
        }
    }

    return 0;
}
