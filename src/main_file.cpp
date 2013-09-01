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

float speed_x=0; //60 stopni/s
float speed_y=0; //60 stopni/s
int lastTime=0;
float angle_x;
float angle_y;
float zoom = -5;

int currentLevelFields[10][10];
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

void drawFloor() {
  // glutSolidCube(0.5);
  glutSolidTeapot(0.8);
}

void displayFrame(void) {
	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 M=glm::mat4(1.0f);
    M=glm::rotate(M,angle_x,glm::vec3(1.0f,0.0f,0.0f));
    M=glm::rotate(M,angle_y,glm::vec3(0.0f,1.0f,0.0f));

    drawFloor();

    glm::mat4 V=glm::lookAt(
      glm::vec3(0.0f, 0.0f, zoom),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 P=glm::perspective(50.0f, 1.0f, 1.0f, 50.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(P));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(V*M));


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
	angle_y+=speed_y*interval/1000.0;
	if (angle_x>360) angle_x-=360;
	if (angle_x>360) angle_x+=360;
	if (angle_y>360) angle_y-=360;
	if (angle_y>360) angle_y+=360;

	glutPostRedisplay();
}

void keyDown(int c, int x, int y) {
  switch (c) {
    case GLUT_KEY_LEFT:
      speed_y=60;
      break;
    case GLUT_KEY_RIGHT:
      speed_y=-60;
      break;
    case GLUT_KEY_UP:
      speed_x=60;
      break;
    case GLUT_KEY_DOWN:
      speed_x=-60;
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
    case GLUT_KEY_LEFT:
      speed_y=0;
      break;
    case GLUT_KEY_RIGHT:
      speed_y=-0;
      break;
    case GLUT_KEY_UP:
      speed_x=0;
      break;
    case GLUT_KEY_DOWN:
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

void readLevel(int level) {
  currentLevel = level;
  int size = -1;
  string filePath = "levels/" + toString(level) + ".level";
  ifstream levelFile(filePath.c_str());
  levelFile >> levelSize;
  for(int i=0; i<levelSize; i++) {
    for(int j=0; j<levelSize; j++) {
      levelFile >> currentLevelFields[i][j];
    }
  }
  debugLevel();
}

int main(int argc, char* argv[]) {
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
