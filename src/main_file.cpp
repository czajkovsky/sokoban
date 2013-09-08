#include <GL/glew.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "tga.h"
#include "cube.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define up 1
#define down 2
#define left 3
#define right 4
#define x_axis 0
#define y_axis 1

int directions[4][4][2];
void defineDirections() {
  directions[0][up][x_axis]=0;
  directions[0][up][y_axis]=1;
  directions[0][left][x_axis]=1;
  directions[0][left][y_axis]=0;

  directions[1][up][x_axis]=1;
  directions[1][up][y_axis]=0;
  directions[1][left][x_axis]=0;
  directions[1][left][y_axis]=-1;

  for(int i=0; i<2; i++) {
    directions[i+2][up][x_axis]=-directions[i][up][x_axis];
    directions[i+2][up][y_axis]=-directions[i][up][y_axis];
    directions[i+2][left][x_axis]=-directions[i][left][x_axis];
    directions[i+2][left][y_axis]=-directions[i][left][y_axis];
  }

  for(int i=0; i<4; i++) {
    directions[i][down][x_axis]=-directions[i][up][x_axis];
    directions[i][down][y_axis]=-directions[i][up][y_axis];
    directions[i][right][x_axis]=-directions[i][left][x_axis];
    directions[i][right][y_axis]=-directions[i][left][y_axis];
  }
}

int rotation = 0;

float speed_x=0;
float speed_y=0;
int lastTime=0;
float angle_x=45;
int angle_y;
float zoom = -15;

int folk_x, folk_y;
int currentLevelFields[10][10];
int currentLevelBox[10][10];
int currentLevelFolk[10][10];
int currentLevelFloor[10][10];
int levelSize = -1;
int currentLevel = -1;

float angle = 0;

int window_id = -1;

string toString(int number) {
  if (number == 0) return "0";
  string temp="";
  string returnvalue="";
  while (number>0) {
    temp+=number%10+48;
    number/=10;
  }
  for (int i=0;i<temp.length();i++)
    returnvalue+=temp[temp.length()-i-1];
  return returnvalue;
}

void move(int direction) {
  int new_folk_y, new_folk_x;
  int move_allowed = 1;
  new_folk_x = folk_x + directions[rotation][direction][x_axis];
  new_folk_y = folk_y + directions[rotation][direction][y_axis];
  if (currentLevelFields[new_folk_x][new_folk_y]) move_allowed= 0;
  if (move_allowed && currentLevelBox[new_folk_x][new_folk_y]){
    int new_box_x = new_folk_x + directions[rotation][direction][x_axis];
    int new_box_y = new_folk_y + directions[rotation][direction][y_axis];
    if (currentLevelFields[new_box_x][new_box_y]) move_allowed = 0;
    if (currentLevelBox[new_box_x][new_box_y]) move_allowed = 0;
    if (move_allowed) {
      currentLevelBox[new_box_x][new_box_y] = 1;
      currentLevelBox[new_folk_x][new_folk_y] = 0;
    }
  }
  if (move_allowed) {
    currentLevelFolk[folk_x][folk_y]=0;
    folk_x = new_folk_x;
    folk_y = new_folk_y;
    currentLevelFolk[folk_x][folk_y]=1;
  }
}

void drawFloor(glm::mat4 V) {
  glm::mat4 M=glm::mat4(1.0f);
  glLoadMatrixf(glm::value_ptr(V*M));

  for(int i=0; i<levelSize; i++) {
    for(int j=0; j<levelSize; j++) {
      if (currentLevelFields[i][j]==1) {
        glColor3d(255, 255, 255);
        glutSolidCube(1.0);
      }
      if (currentLevelBox[i][j]==1) {
        glColor3d(255, 0, 0);
        glutSolidCube(1.0);
      }
      else if (currentLevelBox[i][j]==2) {
        glColor3d(255, 255, 0);
        glutSolidCube(1.0);
      }
      if (currentLevelFolk[i][j]) {
        glColor3d(255, 0, 255);
        glutSolidCube(1.0);
      }
      M=glm::translate(M,glm::vec3(0.0f, 1.0f, 0.0f));
      glLoadMatrixf(glm::value_ptr(V*M));
    }
    M=glm::translate(M,glm::vec3(1.0f, float(-levelSize), 0.0f));
    glLoadMatrixf(glm::value_ptr(V*M));
  }
}

