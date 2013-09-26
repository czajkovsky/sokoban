#include <GL/glew.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "tga.h"
#include "cube.h"
#include "wall.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define up 0
#define down 1
#define left 2
#define right 3
#define x_axis 0
#define y_axis 1
#define last_level 3

#define max_zoom -6
#define min_zoom -35

GLuint tex[10];
TGAImg img;

int directions[4][4][2];
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
int currentLevel = 1;
int boxesCount = 0, boxesDone = 0;

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

void initAngles() {
  rotation = 0;
  speed_x=0;
  speed_y=0;
  angle_x=45;
  angle_y = 0;
  zoom = -15;
}

void loadTexture(char* path, int id) {
  if (img.Load(path) == IMG_OK) {
    glGenTextures(1,&tex[id]);
    glBindTexture(GL_TEXTURE_2D,tex[id]);
    if (img.GetBPP() >= 24) {
      glTexImage2D(GL_TEXTURE_2D, 0, img.GetBPP()/8, img.GetWidth(), img.GetHeight(), 0,  GL_RGB,GL_UNSIGNED_BYTE, img.GetImg());
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
  }
}

void debugLevel() {
  for(int i=0; i<levelSize; i++) {
    for(int j=0; j<levelSize; j++) {
      cout << currentLevelFloor[i][j] << " ";
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
  boxesCount = 0;
  currentLevel = level;
  int size = -1, field;
  string filePath = "levels/" + toString(level) + ".level";
  ifstream levelFile(filePath.c_str());
  levelFile >> levelSize;
  for(int i=0; i<levelSize; i++) {
    for(int j=0; j<levelSize; j++) {
      levelFile >> field;
      if (field>0) currentLevelFloor[i][j]=1;
      switch(field) {
        case 9: //borders
          currentLevelFields[i][j]=1;
          currentLevelFloor[i][j]=1;
          break;
        case 5: //folk position
          folk_x = i;
          folk_y = j;
          currentLevelFolk[i][j]=1;
          currentLevelFloor[i][j]=1;
          break;
        case 3: //normal box
          boxesCount++;
          currentLevelBox[i][j]=1;
          currentLevelFloor[i][j]=1;
          break;
        case 4: //done box
          boxesCount++;
          currentLevelBox[i][j]=1;
          currentLevelFloor[i][j]=2;
          break;
        case 2: //empty spot
          currentLevelFloor[i][j]=2;
          break;
      }
    }
  }
}

void changeZoom(int change) {
  int new_zoom = zoom + change;
  if (new_zoom > min_zoom && new_zoom < max_zoom) zoom = new_zoom;
}

void restartLevel(int level) {
  initAngles();
  resetLevel();
  readLevel(level);
}

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

void move(int direction) {
  int new_folk_y, new_folk_x;
  int move_allowed = 1;
  new_folk_x = folk_x + directions[rotation][direction][x_axis];
  new_folk_y = folk_y + directions[rotation][direction][y_axis];
  if (currentLevelFields[new_folk_x][new_folk_y]) move_allowed = 0;
  if (move_allowed && currentLevelBox[new_folk_x][new_folk_y]) {
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

void renderSqaure () {
  glBegin(GL_QUADS);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glEnd();
}

void drawCube(int texture_id) {
  glBindTexture(GL_TEXTURE_2D,tex[texture_id]);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
  glTexCoordPointer(2, GL_FLOAT, 0, cubeTexCoords);
  glDrawArrays(GL_QUADS, 0, cubeVertexCount);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void drawWall(int texture_id) {
  glBindTexture(GL_TEXTURE_2D,tex[texture_id]);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, wallVertices);
  glTexCoordPointer(2, GL_FLOAT, 0, wallTexCoords);
  glDrawArrays(GL_QUADS, 0, wallVertexCount);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

int drawFloor(glm::mat4 V) {
  boxesDone = 0;
  glm::mat4 M=glm::mat4(1.0f);
  glLoadMatrixf(glm::value_ptr(V*M));

  for(int i=0; i<levelSize; i++) {
    for(int j=0; j<levelSize; j++) {
      if (currentLevelFields[i][j] == 1) drawCube(1);
      if (currentLevelBox[i][j] == 1) {
        if (currentLevelFloor[i][j] == 2) {
          boxesDone++;
          drawCube(4);
        }
        else drawCube(3);
      }
      if (currentLevelFolk[i][j]) drawCube(2);
      if (currentLevelFloor[i][j]) {
        if (currentLevelFloor[i][j] == 2) drawWall(6);
        else drawWall(5);
      }
      M=glm::translate(M,glm::vec3(0.0f, 1.0f, 0.0f));
      glLoadMatrixf(glm::value_ptr(V*M));
    }
    M=glm::translate(M,glm::vec3(1.0f, float(-levelSize), 0.0f));
    glLoadMatrixf(glm::value_ptr(V*M));
  }
  return boxesDone;
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

  if(drawFloor(V)==boxesCount && currentLevel < last_level) {
    currentLevel++;
    restartLevel(currentLevel);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(glm::value_ptr(V));

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(glm::value_ptr(P));
  glMatrixMode(GL_MODELVIEW);

  glutSwapBuffers();
}

void initLight() {

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  GLfloat light_ambient[] = { 0.4, 0.4, 0.4, 0.1 };
  GLfloat light_diffuse[] = { 0.2, 0.2, 0.2, 0.1 };
  GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1.0 };
  GLfloat light_position[] = { 3.0, 3.0, 30.0, 1.0 };
  GLfloat light_spot_cutoff[] = { 11.0, 4.0, 9.0, 1.0 };
  GLfloat light_spot_exponent[] = { 11.0, 4.0, 9.0, 1.0 };

  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position);
  glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, light_spot_cutoff);
  glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, light_spot_exponent);

  glEnable(GL_LIGHT2);

  GLfloat light2_ambient[] = { 1.0, 1.0, 1.0, 0.7 };
  GLfloat light2_diffuse[] = { 1.0, 1.0, 1.0, 0.7 };
  GLfloat light2_specular[] = { 1.0, 1.0, 1.0, 0.9 };
  GLfloat light2_position[] = { 0.0, 0.0, 20.0, 1.0 };

  glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
  glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

  GLfloat global_ambient[] = { 0.3f, 0.3f, 0.3f, 0.3f };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
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
	if (angle_y==360) angle_y=0;
	if (angle_y<0) angle_y+=360;
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
      changeZoom(1);
      break;
    case 45:
      //zoom out
      changeZoom(-1);
      break;
    case 114:
      //reset level
      restartLevel(currentLevel);
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

int main(int argc, char* argv[]) {
  defineDirections();
  restartLevel(currentLevel);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	window_id = glutCreateWindow("Sokoban 3D");
	glutDisplayFunc(displayFrame);
	glutIdleFunc(nextFrame);

	glewInit();
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);
  glShadeModel(GL_FLAT);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_NORMALIZE);

  initLight();

  loadTexture("./res/bricks.tga", 1);
  loadTexture("./res/folk.tga", 2);
  loadTexture("./res/block.tga", 3);
  loadTexture("./res/done.tga", 4);
  loadTexture("./res/floor.tga", 5);
  loadTexture("./res/spot.tga", 6);

  glutMainLoop();

  return 0;
}
