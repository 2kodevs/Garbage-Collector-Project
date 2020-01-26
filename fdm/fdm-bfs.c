#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

typedef struct Position sPos;



//BFS data
//sPos path[100];
//sPos* last = path;

//Movements arrays
const int xAxis[4] = {-1, 0, 1, 0};
const int yAxis[4] = {0, 1, 0, -1};

struct Position{
    int x;
    int y;
};
struct Node {
	sPos vertex;
	struct Node* next;
};


void ShowQueue(struct Node *f){
    if(f == 0){
        printf("-----\n");
        return;
    }
    printf("%d %d\n", f->vertex.x, f->vertex.y);
    ShowQueue(f->next);
}

//Check if is posible to move to the given direction
//1 if is possible, 0 otherwise
int CheckMove(int**visited, int dir, sPos pos){
    int vectorX = pos.x + xAxis[dir];
    int vectorY = pos.y + yAxis[dir];
    if (!(vectorX >= 0 && vectorX <= 14 && vectorY >= 0 && vectorY <= 14))
        return 0;
    //printf("%d %d %d\n",vectorX, vectorY, visited[vectorX][vectorY]);
    if(!visited[vectorX][vectorY]){
        visited[vectorX][vectorY] = 1;
        return 1;
    }
    return 0;
}

int CalculateDir(sPos robot, sPos garbage){
    if (robot.x > garbage.x)
        return 0;
    if (robot.y < garbage.y)
        return 1;
    if (robot.x < garbage.x)
        return 2;
    return 3;
}

//do_bfs(robot, visited, world);
// breadth first search
int do_bfs(sPos node, int **visited, char **world) {
	struct Node *initialQueue, *front, *rear, *temp;
    sPos p;
	visited[node.x][node.y] = 1;
	// initialize a queue, and initialize with vertex.
	initialQueue = (struct Node*)malloc(sizeof(struct Node));
	initialQueue->vertex = node;
	initialQueue->next = 0;
	front = initialQueue;
	rear  = initialQueue;
    //ShowQueue(front);
	// while queue is not empty
	while (front != 0) {
		p = front->vertex;
		//printf("%d %d", p->x, p->y);
        for (int i = 0; i < 4; ++i){
            if (CheckMove(visited, i, p))
            {
                sPos u = {p.x + xAxis[i], p.y + yAxis[i]};
                if (world[u.x][u.y] == 'o')
                    return CalculateDir(node, u);
               // printf("%d{%d %d} ",i, u.x, u.y);
                struct Node *nextQueue = malloc(sizeof(struct Node));
				nextQueue->vertex = u;
				nextQueue->next = 0;
                rear->next = nextQueue;
				rear = nextQueue;
            }
        }
       // printf("\n");
        //ShowQueue(front);
		// remove front vertex from queue
		temp = front;
		front = front->next;
		free (temp);
	}
    return 0;
}
int bfs(sPos robot, char**world) {
	int i;
	int **visited;
    visited = malloc(16 * sizeof(int*));
    for(int i = 0; i < 16; ++i){
        visited[i] = malloc(16 * sizeof(int));
    }
	// set all to unvisited
	for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++) {
            visited[i][j] = 0;
	}
	return do_bfs(robot, visited, world);
}

//Return the position of the robot in the world
sPos LocateRobot(char**world){
    for (int i = 0; i < 15; ++i){
        for(int j = 0; j < 15; ++j){
            if (world[i][j] == 'x'){
                sPos robot = {i, j};
                return robot;
            }
        }
    }
}


int fdm(char**world){
    sPos robot = LocateRobot(world);
    return bfs(robot, world);
}