void displayFrame(void) {

	glClearColor(0,0,0,1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 V=glm::lookAt(
    glm::vec3(0.0f, 0.0f, zoom),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));

  //rotation by arrows
  V=glm::rotate(V, angle_x, glm::vec3(1.0f, 0.0f, 0.0f));
  V=glm::rotate(V, float(angle_y), glm::vec3(0.0f, 0.0f, 1.0f));

  //centering board
  float offset = -float(levelSize)/2;
  V=glm::translate(V, glm::vec3(offset, offset, 0.0f));

  //objects closer than 1.0 and further than 50.0 are hidden
  glm::mat4 P=glm::perspective(50.0f, 1.0f, 1.0f, 50.0f);

  drawFloor(V);

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(glm::value_ptr(P));
  glMatrixMode(GL_MODELVIEW);

  glutSwapBuffers();
}

void quit() {
  glutDestroyWindow (window_id);
  exit (0);
}

void nextFrame(void) {
	int actTime=glutGet(GLUT_ELAPSED_TIME);
	int interval=actTime-lastTime;
	lastTime=actTime;
  angle_x+=speed_x*interval/1000.0;
	angle+=speed_x*interval/1000.0;
	if (angle_x>90) angle_x=90;
  if (angle_x<0) angle_x=0;
	if (angle_y>360) angle_y-=360;
	if (angle_y>360) angle_y+=360;
  rotation = angle_y/90;

	glutPostRedisplay();
}

void keyDown(int c, int x, int y) {
  switch (c) {
    case GLUT_KEY_LEFT:
      move(left);
      break;
    case GLUT_KEY_RIGHT:
      move(right);
      break;
    case GLUT_KEY_UP:
      move(up);
      break;
    case GLUT_KEY_DOWN:
      move(down);
      break;
    case 122:
      // camera left
      angle_y+=90;
      break;
    case 120:
      // camera right
      angle_y-=90;
      break;
    case 99:
      // camera up
      speed_x=-60;
      break;
    case 118:
      // camera down
      speed_x=60;
      break;
    case 27:
      //ESC
      quit();
      break;
    case 61:
      //zoom in
      zoom+=1;
      break;
    case 45:
      //zoom out
      zoom-=1;
      break;
  }
}

void keyUp(int c, int x, int y) {
  switch (c) {
    case 99:
      speed_x=0;
      break;
    case 118:
      speed_x=-0;
      break;
  }
}

void debugLevel() {
  for(int i=0; i<levelSize; i++) {
    for(int j=0; j<levelSize; j++) {
      cout << currentLevelFields[i][j] << " ";
    }
    cout << endl;
  }
}

void resetLevel() {
  for(int i=0; i<levelSize; i++) {
    for(int j=0; j<levelSize; j++) {
      currentLevelFields[i][j]=0;
      currentLevelFloor[i][j]=0;
      currentLevelBox[i][j]=0;
      currentLevelFolk[i][j]=0;
    }
  }
}

void readLevel(int level) {
  resetLevel();
  currentLevel = level;
  int size = -1, field;
  string filePath = "levels/" + toString(level) + ".level";
  ifstream levelFile(filePath.c_str());
  levelFile >> levelSize;
  for(int i=0; i<levelSize; i++) {
    for(int j=0; j<levelSize; j++) {
      levelFile >> field;
      // Fields:
      //   1: border exists
      // Folk:
      //   1: fols exists
      // Box:
      //   1: normal box
      //   2: done box
      // Floor:
      //   2: spot
      //   3: normal floor
      switch(field) {
        case 9:
          //borders
          currentLevelFields[i][j]=1;
          currentLevelFloor[i][j]=1;
          break;
        case 5:
          //folk position
          folk_x = i;
          folk_y = j;
          currentLevelFolk[i][j]=1;
          currentLevelFloor[i][j]=1;
          break;
        case 3:
          //normal box
          currentLevelBox[i][j]=1;
          currentLevelFloor[i][j]=1;
          break;
        case 4:
          //done box
          currentLevelBox[i][j]=2;
          currentLevelFloor[i][j]=2;
          break;
        case 2:
          //empty spot
          currentLevelFloor[i][j]=2;
          break;
      }
    }
  }
  debugLevel();
}

int main(int argc, char* argv[]) {
  defineDirections();
  readLevel(1);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	window_id = glutCreateWindow("Program OpenGL");
	glutDisplayFunc(displayFrame);
	glutIdleFunc(nextFrame);

	//Tutaj kod inicjujacy
	glewInit();
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
  // glShadeModel(GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);
  glutMainLoop();
  return 0;
}
