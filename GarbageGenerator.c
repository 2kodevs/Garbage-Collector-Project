#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define true 1
#define false 0
#define next (rand() % 15)

char ascii[10], cur[1];
int fd;

//Convert a number to string and put it in the garbage file
void NumWriter(int num){
    cur[0] = ascii[1];
    if(num >= 10)
        write(fd, cur, 1);
    cur[0] = ascii[num % 10];
    write(fd, cur, 1);
}

//Write a character in the garbage file
void WriteSep(char sep){
    cur[0] = sep;
    write(fd, cur, 1);
}

//Generate two random numbers and put them
// in the garbage file with a comma between them 
void MakePair(){
    NumWriter(next);
    WriteSep(',');
    NumWriter(next);
}

//Generate the garbage file data
void GarbageGenerator(const char **argv){
    fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0664);
    srand(time(NULL));
    for(int i = 0; i < 100; ++i){
        MakePair();
        for(int j = 1; j < 5; ++j){
            WriteSep(' ');
            MakePair();
        }
        WriteSep('\n');
    }
    close(fd);
}

//Create a mapping list to convert int to char
void InitializeAll(){
    ascii[0] = 48;
    for(int i = 1; i < 10; ++i)
        ascii[i] = ascii[i - 1] + 1;
}

int main(int arg, const char **argv){
    InitializeAll();
    GarbageGenerator(argv);
    return 0;
}