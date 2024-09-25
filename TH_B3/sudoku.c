#include <stdio.h>
#include <stdlib.h>

#define Maxlenght 99999
#define NB_ROWS 9
#define NB_COLUMNS 9
#define MAX_VALUE 10
#define EMPTY 0
#define AREA_SQUARE_SIZE 3
#define INF 999999

/*---------------------------------------------------------------------------*/
//khai bao cau truc danh sach
typedef struct 
{
    int Elements[Maxlenght];
    int size;
}List;

//khoi tao danh sach rong
void initList (List *list)
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

//them phan tu vao vi tri position trong danh sach
void appendList (List *list, int x)
{
    if (!full_List(*list))
    {
        list->Elements[list->size] = x;
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

/*---------------------------------------------------------------------------*/
//cau truc danh sach toa do
typedef struct 
{
    int x, y;
}Coord;
// x va y co gia tri tu 0 -> 8

typedef struct 
{
    Coord data [Maxlenght];
    int size;
}ListCoord;

void initListCoord (ListCoord *list)
{
    list->size = 0;
}

void appendListCoord (ListCoord *list, Coord coord)
{
    list->data[list->size++] = coord;
}

/*---------------------------------------------------------------------------*/
//khoi tao rang buoc 
typedef struct 
{
    int data [NB_ROWS*NB_COLUMNS][NB_ROWS*NB_COLUMNS];
    int n;
}Constrains;

//khoi tao rang buoc rong
void initConstrains (Constrains *constrains)
{
    for (int i = 0; i < NB_ROWS*NB_COLUMNS; i++)
    {
        for (int j = 0; j < NB_ROWS*NB_COLUMNS; j++)
        {
            constrains->data[i][j] = 0;
        }        
    }
    constrains->n = NB_ROWS*NB_COLUMNS;    
}

/*---------------------------------------------------------------------------*/
//tim chi so dinh khi biet toa do
int indexOf (Coord coord)
{
    return (NB_ROWS*coord.x + coord.y);
}

//tim toa do khi biet chi so dinh
Coord positionOfVertex (int vertex)
{
    Coord coord;
    coord.x = vertex / NB_ROWS;
    coord.y = vertex % NB_COLUMNS;
    return coord;
}

//them rang buoc
int addConstrain (Constrains *constrain, Coord source, Coord target)
{
    int u = indexOf(source);
    int v = indexOf(target);
    if (constrain->data[u][v] == 0)
    {
        constrain->data[u][v] = 1;
        constrain->data[v][u] = 1;
        return 1;
    }
    return 0;
}

//tap rang buoc cua 1 dinh
ListCoord getConsrtains (Constrains constrains, Coord coord)
{
    int v = indexOf(coord);
    ListCoord result;
    initListCoord(&result);
    for (int i = 0; i < constrains.n; i++)
    {
        if (constrains.data[v][i] == 1)
        {
            appendListCoord(&result, positionOfVertex(i));
        }
        
    }
    return result;
}

/*---------------------------------------------------------------------------*/
//cau truc bang sudoku
typedef struct 
{
    int cells [NB_ROWS][NB_COLUMNS];
    Constrains constrains;
}Sudoku;

//khoi tao bang rong
void initSudoku (Sudoku *sudoku)
{
    for (int i = 0; i < NB_ROWS; i++)
    {
        for (int j = 0; j < NB_COLUMNS; j++)
        {
            sudoku->cells[i][j] = EMPTY;
        }
        
    }
    initConstrains(&sudoku->constrains);
}

//khoi tao bang sudoku co gia tri
void initSudokuWithValue (Sudoku *sudoku, int input[NB_ROWS][NB_COLUMNS])
{
    for (int i = 0; i < NB_ROWS; i++)
    {
        for (int j = 0; j < NB_COLUMNS; j++)
        {
            sudoku->cells[i][j] = input[i][j];
        }
        
    }
    initConstrains(&sudoku->constrains);
}

//in bang sudoku
void printSudoku (Sudoku sudoku)
{
    printf("Sudoku:\n");
    for (int i = 0; i < NB_ROWS; i++)
    {
        if (i % AREA_SQUARE_SIZE == 0)
        {
            printf("-------------------------\n");
        }
        for (int j = 0; j < NB_COLUMNS; j++)
        {
            if (j % AREA_SQUARE_SIZE == 0)
            {
                printf("| ");
            }
            printf("%d ", sudoku.cells[i][j]);
        }
        printf("|\n") ;                
    }
    printf("-------------------------\n");
}

//kiem tra trang thai ket thuc
int isFilledSudoku (Sudoku sudoku)
{
    for (int i = 0; i < NB_ROWS; i++)
    {
        for (int j = 0; j < NB_COLUMNS; j++)
        {
            if (sudoku.cells[i][j] == EMPTY)
            {
                return 0;
            }
            
        }
        
    }
    return 1;
}

/*---------------------------------------------------------------------------*/
//tao rang buoc tu vi tri cho truoc

void spreadConstrainsFrom (Coord postision, Constrains *constrain, ListCoord *changed)
{
    int row = postision.x;
    int colmn = postision.y;
    //tao rang buoc theo cot
    for (int i = 0; i < NB_ROWS; i++)
    {
        if (i != row)
        {
            Coord pos = {i, colmn};
            if (addConstrain(constrain, postision, pos))
            {
                appendListCoord(changed, pos);
            }            
        }        
    }

    //tao rang buoc theo hang
    for (int i = 0; i < NB_COLUMNS; i++)
    {
        if (i != colmn)
        {
            Coord pos = {row, i};
            if (addConstrain(constrain, postision, pos))
            {
                appendListCoord(changed, pos);
            }                        
        }        
    }

    //truyen rang buoc theo khoi
    for (int i = 0; i < AREA_SQUARE_SIZE; i++)
    {
        for (int j = 0; j < AREA_SQUARE_SIZE; j++)
        {
            int areaX = (row/AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
            int areaY = (colmn/AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
            if (areaX + i != row || areaY + j != colmn)
            {
                Coord pos = {areaX+i, areaY+j};
                if (addConstrain(constrain, postision, pos))
                {
                    appendListCoord (changed, pos);
                }                
            }            
        }        
    }          
}

//tim mien gia tri cua o trong
List getAvailableValues (Coord position, Sudoku sudoku)
{
    ListCoord posList = getConsrtains(sudoku.constrains, position);
    int availables [MAX_VALUE]; //0->9 array
    for (int i = 1; i < MAX_VALUE; i++)
    {
        availables[i] = 1;
    }
    for (int i = 0; i < posList.size; i++)
    {
        Coord pos = posList.data[i];
        if (sudoku.cells[pos.x][pos.y] != EMPTY)
        {
            availables[sudoku.cells[pos.x][pos.y]] = 0;
        }        
    }
    List result;
    initList(&result);
    for (int i = 1; i < MAX_VALUE; i++)
    {
        if (availables[i])
        {
            appendList(&result, i);
        }        
    }
    return result;
}

/*---------------------------------------------------------------------------*/
//Xac dinh o nao uu tien thi dien truoc
//cach 1: do lan luot o nao trong thi chon
Coord getNextEmptyCell (Sudoku sudoku)
{
    for (int i = 0; i < NB_ROWS; i++)
    {
        for (int j = 0; j < NB_COLUMNS; j++)
        {
            Coord pos = {i, j};
            if (sudoku.cells[i][j] == EMPTY)
            {
                return pos;
            }            
        }        
    }    
}

//cach 2: uu tien o co mien gia tri it nhat
Coord getMinDomainCell (Sudoku sudoku)
{
    int minLength = INF;
    Coord result;
    for (int i = 0; i < NB_ROWS; i++)
    {
        for (int j = 0; j < NB_COLUMNS; j++)
        {
            if (sudoku.cells[i][j] == EMPTY)
            {
                Coord pos = {i, j};
                int availableLenght = getAvailableValues(pos, sudoku).size;
                if (availableLenght < minLength)
                {
                    minLength = availableLenght;
                    result = pos;
                }
                
            }
            
        }
        
    }
    return result;  
}

//giai quyet bai toan
int exploredCounter = 0;
int sudokuBackTracking (Sudoku *sudoku)
{
    if (isFilledSudoku(*sudoku))
    {
        return 1;
    }
    Coord position = getMinDomainCell(*sudoku);
    List available = getAvailableValues(position, *sudoku);
    if (available.size == 0)
    {
        //neu nhu mien gia tri rong nhung o chua duoc dien
        return 0;
    }
    
    for (int j = 0; j < available.size; j++)
    {
        int value = available.Elements[j];
        sudoku->cells[position.x][position.y] = value;
        exploredCounter ++;
        if (sudokuBackTracking(sudoku))
        {
            return 1;
        }
        sudoku->cells[position.x][position.y] = EMPTY;        
    }
    return 0;    
}

//lan truyen tu de bai cho truc va ket thuc bai toan
Sudoku solveSudoku (Sudoku sudoku)
{
    initConstrains(&sudoku.constrains);
    for (int i = 0; i < NB_ROWS; i++)
    {
        for (int j = 0; j < NB_COLUMNS; j++)
        {
            ListCoord history;
            initListCoord(&history);
            Coord pos = {i, j};
            spreadConstrainsFrom(pos, &sudoku.constrains, &history);
        }
        
    }
    exploredCounter = 0;
    if (sudokuBackTracking(&sudoku))
    {
        printf("Solved\n");
    }
    else{
        printf("Can not solve\n");
    }
    printf("Explored %d state\n", exploredCounter);
    return sudoku; 
    
}

//bang dau vao
int input1 [9][9] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9},
};

int main()
{
    Sudoku sudoku;
    initSudokuWithValue(&sudoku, input1);
    printSudoku(sudoku);
    Sudoku result = solveSudoku(sudoku);
    printSudoku(result);
}