#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

typedef struct _Position _sPos;

//Movements arrays
const int _xAxis[4] = {-1, 0, 1, 0};
const int _yAxis[4] = {0, 1, 0, -1};

int _g = 0;
int _turn = -1;

struct _Position{
    int x;
    int y;
};

struct _Node {
	_sPos vertex;
	struct _Node* next;
};

struct _Zone {
    int zoneNum;
    _sPos cornerSupLeft;
    _sPos cornerSupRight;
    _sPos cornerInfLeft;
    _sPos cornerInfRight;
};

struct _PairZN{
    int count;
    struct _Zone zone;
};

struct _PairZN z = {0};

struct _Zone _BiggestZone = {
    0,
    {0, 0}, {0, 14},
    {14, 0}, {14, 14}
};

struct _Zone _BigZone1 = {
    1,
    {0, 0}, {0, 7},
    {7, 0}, {7, 7}
};

struct _Zone _supLeftZ1 = {
    11,
    {0, 0}, {0, 4},
    {4, 0}, {4, 4}
};

struct _Zone _supRightZ1 = {
    12,
    {0, 3}, {0, 7},
    {4, 3}, {4, 7}
};

struct _Zone _InfLeftZ1 = {
    13,
    {3, 0}, {3, 4},
    {7, 0}, {7, 4}
};

struct _Zone _InfRightZ1 = {
    14,
    {3, 3}, {3, 7},
    {7, 3}, {7, 7}
};

struct _Zone _CenterZ1 = {
    15,
    {2, 2}, {2, 6},
    {6, 2}, {6, 6}
};

struct _Zone _BigZone2 = {
    2,
    {0, 8}, {0, 14},
    {7, 8}, {7, 14}
};

struct _Zone _supLeftZ2 = {
    21,
    {0, 8}, {0, 12},
    {4, 8}, {4, 12}
};

struct _Zone _supRightZ2 = {
    22,
    {0, 10}, {0, 14},
    {4, 10}, {4, 14}
};

struct _Zone _InfLeftZ2 = {
    23,
    {3, 8}, {3, 12},
    {7, 8}, {7, 12}
};

struct _Zone _InfRightZ2 = {
    24,
    {3, 10}, {3, 14},
    {7, 10}, {7, 14}
};

struct _Zone _CenterZ2 = {
    25,
    {1, 9}, {1, 13},
    {5, 9}, {5, 13}
};

struct _Zone _BigZone3 = {
    3,
    {8, 0}, {8, 8},
    {14, 0}, {14, 7}
};

struct _Zone _supLeftZ3 = {
    31,
    {8, 0}, {8, 4},
    {12, 0}, {12, 4}
};

struct _Zone _supRightZ3 = {
    32,
    {8, 3}, {8, 7},
    {12, 3}, {12, 7}
};

struct _Zone _InfLeftZ3 = {
    33,
    {10, 0}, {10, 4},
    {14, 0}, {14, 4}
};

struct _Zone _InfRightZ3 = {
    34,
    {10, 3}, {10, 7},
    {14, 3}, {14, 7}
};

struct _Zone _CenterZ3 = {
    35,
    {9, 2}, {9, 6},
    {13, 2}, {13, 6}
};

struct _Zone _BigZone4 = {
    4,
    {8, 8}, {8, 14},
    {14, 8}, {14, 14}
};

struct _Zone _supLeftZ4 = {
    41,
    {8, 8}, {8, 12},
    {12, 8}, {12, 12}
};

struct _Zone _supRightZ4 = {
    42,
    {8, 10}, {8, 14},
    {12, 10}, {12, 14}
};

struct _Zone _InfLeftZ4 = {
    43,
    {10, 8}, {10, 12},
    {14, 8}, {14, 12}
};

struct _Zone _InfRightZ4 = {
    44,
    {10, 10}, {10, 14},
    {14, 10}, {14, 14}
};

struct _Zone _CenterZ4 = {
    45,
    {9, 9}, {9, 13},
    {13, 9}, {13, 13}
};

//check if actualPos is within the specified zone
int IsInZone(_sPos actualPos, struct _Zone desiredZone){
    if (actualPos.x >= desiredZone.cornerSupLeft.x &&
        actualPos.x <= desiredZone.cornerInfLeft.x &&
        actualPos.y >= desiredZone.cornerSupLeft.y &&
        actualPos.y <= desiredZone.cornerSupRight.y)
        return 1;
    return 0;
}

