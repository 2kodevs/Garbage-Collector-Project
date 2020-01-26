#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/wait.h>


#define true 1
#define false 0
#define next (rand() % 15)
#define oflow(t) t > 14? 14 : t
#define uflow(t) t < 0? 0 : t

char **cl;
char cur[1];
int fd;
int (*fdm)(char**);
char **table;
int points = 0, turn;
int x = 0, y = 0;
const int cant = 10;

//Initialize the table at the start of the program
void ResetTable(){
    for(int i = 0; i < 15; ++i)
        for(int j = 0; j < 15; ++j)
            table[i][j] = '.';
    table[0][0] = 'x';
}

//Increase the total amount of point by the specifided cant
void IncPoints(){
    points += cant;
}

//Read the next number in the line of garbage that is being added
int ReadNum(){
    char num[2];
    read(fd, num, 2);
    if(num[1] < 48 || num[1] > 57){
        cur[0] = num[1];
        num[1] = 0;
    }
    else
        read(fd, cur, 1);
    return atoi(num);
}

//Get the garbage from the file garbage created
//by garbage generator, remeber to close the fd
//after the last refresh
void RefreshGarbage(){
    int pos[2], i = 0, j = 0;
    char num[2];
    cur[0] = 1;
    while(cur[0] != '\n'){
        pos[0] = ReadNum();
        pos[1] = ReadNum();
        if(pos[0] == x && pos[1] == y)
            IncPoints();
        else
            table[pos[0]][pos[1]] = 'o';
    }
}

//Given a new position of the robot refresh the table
void MoveRobot(){
    int dir = fdm(table);
    //printf("----%d-----\n", dir);
    table[x][y] = '.';
    x += (dir == 0)*(-1) + (dir == 2);
    y += (dir == 3)*(-1) + (dir == 1);
    x = uflow(oflow(x));
    y = uflow(oflow(y));
    if (table[x][y] == 'o')
        IncPoints();
    table[x][y] = 'x';
}

void ClearTerminal(){
    int id = fork(); 
    if(id){
        wait(&id);
    }
    else{
        execvp(cl[0], cl);
    }
}

//Show the table 
void Paint(){
    ClearTerminal();
    printf("coins: %d -- turn: %d\n", points, turn - 1);
    for(int i = 0; i < 15; ++i){
        printf("%s\n", table[i]);
    }
}

void InitializeAll(const char** argv){
    void* handle = dlopen(argv[2], RTLD_LAZY);
    fdm = (int(*)(char**))dlsym(handle, "fdm");
    cl = malloc(sizeof(char*));
    cl[0] = "clear";
    table = malloc(16 * sizeof(char*));
    for(int i = 0; i < 16; ++i){
        table[i] = malloc(16 * sizeof(char));
    }
    ResetTable();
    srand(time(NULL));
}

int main(int arg, const char** argv)
{
    InitializeAll(argv);
    turn = 1;
    int adv;
    fd = open(argv[1], O_RDONLY, 0664);
    RefreshGarbage();
    Paint();
    while(turn <= 1000){
        printf("%s", "Write the number of turns that you want to advance: ");
        scanf("%d", &adv);
        //printf("%d", adv);
        while(adv-- && turn <= 1000){
            MoveRobot();
            if(turn % 10 == 0 && turn != 1000)
                RefreshGarbage();
            ++turn;
        }
        Paint();
    }
    close(fd);

    return 0;
}
