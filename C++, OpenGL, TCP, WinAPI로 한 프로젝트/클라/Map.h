#pragma once
#include "global.h"

const char TYPE_MAP_START = 0;

GLvoid DrawMap();
GLvoid DrawMap_2();
GLvoid DrawDoor();

BOOL RectMap(GLfloat xPos, GLfloat zPos, int type);
BOOL CheckWall(GLfloat *xPos, GLfloat *zPos, int *type, GLfloat *yPos);
GLvoid MapAni();