//give the bigZone that contains the robot
struct _Zone PosZone(_sPos pos){
     //robot is in half superior
    if (pos.x < _BigZone3.cornerSupLeft.x){
            //robot is in zone 1
            if (pos.y < _BigZone2.cornerSupLeft.y){
                return _BigZone1;
            }
            //robot is in zone 2
            else
                return _BigZone2;  
    }
    //robot is in half inferior
    else{
        //robot is in zone 3
        if (pos.y < _BigZone2.cornerSupLeft.y)
            return _BigZone3;  
        //robot is in zone 4
        else
            return _BigZone4;       
    }
}

int isGarbage(_sPos p, char **world){
    return world[p.x][p.y] == 'o';
}

int isInZone(_sPos p, char **world){
    return IsInZone(p, z.zone);
}

//Check if is posible to move to the given direction
//1 if is possible, 0 otherwise
int CheckMove(int**visited, int dir, _sPos pos, struct _Zone zone, int firstCmp){
    int vectorX = pos.x + _xAxis[dir];
    int vectorY = pos.y + _yAxis[dir];
    if (!(vectorX >= zone.cornerSupLeft.x && vectorX <= zone.cornerInfLeft.x 
        && vectorY >= zone.cornerSupLeft.y && vectorY <= zone.cornerSupRight.y))
        return 0;

    if (firstCmp)
        return 1;
    if(!visited[vectorX][vectorY]){
        visited[vectorX][vectorY] = 1;
        return 1;
    }
    return 0;
}

//decide the direction of the next movement 
//using the closest garbage
int CalculateDir(_sPos robot, _sPos garbage){
    if (robot.x > garbage.x)
        return 0;
    if (robot.y < garbage.y)
        return 1;
    if (robot.x < garbage.x)
        return 2;
    return 3;
}

// breadth first search
int do_bfs(_sPos node, int **visited, char **world, int update, struct _Zone zone, int (*f)(_sPos, char **)) {

	struct _Node *initialQueue, *front, *rear, *temp;
    _sPos p;
    int count = 0;
	visited[node.x][node.y] = 1;
	// initialize a queue, and initialize with vertex.
	initialQueue = (struct _Node*)malloc(sizeof(struct _Node));
	initialQueue->vertex = node;
	initialQueue->next = 0;
	front = initialQueue;
	rear  = initialQueue;
	// while queue is not empty
	while (front != 0) {
        ++count;
		p = front->vertex;
        
        for (int k = 0; k < 4; ++k){
            _sPos temp = {node.x + _xAxis[k], node.y + _yAxis[k]};
            if (CheckMove(visited, k, node, zone, 1) && isGarbage(temp, world)){
                if (isInZone(temp, world))
                    z.count -= 1;
                return CalculateDir(node, temp);
            }
        }
        
                
        for (int i = 0; i < 4; ++i){
            if (CheckMove(visited, i, p, zone, 0))
            {
                _sPos u = {p.x + _xAxis[i], p.y + _yAxis[i]};
                
                if (f(u, world)){
                    if(count == 1)
                        z.count -= update;
                    return CalculateDir(node, u);
                }
                
                struct _Node *nextQueue = malloc(sizeof(struct _Node));
				nextQueue->vertex = u;
				nextQueue->next = 0;
                rear->next = nextQueue;
				rear = nextQueue;
            }
        }
		// remove front vertex from queue
		temp = front;
		front = front->next;
		free (temp);
	}
    return 0;
}

int bfs(_sPos robot, char**world, int update, struct _Zone zone, int(*f)(_sPos, char **)){
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
	return do_bfs(robot, visited, world, update, zone, f);
}

//Return the position of the robot in the world
_sPos LocateRobot(char**world){
    for (int i = 0; i < 15; ++i){
        for(int j = 0; j < 15; ++j){
            if (world[i][j] == 'x'){
                _sPos robot = {i, j};
                return robot;
            }
        }
    }
}

//calculate the density of the subzones of bigZone 1
struct _PairZN calDenZ1(char**world){
    int max = 0, _g = 0;
    struct _Zone best = _supLeftZ1;

    for(int i = _supLeftZ1.cornerSupLeft.x; i <= _supLeftZ1.cornerInfLeft.x; ++i){
        for(int j = _supLeftZ1.cornerSupLeft.y; j <= _supLeftZ1.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;
        }
    }
    max = _g;
    _g = 0;
    
    for(int i = _supRightZ1.cornerSupLeft.x; i <= _supRightZ1.cornerInfLeft.x; ++i){
        for(int j = _supRightZ1.cornerSupLeft.y; j <= _supRightZ1.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;
        }
    }
    if (_g > max){
        max = _g;
        best = _supRightZ1;
    }
    _g = 0;

    for(int i = _InfLeftZ1.cornerSupLeft.x; i <= _InfLeftZ1.cornerInfLeft.x; ++i){
        for(int j = _InfLeftZ1.cornerSupLeft.y; j <= _InfLeftZ1.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;
        }
    }
    if (_g > max){
        max = _g;
        best = _InfLeftZ1;
    }
    _g = 0;

    for(int i = _InfRightZ1.cornerSupLeft.x; i <= _InfRightZ1.cornerInfLeft.x; ++i){
        for(int j = _InfRightZ1.cornerSupLeft.y; j <= _InfRightZ1.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;
        }
    }
    if (_g > max){
        max = _g;
        best = _InfRightZ1;
    }
    _g = 0;


    for(int i = _CenterZ1.cornerSupLeft.x; i <= _CenterZ1.cornerInfLeft.x; ++i){
        for(int j = _CenterZ1.cornerSupLeft.y; j <= _CenterZ1.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;
        }
    }
    if (_g > max){
        max = _g;
        best = _CenterZ1;
    }
    struct _PairZN ret = {max, best};
    return ret;
}

//calculate the density of the subzones of bigZone 2
struct _PairZN calDenZ2(char**world){   
    int max = 0, _g = 0;
    struct _Zone best = _supLeftZ2;

    for(int i = _supLeftZ2.cornerSupLeft.x; i <= _supLeftZ2.cornerInfLeft.x; ++i){
        for(int j = _supLeftZ2.cornerSupLeft.y; j <= _supLeftZ2.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    max = _g;
    _g = 0;
    
    for(int i = _supRightZ2.cornerSupLeft.x; i <= _supRightZ2.cornerInfLeft.x; ++i){
        for(int j = _supRightZ2.cornerSupLeft.y; j <= _supRightZ2.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    if (_g > max){
        max = _g;
        best = _supRightZ2;
    }
    _g = 0;

    for(int i = _InfLeftZ2.cornerSupLeft.x; i <= _InfLeftZ2.cornerInfLeft.x; ++i){
        for(int j = _InfLeftZ2.cornerSupLeft.y; j <= _InfLeftZ2.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    if (_g > max){
        max = _g;
        best = _InfLeftZ2;
    }
    _g = 0;

    for(int i = _InfRightZ2.cornerSupLeft.x; i <= _InfRightZ2.cornerInfLeft.x; ++i){
        for(int j = _InfRightZ2.cornerSupLeft.y; j <= _InfRightZ2.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    if (_g > max){
        max = _g;
        best = _InfRightZ2;
    }
    _g = 0;


    for(int i = _CenterZ2.cornerSupLeft.x; i <= _CenterZ2.cornerInfLeft.x; ++i){
        for(int j = _CenterZ2.cornerSupLeft.y; j <= _CenterZ2.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    if (_g > max){
        max = _g;
        best = _CenterZ2;
    }
    struct _PairZN ret = {max, best};
    return ret;
}

//calculate the density of the subzones of bigZone 3
struct _PairZN calDenZ3(char**world){
    int max = 0, _g = 0;
    struct _Zone best = _supLeftZ3;

    for(int i = _supLeftZ3.cornerSupLeft.x; i <= _supLeftZ3.cornerInfLeft.x; ++i){
        for(int j = _supLeftZ3.cornerSupLeft.y; j <= _supLeftZ3.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    max = _g;
    _g = 0;
    
    for(int i = _supRightZ3.cornerSupLeft.x; i <= _supRightZ3.cornerInfLeft.x; ++i){
        for(int j = _supRightZ3.cornerSupLeft.y; j <= _supRightZ3.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    if (_g > max){
        max = _g;
        best = _supRightZ3;
    }
    _g = 0;

    for(int i = _InfLeftZ3.cornerSupLeft.x; i <= _InfLeftZ3.cornerInfLeft.x; ++i){
        for(int j = _InfLeftZ3.cornerSupLeft.y; j <= _InfLeftZ3.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    if (_g > max){
        max = _g;
        best = _InfLeftZ3;
    }
    _g = 0;

    for(int i = _InfRightZ3.cornerSupLeft.x; i <= _InfRightZ3.cornerInfLeft.x; ++i){
        for(int j = _InfRightZ3.cornerSupLeft.y; j <= _InfRightZ3.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    if (_g > max){
        max = _g;
        best = _InfRightZ2;
    }
    _g = 0;


    for(int i = _CenterZ3.cornerSupLeft.x; i <= _CenterZ3.cornerInfLeft.x; ++i){
        for(int j = _CenterZ3.cornerSupLeft.y; j <= _CenterZ3.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    if (_g > max){
        max = _g;
        best = _CenterZ3;
    }
    struct _PairZN ret = {max, best};
    return ret;
}

//calculate the density of the subzones of bigZone 4
struct _PairZN calDenZ4(char**world){
    int max = 0, _g = 0;
    struct _Zone best = _supLeftZ4;

    for(int i = _supLeftZ4.cornerSupLeft.x; i <= _supLeftZ4.cornerInfLeft.x; ++i){
        for(int j = _supLeftZ4.cornerSupLeft.y; j <= _supLeftZ4.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    max = _g;
    _g = 0;
    
    for(int i = _supRightZ4.cornerSupLeft.x; i <= _supRightZ4.cornerInfLeft.x; ++i){
        for(int j = _supRightZ4.cornerSupLeft.y; j <= _supRightZ4.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    if (_g > max){
        max = _g;
        best = _supRightZ4;
    }
    _g = 0;

    for(int i = _InfLeftZ4.cornerSupLeft.x; i <= _InfLeftZ4.cornerInfLeft.x; ++i){
        for(int j = _InfLeftZ4.cornerSupLeft.y; j <= _InfLeftZ4.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    if (_g > max){
        max = _g;
        best = _InfLeftZ4;
    }
    _g = 0;

    for(int i = _InfRightZ4.cornerSupLeft.x; i <= _InfRightZ4.cornerInfLeft.x; ++i){
        for(int j = _InfRightZ4.cornerSupLeft.y; j <= _InfRightZ4.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    if (_g > max){
        max = _g;
        best = _InfRightZ4;
    }
    _g = 0;


    for(int i = _CenterZ4.cornerSupLeft.x; i <= _CenterZ4.cornerInfLeft.x; ++i){
        for(int j = _CenterZ4.cornerSupLeft.y; j <= _CenterZ4.cornerSupRight.y; ++j){
            if (world[i][j] == 'o')
                ++_g;;
        }
    }
    if (_g > max){
        max = _g;
        best = _CenterZ4;
    }
    struct _PairZN ret = {max, best};
    return ret;
}


//return the more dense zone and the total
//of garbage in it
struct _PairZN CalculateDensity(_sPos robot, char**world){
    struct _PairZN best, temp;
    struct _Zone actual = PosZone(robot);
    //calculate subzones 1
    if (actual.zoneNum == 1){
        best = calDenZ1(world);
        temp = calDenZ2(world);
        if (best.count < temp.count && temp.count > z.count)
            best = temp;
        temp = calDenZ3(world);
        if (best.count < temp.count && temp.count > z.count)
            best = temp;
    }
    //calculate subzones 2
    else if (actual.zoneNum == 2){
        best = calDenZ2(world);
        temp = calDenZ1(world);
        if (best.count < temp.count && temp.count > z.count)
            best = temp;
            temp = calDenZ4(world);
            if (best.count < temp.count && temp.count > z.count)
                best = temp;
    }            
    //calculate subzones 3
    else if (actual.zoneNum == 3){
        best = calDenZ3(world);
        temp = calDenZ1(world);
        if (best.count < temp.count && temp.count > z.count)
            best = temp;
        temp = calDenZ4(world);
        if (best.count < temp.count && temp.count > z.count)
            best = temp;
        }
            
    //calculate subzones 4
    else{
        best = calDenZ4(world);
        temp = calDenZ2(world);
        if (best.count < temp.count && temp.count > z.count)
            best = temp;
        temp = calDenZ3(world);
        if (best.count < temp.count && temp.count > z.count)
            best = temp;
    }        

    return best;
}


int fdm(char**world){
    ++_turn;
    _sPos robot = LocateRobot(world);
    if(z.count == 0 || _turn % 10 == 0){
        z = CalculateDensity(robot, world);
    }
    
    //printf("zone: %d---density:%d\n", z.zone.zoneNum, z.count);
    if(!IsInZone(robot, z.zone)){
        return bfs(robot, world, 0, _BiggestZone, isGarbage);
    }   
    if (z.count == 0){
        z = CalculateDensity(robot, world);
        return bfs(robot, world, 0, z.zone, isInZone);
    }
    return bfs(robot, world, 1, z.zone, isGarbage);
}
