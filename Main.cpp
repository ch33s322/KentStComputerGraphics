//Dakota Deets september 2025
#include <GL/glut.h>
#include <iostream>
#include <math.h> //only used for sqrt and pow cause im lazy
#include <Windows.h>
#include <mmsystem.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"





#pragma endregion


//Global variables
const int numOfRobots = 50;

bool sound = true;

bool isFullscreen = 0;  // track state
int windowPosX = 100, windowPosY = 100;
int width = 800;
int height = 600;

bool enemyMotion = false;

int moveDir = 1;

bool leftDown = false;
int lightMode = 0;
int score = 0;

double bulletVelocity[3] = { 0.0f, 0.0f, 0.0f };   // initial position
double bulletPos[3] = { 0.0f, 0.0f, 0.0f };   // velocity vector
bool bulletAlive = false;

double bulletSpeed = 0.01;

bool drawAxis = true;
bool blackAndWhite = false;
char drawMode = 's';
bool music = true;
bool dancing = false;

bool paused = false;
bool started = false;
int gameTime = 0;


bool crazyMode = false;

double mouseSensetivity = 0.0025;

int zFar = 1000;

int lastX = 0;
int lastY = 0;

double cameraSpeed = 1;
double danceTheta = 0;

long double theta = 0;
long double phi = 3.14 / 2;

bool cameraMode1 = true;
bool cameraMode2 = false;
bool cameraMode3 = false;
bool cameraMode4 = false;

double cameraRadius = 50;
long double cameraX = 5;
long double cameraY = 0;
long double cameraZ = 0;

long double cameraLookX = 0;
long double cameraLookY = 0;
long double cameraLookZ = -1;


int shadeMode = 1;

GLuint backgroundTexture;
GLuint headTexture;
GLuint torsoTexture;
GLuint armTexture;
GLuint legTexture;
GLuint hurtTexture;

GLuint grassTexture;

GLuint menuTexture;


#pragma region class definitionc

class position {
public:
	double cords[3];
};




class axis {
public:
	axis(bool drawAxis) {
		if (drawAxis) {
			glBegin(GL_LINES);
			glColor3f(1, 0, 0); //x is red
			glVertex3d(0, 0, 0);
			glVertex3d(1, 0, 0);

			glColor3f(0, 1, 0); //y is green
			glVertex3d(0, 0, 0);
			glVertex3d(0, 1, 0);

			glColor3f(0, 0, 1); //z is blue
			glVertex3d(0, 0, 0);
			glVertex3d(0, 0, 1);
			glEnd();
		}
	}
};

class cameraObject {
public:
	cameraObject() {
			glBegin(GL_LINES);
			glColor3f(0, 1, 0); //y is green
			glVertex3d(cameraX, cameraY, cameraZ);
			glVertex3d(cameraX, cameraY + cameraLookY, cameraZ);

			glColor3f(0, 0, 1); //z is blue
			glVertex3d(cameraX, cameraY, cameraZ);
			glVertex3d(cameraX + cameraLookX, cameraY, cameraZ + cameraLookZ);
			glEnd();
	}
};

class vector3d {
public:
	double x;
	double y;
	double z;

	vector3d() {
		x = 0;
		y = 0;
		z = 0;
	}

	vector3d(double a, double b, double c) {
		x = a;
		y = b;
		z = c;
	}

	void setVector(double a, double b, double c){
		x = a;
		y = b;
		z = c;
	}
};

class box {
public:
	box(double x, double y, double z, double l, double h, double w, char mode, GLuint textureID) {
		glPushMatrix();
		//glTranslated(x, y, z);

		if (mode == 'p') { //draw only verts of box
			glBegin(GL_POINTS);
			glVertex3d(x, y, z);
			glVertex3d(x + l, y, z);
			glVertex3d(x, y + h, z);
			glVertex3d(x + l, y + h, z);
			glVertex3d(x, y, z + w);
			glVertex3d(x + l, y, z + w);
			glVertex3d(x, y + h, z + w);
			glVertex3d(x + l, y + h, z + w);
			glEnd();
		}
		else if (mode == 'w') { //wireframe box must be 12 pairs of lines
			glBegin(GL_LINES);
			glVertex3d(x, y, z);
			glVertex3d(x + l, y, z);

			glVertex3d(x, y + h, z);
			glVertex3d(x + l, y + h, z);

			glVertex3d(x, y, z + w);
			glVertex3d(x + l, y, z + w);

			glVertex3d(x, y + h, z + w);
			glVertex3d(x + l, y + h, z + w);

			glVertex3d(x, y, z);
			glVertex3d(x, y + h, z);

			glVertex3d(x, y, z);
			glVertex3d(x, y + h, z);

			glVertex3d(x, y, z);
			glVertex3d(x, y, z + w);

			glVertex3d(x, y, z + w);
			glVertex3d(x, y + h, z + w);

			glVertex3d(x, y + h, z + w);
			glVertex3d(x, y + h, z);

			glVertex3d(x + l, y, z);
			glVertex3d(x + l, y + h, z);

			glVertex3d(x + l, y + h, z);
			glVertex3d(x + l, y + h, z + w);

			glVertex3d(x + l, y + h, z + w);
			glVertex3d(x + l, y, z + w);

			glVertex3d(x + l, y, z + w);
			glVertex3d(x + l, y, z);


			glEnd();
		}
		else if (mode == 's') { //quads/surfaces must be 6 sets of 4 points

			if (textureID != NULL) {
				glBindTexture(GL_TEXTURE_2D, textureID);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, NULL);
			}

			glBegin(GL_QUADS);

			//front
			if(shadeMode == 0){ 
				glNormal3f(-1, -1, -1); glTexCoord2f(0, 1); glVertex3d(x, y, z);
				glNormal3f(1, -1, -1); glTexCoord2f(1, 1); glVertex3d(x + l, y, z);
				glNormal3f(1, 1, -1); glTexCoord2f(1, 0); glVertex3d(x + l, y + h, z);
				glNormal3f(-1, 1, -1); glTexCoord2f(0, 0); glVertex3d(x, y + h, z);
			}
			else{ glNormal3f(0, 1, 0); 
			glTexCoord2f(0, 1); glVertex3d(x, y, z);
			glTexCoord2f(1, 1); glVertex3d(x + l, y, z);
			glTexCoord2f(1, 0); glVertex3d(x + l, y + h, z);
			glTexCoord2f(0, 0); glVertex3d(x, y + h, z);
			}
			

			//right
			if (shadeMode == 0) { 
				glNormal3f(-1, -1, -1); glTexCoord2f(1, 1); glVertex3d(x, y, z);
				glNormal3f(-1, -1, 1); glTexCoord2f(0, 1); glVertex3d(x, y, z + w);
				glNormal3f(-1, 1, 1); glTexCoord2f(0, 0); glVertex3d(x, y + h, z + w);
				glNormal3f(-1, 1, -1); glTexCoord2f(1, 0); glVertex3d(x, y + h, z);
			}
			else { glNormal3f(0, 1, 0); 
			glTexCoord2f(1, 1); glVertex3d(x, y, z);
			glTexCoord2f(0, 1); glVertex3d(x, y, z + w);
			glTexCoord2f(0, 0); glVertex3d(x, y + h, z + w);
			glTexCoord2f(1, 0); glVertex3d(x, y + h, z);
			}


			//bottom
			if (shadeMode == 0) { 
				glNormal3f(-1, -1, -1); glTexCoord2f(0, 0); glVertex3d(x, y, z);
				glNormal3f(1, -1, -1); glTexCoord2f(1, 0); glVertex3d(x + l, y, z);
				glNormal3f(1, -1, 1); glTexCoord2f(1, 1); glVertex3d(x + l, y, z + w);
				glNormal3f(-1, -1, 1); glTexCoord2f(0, 1); glVertex3d(x, y, z + w);
			}
			else { glNormal3f(0, 1, 0); 
			glTexCoord2f(0, 0); glVertex3d(x, y, z);
			glTexCoord2f(1, 0); glVertex3d(x + l, y, z);
			glTexCoord2f(1, 1); glVertex3d(x + l, y, z + w);
			glTexCoord2f(0, 1); glVertex3d(x, y, z + w);
			}
			

			//back
			if (shadeMode == 0) {
				glNormal3f(-1, -1, 1); glTexCoord2f(1, 1); glVertex3d(x, y, z + w);
				glNormal3f(-1, 1, 1); glTexCoord2f(1, 0); glVertex3d(x, y + h, z + w);
				glNormal3f(1, 1, 1); glTexCoord2f(0, 0); glVertex3d(x + l, y + h, z + w);
				glNormal3f(1, -1, 1); glTexCoord2f(0, 1); glVertex3d(x + l, y, z + w);
			}
			else { glNormal3f(0, 1, 0); 
			glTexCoord2f(1, 1); glVertex3d(x, y, z + w);
			glTexCoord2f(1, 0); glVertex3d(x, y + h, z + w);
			glTexCoord2f(0, 0); glVertex3d(x + l, y + h, z + w);
			glTexCoord2f(0, 1); glVertex3d(x + l, y, z + w);
			}
			

			//top
			if (shadeMode == 0) { 
				glNormal3f(-1, 1, -1); glTexCoord2f(1, 0); glVertex3d(x, y + h, z);
				glNormal3f(1, 1, -1); glTexCoord2f(0, 0); glVertex3d(x + l, y + h, z);
				glNormal3f(1, 1, 1); glTexCoord2f(0, 1); glVertex3d(x + l, y + h, z + w);
				glNormal3f(-1, 1, 1); glTexCoord2f(1, 1); glVertex3d(x, y + h, z + w);
			}
			else { glNormal3f(0, 1, 0); 
			glTexCoord2f(1, 0); glVertex3d(x, y + h, z);
			glTexCoord2f(0, 0); glVertex3d(x + l, y + h, z);
			glTexCoord2f(0, 1); glVertex3d(x + l, y + h, z + w);
			glTexCoord2f(1, 1); glVertex3d(x, y + h, z + w);
			}
			

			//left
			if (shadeMode == 0) { 
				glNormal3f(1, -1, -1); glTexCoord2f(0, 1); glVertex3d(x + l, y, z);
				glNormal3f(1, -1, 1); glTexCoord2f(1, 1); glVertex3d(x + l, y, z + w);
				glNormal3f(1, 1, 1); glTexCoord2f(1, 0); glVertex3d(x + l, y + h, z + w);
				glNormal3f(1, 1, -1); glTexCoord2f(0, 0); glVertex3d(x + l, y + h, z);
			}
			else { glNormal3f(0, 1, 0); 
			glTexCoord2f(0, 1); glVertex3d(x + l, y, z);
			glTexCoord2f(1, 1); glVertex3d(x + l, y, z + w);
			glTexCoord2f(1, 0); glVertex3d(x + l, y + h, z + w);
			glTexCoord2f(0, 0); glVertex3d(x + l, y + h, z);
			}
			

			glEnd();
		}


	}
};


vector3d headColor(1, 1, 0);
vector3d torsoColor(0, 0, 0.5);
vector3d rightArmColor(1, 1, 0);
vector3d leftArmColor(1, 1, 0);
vector3d rightLegColor(0, 0.75, 0);
vector3d leftLegColor(0, 0.75, 0);

bool robotHit[numOfRobots];

position enemies[numOfRobots];

class robot {
public:
	robot(double x, double y, double z, double saturation, int danceType, int id) {
		if (danceType == 0) {
			// robot primatives
			
			if (true) {
				if (robotHit[id]) {
					box head(-0.5 + x, -0.5 + y, -0.5 + z, 1, 1, 1, drawMode, hurtTexture);
					box torso(-1 + x, -2.5 + y, -0.5 + z, 2, 2, 1, drawMode, hurtTexture);
					box rightArm(-2 + x, (!dancing ? -2.5 : sin(danceTheta)) + y, -0.5 + z, 1, 2, 1, drawMode, hurtTexture);
					box leftArm(1 + x, (!dancing ? -2.5 : cos(danceTheta)) + y, -0.5 + z, 1, 2, 1, drawMode, hurtTexture);
					box rigtLeg(-1 + x, -4.5 + y, -0.5 + z, 1, 2, 1, drawMode, hurtTexture);
					box leftLeg(0 + x, -4.5 + y, -0.5 + z, 1, 2, 1, drawMode, hurtTexture);
				}
				else {
					box head(-0.5 + x, -0.5 + y, -0.5 + z, 1, 1, 1, drawMode, headTexture);
					box torso(-1 + x, -2.5 + y, -0.5 + z, 2, 2, 1, drawMode, torsoTexture);
					box rightArm(-2 + x, (!dancing ? -2.5 : sin(danceTheta)) + y, -0.5 + z, 1, 2, 1, drawMode, armTexture);
					box leftArm(1 + x, (!dancing ? -2.5 : cos(danceTheta)) + y, -0.5 + z, 1, 2, 1, drawMode, armTexture);
					box rigtLeg(-1 + x, -4.5 + y, -0.5 + z, 1, 2, 1, drawMode, legTexture);
					box leftLeg(0 + x, -4.5 + y, -0.5 + z, 1, 2, 1, drawMode, legTexture);
				}
			}
			else {
			//	saturation = sqrt(pow(headColor.x, 2) + pow(headColor.y, 2) + pow(headColor.z, 2));
			//	!robotHit[id] ? glColor3f(headColor.x, headColor.y, headColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box head(-0.5 + x, -0.5 + y, -0.5 + z, 1, 1, 1, drawMode, headTexture);



			//	saturation = sqrt(pow(torsoColor.x, 2) + pow(torsoColor.y, 2) + pow(torsoColor.z, 2));
			//	!robotHit[id] ? glColor3f(torsoColor.x, torsoColor.y, torsoColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box torso(-1 + x, -2.5 + y, -0.5 + z, 2, 2, 1, drawMode, torsoTexture);


			//	saturation = sqrt(pow(rightArmColor.x, 2) + pow(rightArmColor.y, 2) + pow(rightArmColor.z, 2));
			//	!robotHit[id] ? glColor3f(rightArmColor.x, rightArmColor.y, rightArmColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box rightArm(-2 + x, (!dancing ? -2.5 : sin(danceTheta)) + y, -0.5 + z, 1, 2, 1, drawMode, armTexture);

			//	saturation = sqrt(pow(leftArmColor.x, 2) + pow(leftArmColor.y, 2) + pow(leftArmColor.z, 2));
			//	!robotHit[id] ? glColor3f(leftArmColor.x, leftArmColor.y, leftArmColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box leftArm(1 + x, (!dancing ? -2.5 : cos(danceTheta)) + y, -0.5 + z, 1, 2, 1, drawMode, armTexture);

			//	saturation = sqrt(pow(rightLegColor.x, 2) + pow(rightLegColor.y, 2) + pow(rightLegColor.z, 2));
			//	!robotHit[id] ? glColor3f(rightLegColor.x, rightLegColor.y, rightLegColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box rigtLeg(-1 + x, -4.5 + y, -0.5 + z, 1, 2, 1, drawMode, legTexture);

			//	saturation = sqrt(pow(leftLegColor.x, 2) + pow(leftLegColor.y, 2) + pow(leftLegColor.z, 2));
			//	!robotHit[id] ? glColor3f(leftLegColor.x, leftLegColor.y, leftLegColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box leftLeg(0 + x, -4.5 + y, -0.5 + z, 1, 2, 1, drawMode, legTexture);
			//}
			//if (danceType == 1) {
			//	// robot primatives
			//	!robotHit[id] ? glColor3f(headColor.x, headColor.y, headColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box head(-0.5 + x, (!dancing ? -0.5 : -cos(danceTheta)) + y, -0.5 + z, 1, 1, 1, drawMode, NULL);

			//	saturation = sqrt(pow(torsoColor.x, 2) + pow(torsoColor.y, 2) + pow(torsoColor.z, 2));
			//	!robotHit[id] ? glColor3f(torsoColor.x, torsoColor.y, torsoColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box torso(-1 + x, -2.5 + y, -0.5 + z, 2, 2, 1, drawMode, NULL);


			//	saturation = sqrt(pow(rightArmColor.x, 2) + pow(rightArmColor.y, 2) + pow(rightArmColor.z, 2));
			//	!robotHit[id] ? glColor3f(rightArmColor.x, rightArmColor.y, rightArmColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box rightArm(-2 + x, (!dancing ? -2.5 : cos(danceTheta)) + y, -0.5 + z, 1, 2, 1, drawMode, NULL);

			//	saturation = sqrt(pow(leftArmColor.x, 2) + pow(leftArmColor.y, 2) + pow(leftArmColor.z, 2));
			//	!robotHit[id] ? glColor3f(leftArmColor.x, leftArmColor.y, leftArmColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box leftArm(1 + x, (!dancing ? -2.5 : cos(danceTheta)) + y, -0.5 + z, 1, 2, 1, drawMode, NULL);

			//	saturation = sqrt(pow(rightLegColor.x, 2) + pow(rightLegColor.y, 2) + pow(rightLegColor.z, 2));
			//	!robotHit[id] ? glColor3f(rightLegColor.x, rightLegColor.y, rightLegColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box rigtLeg(-1 + x, -4.5 + y, -0.5 + z, 1, 2, 1, drawMode, NULL);

			//	saturation = sqrt(pow(leftLegColor.x, 2) + pow(leftLegColor.y, 2) + pow(leftLegColor.z, 2));
			//	!robotHit[id] ? glColor3f(leftLegColor.x, leftLegColor.y, leftLegColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box leftLeg(0 + x, -4.5 + y, -0.5 + z, 1, 2, 1, drawMode, NULL);
			//}
			//if (danceType == 2) {
			//	// robot primatives
			//	!robotHit[id] ? glColor3f(headColor.x, headColor.y, headColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box head(-0.5 + x, (!dancing ? -0.5 : cos(danceTheta) - 0.5) + y, -0.5 + z, 1, 1, 1, drawMode, NULL);

			//	saturation = sqrt(pow(torsoColor.x, 2) + pow(torsoColor.y, 2) + pow(torsoColor.z, 2));
			//	!robotHit[id] ? glColor3f(torsoColor.x, torsoColor.y, torsoColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box torso(-1 + x, (!dancing ? -2.5 : cos(danceTheta) - 2.5) + y, -0.5 + z, 2, 2, 1, drawMode, NULL);


			//	saturation = sqrt(pow(rightArmColor.x, 2) + pow(rightArmColor.y, 2) + pow(rightArmColor.z, 2));
			//	!robotHit[id] ? glColor3f(rightArmColor.x, rightArmColor.y, rightArmColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box rightArm(-2 + x, (!dancing ? -2.5 : cos(danceTheta) - 2.5) + y, -0.5 + z, 1, 2, 1, drawMode, NULL);

			//	saturation = sqrt(pow(leftArmColor.x, 2) + pow(leftArmColor.y, 2) + pow(leftArmColor.z, 2));
			//	!robotHit[id] ? glColor3f(leftArmColor.x, leftArmColor.y, leftArmColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box leftArm(1 + x, (!dancing ? -2.5 : cos(danceTheta) - 2.5) + y, -0.5 + z, 1, 2, 1, drawMode, NULL);

			//	saturation = sqrt(pow(rightLegColor.x, 2) + pow(rightLegColor.y, 2) + pow(rightLegColor.z, 2));
			//	!robotHit[id] ? glColor3f(rightLegColor.x, rightLegColor.y, rightLegColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box rigtLeg(-1 + x, (!dancing ? -4.5 : cos(danceTheta) - 4.5) + y, -0.5 + z, 1, 2, 1, drawMode, NULL);

			//	saturation = sqrt(pow(leftLegColor.x, 2) + pow(leftLegColor.y, 2) + pow(leftLegColor.z, 2));
			//	!robotHit[id] ? glColor3f(leftLegColor.x, leftLegColor.y, leftLegColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box leftLeg(0 + x, (!dancing ? -4.5 : cos(danceTheta) - 4.5) + y, -0.5 + z, 1, 2, 1, drawMode, NULL);
			//}
			//if (danceType == 3) {
			//	// robot primatives
			//	!robotHit[id] ? glColor3f(headColor.x, headColor.y, headColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box head(-0.5 + x, -0.5 + y, -0.5 + z, 1, 1, 1, drawMode, NULL);

			//	saturation = sqrt(pow(torsoColor.x, 2) + pow(torsoColor.y, 2) + pow(torsoColor.z, 2));
			//	!robotHit[id] ? glColor3f(torsoColor.x, torsoColor.y, torsoColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box torso(-1 + x, -2.5 + y, -0.5 + z, 2, 2, 1, drawMode, NULL);


			//	saturation = sqrt(pow(rightArmColor.x, 2) + pow(rightArmColor.y, 2) + pow(rightArmColor.z, 2));
			//	!robotHit[id] ? glColor3f(rightArmColor.x, rightArmColor.y, rightArmColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box rightArm(-2 + x, (!dancing ? -2.5 : sin(danceTheta)) + y, -0.5 + z, 1, 2, 1, drawMode, NULL);

			//	saturation = sqrt(pow(leftArmColor.x, 2) + pow(leftArmColor.y, 2) + pow(leftArmColor.z, 2));
			//	!robotHit[id] ? glColor3f(leftArmColor.x, leftArmColor.y, leftArmColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box leftArm(1 + x, (!dancing ? -2.5 : cos(danceTheta)) + y, -0.5 + z, 1, 2, 1, drawMode, NULL);

			//	saturation = sqrt(pow(rightLegColor.x, 2) + pow(rightLegColor.y, 2) + pow(rightLegColor.z, 2));
			//	!robotHit[id] ? glColor3f(rightLegColor.x, rightLegColor.y, rightLegColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box rigtLeg((!dancing ? -1 : (cos(danceTheta * .5) - 2) * .5) + x, -4.5 + y, -0.5 + z, 1, 2, 1, drawMode, NULL);

			//	saturation = sqrt(pow(leftLegColor.x, 2) + pow(leftLegColor.y, 2) + pow(leftLegColor.z, 2));
			//	!robotHit[id] ? glColor3f(leftLegColor.x, leftLegColor.y, leftLegColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box leftLeg((!dancing ? 0 : (cos(danceTheta * .5) + 1) * -.5) + x, -4.5 + y, -0.5 + z, 1, 2, 1, drawMode, NULL);
			//}
			//if (danceType == 4) {
			//	// robot primatives
			//	!robotHit[id] ? glColor3f(headColor.x, headColor.y, headColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box head(-0.5 + x, -0.5 + y, -0.5 + z, 1, 1, 1, drawMode, NULL);

			//	saturation = sqrt(pow(torsoColor.x, 2) + pow(torsoColor.y, 2) + pow(torsoColor.z, 2));
			//	!robotHit[id] ? glColor3f(torsoColor.x, torsoColor.y, torsoColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box torso(-1 + x, -2.5 + y, -0.5 + z, 2, 2, 1, drawMode, NULL);


			//	saturation = sqrt(pow(rightArmColor.x, 2) + pow(rightArmColor.y, 2) + pow(rightArmColor.z, 2));
			//	!robotHit[id] ? glColor3f(rightArmColor.x, rightArmColor.y, rightArmColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box rightArm(-2 + x, -2.5 + y, (!dancing ? -0.5 : (cos(danceTheta * .5) + 1) * -.5) + z, 1, 2, 1, drawMode, NULL);

			//	saturation = sqrt(pow(leftArmColor.x, 2) + pow(leftArmColor.y, 2) + pow(leftArmColor.z, 2));
			//	!robotHit[id] ? glColor3f(leftArmColor.x, leftArmColor.y, leftArmColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box leftArm(1 + x, -2.5 + y, (!dancing ? -0.5 : (cos(danceTheta * .5) + 1) * -.5) + z, 1, 2, 1, drawMode, NULL);

			//	saturation = sqrt(pow(rightLegColor.x, 2) + pow(rightLegColor.y, 2) + pow(rightLegColor.z, 2));
			//	!robotHit[id] ? glColor3f(rightLegColor.x, rightLegColor.y, rightLegColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box rigtLeg(-1 + x, -4.5 + y, -0.5 + z, 1, 2, 1, drawMode, NULL);

			//	saturation = sqrt(pow(leftLegColor.x, 2) + pow(leftLegColor.y, 2) + pow(leftLegColor.z, 2));
			//	!robotHit[id] ? glColor3f(leftLegColor.x, leftLegColor.y, leftLegColor.z) : glColor3f(1, 0, 0); // R,G,B
			//	box leftLeg(0 + x, -4.5 + y, -0.5 + z, 1, 2, 1, drawMode, NULL);
				}
			}
			
	}
};

class plane {
public:
	plane(char mode, GLuint textureID) {
		glColor3f(1, 1, 1);
		if (mode == 'p') { //draw only verts of box
			glBegin(GL_POINTS);
			glVertex3d(200, -5, 200);
			glVertex3d(200, -5, -200);
			glVertex3d(-200, -5, -200);
			glVertex3d(-200, -5, 200);
			glEnd();
		}
		else if (mode == 'w') { //wireframe box must be 12 pairs of lines
			glBegin(GL_LINES);
			glVertex3d(200, -5, 200);
			glVertex3d(200, -5, -200);

			glVertex3d(200, -5, -200);
			glVertex3d(-200, -5, -200);

			glVertex3d(-200, -5, -200);
			glVertex3d(-200, -5, 200);

			glVertex3d(-200, -5, 200);
			glVertex3d(200, -5, 200);
			glEnd();
		}
		else if (mode == 's') { //quads/surfaces must be 6 sets of 4 points
			if (textureID != NULL) {
				glBindTexture(GL_TEXTURE_2D, textureID);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, NULL);
			}

			glBegin(GL_QUADS);
			glNormal3f(0, 1, 0);
			glTexCoord2f(1, 0); glVertex3d(200, -5, 200);
			glTexCoord2f(0, 0); glVertex3d(200, -5, -200);
			glTexCoord2f(0, 1); glVertex3d(-200, -5, -200);
			glTexCoord2f(1, 1); glVertex3d(-200, -5, 200);
			glEnd();
		}
	}
};
#pragma endregion



// Set texturemapping from a texture image file
GLuint loadTexture(const char* filename)
{
	int w, h, comp;
	unsigned char* image = stbi_load(filename, &w, &h, &comp, STBI_rgb_alpha);
	if (!image) {
		printf("Failed to load texture: %s\n", filename);
		return 0;
	}

	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,
		0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(image);
	return texID;
}





void delay(float secs)
{
	float end = clock() / CLOCKS_PER_SEC + secs;
	while ((clock() / CLOCKS_PER_SEC) < end);
}

bool checkSphereCollision(double aPos[3], double aRadius,
	double bPos[3], double bRadius) {
	double dx = aPos[0] - bPos[0];
	double dy = aPos[1] - bPos[1];
	double dz = aPos[2] - bPos[2];
	double dist2 = dx * dx + dy * dy + dz * dz;
	double radiusSum = aRadius + bRadius;
	return dist2 <= (radiusSum * radiusSum);
}

// Function to shoot sphere in a direction
void shoot(double dirX, double dirY, double dirZ) {
	bulletVelocity[0] = dirX;
	bulletVelocity[1] = dirY;
	bulletVelocity[2] = dirZ;
	bulletAlive = true;
}

void killRobot(int id) {
	enemies[id].cords[1] = -100;
}

// Update position
void bulletUpdate() {
	if (bulletAlive) {
		bulletPos[0] += bulletVelocity[0] * bulletSpeed;
		bulletPos[1] += bulletVelocity[1] * bulletSpeed;
		bulletPos[2] += bulletVelocity[2] * bulletSpeed;
	}
	for (int i = 0; i < numOfRobots; i++) {
		if (checkSphereCollision(bulletPos, .2, enemies[i].cords, 2)) {
			bulletAlive = false;
			bulletPos[0] = 0;
			bulletPos[1] = 0;
			bulletPos[2] = 0;
			bulletVelocity[0] = 0;
			bulletVelocity[1] = 0;
			bulletVelocity[2] = 0;
			robotHit[i] = true;
			score += 10;
			PlaySoundW(TEXT("oof.wav"), NULL, SND_FILENAME | SND_ASYNC);
			glutTimerFunc(500, killRobot, i);
			
		}
	}
	

	glutPostRedisplay();
}


int randomIntFromRange(int range) {
	int x;
	if (rand() % 2 == 0) {
		return rand() % range + 1;
	}
	else {
		return -(rand() % range + 1);
	}
	
}

void printBitmapString(void* font, std::string s)
{

	for (char c : s) {
		glutBitmapCharacter(font, c);
	}

}


void drawText(int width, int height) {
	// Disable textures before drawing bitmap text
	glDisable(GL_TEXTURE_2D);

	// Set text color and position
	glColor3d(1.0, 0.0, 0.0);
	glRasterPos2i(10, height - 10); // Adjust Y so it appears in the lower part of the top panel

	std::string gameTimeString = "time left: " + std::to_string(gameTime);
	std::string gameScore = "score: " + std::to_string(score);
	std::string bSpeed = "bulletSpeed: " + std::to_string(bulletSpeed);

	void* font = GLUT_BITMAP_9_BY_15;


	glRasterPos2i(10, height - 20);
	printBitmapString(font, gameTimeString);

	glRasterPos2i(10, height - 40);
	printBitmapString(font, gameScore);

	glRasterPos2i(10, height - 60);
	printBitmapString(font, bSpeed);

	// Re-enable textures for the rest of the scene
	glEnable(GL_TEXTURE_2D);
}


void initLighting() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	// Global ambient
	GLfloat gA[] = { 0.3, 0.3, 0.3, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gA);
	
	if (lightMode == 0) {
		// Light source
		GLfloat la[] = { 0.7, 0.7, 0.7, 1.0 };
		GLfloat ld[] = { 0.9, 0.9, 0.9, 1.0 };
		GLfloat ls[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat lp[] = { 50, 100, 50, 0 };
		glLightfv(GL_LIGHT0, GL_AMBIENT, la);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, ld);
		glLightfv(GL_LIGHT0, GL_SPECULAR, ls);
		glLightfv(GL_LIGHT0, GL_POSITION, lp);
	}
	else {
		// Light source
		GLfloat la[] = { 0.7, 0.7, 0.7, 1.0 };
		GLfloat ld[] = { 0.9, 0.9, 0.9, 1.0 };
		GLfloat ls[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat lp[] = { 10, 10, 10, 1 };
		glLightfv(GL_LIGHT0, GL_AMBIENT, la);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, ld);
		glLightfv(GL_LIGHT0, GL_SPECULAR, ls);
		glLightfv(GL_LIGHT0, GL_POSITION, lp);
	}
	

	

	// Material
	GLfloat ma[] = { 0.3,0.3,0.3,1 };
	GLfloat md[] = { 0.8,0.8,0.8,1 };
	GLfloat ms[] = { 1,1,1,1 };
	GLfloat sh[] = { 30 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ma);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, md);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ms);
	glMaterialfv(GL_FRONT, GL_SHININESS, sh);
}


void declareModels() {

	initLighting();


	axis origin(drawAxis);
	double saturation;
	saturation = sqrt(pow(headColor.x, 2) + pow(headColor.y, 2) + pow(headColor.z, 2));
	plane floor(drawMode, grassTexture);

	box background(-400, -10, -400, 800, 800, 800, drawMode, backgroundTexture);


	//robot robots(0, 0, 0, saturation, 0, 0);

	for (int i = 0; i < numOfRobots; i++) {
		robot robots(enemies[i].cords[0], enemies[i].cords[1], enemies[i].cords[2], saturation, 0, i);
	}
	
	if (cameraMode1) {
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glTranslatef(cameraX +cameraLookX, cameraY + cameraLookY, cameraZ + cameraLookZ);
		glRotatef((-theta * 180 / 3.14), 0, 1, 0);
		glColor3f(0.5, 0.5, 0.5);
		box gun(-1,-1,0, 1.5, .5, .5, drawMode, NULL);
		glPopMatrix();
	}

	if (bulletAlive) {
		glPushMatrix();
		glTranslatef(bulletPos[0], bulletPos[1], bulletPos[2]);
		glColor3f(1, 0, 0);
		glutSolidSphere(0.2, 20, 20);
		glPopMatrix();
	}

	
	
	cameraObject();
}


void drawOverlay(GLuint textureID) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);  // normalized coordinates

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glColor3f(1, 1, 1); // ensure full brightness

	glBegin(GL_QUADS);
	glTexCoord2f(1, 1); glVertex2f(1, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glTexCoord2f(0, 0); glVertex2f(0, 1);
	glTexCoord2f(1, 0); glVertex2f(1, 1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}



void handleCamera() {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, (double)width / height, 0.1, zFar);
	glMatrixMode(GL_MODELVIEW);
	if (cameraMode1) {
		cameraLookX = 1 * sin(phi) * cos(theta);
		cameraLookZ = 1 * sin(phi) * sin(theta);
		cameraLookY = 1 * cos(phi);
		// MAIN VIEWPORT
		glViewport(0, 0, width, (height * 7/8));  // (x, y, w, h)
		glLoadIdentity();
		gluLookAt(cameraX, cameraY, cameraZ, cameraLookX + cameraX, cameraLookY + cameraY, cameraLookZ + cameraZ, 0.0, 1.0, 0.0);
		declareModels();

		if (paused) {
			drawOverlay(menuTexture);
		}

		// SKY VIEW CAMERA
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(100, (double)width / height, 0.1, zFar);
		glViewport(width * .75, 0, width * .25, height * .25);
		glEnable(GL_SCISSOR_TEST);
		glScissor(width * .75, 0, width * .25, height * .25);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_SCISSOR_TEST);
		gluLookAt(1, 25, 1, 0, 0, 0, 0, 1, 0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		declareModels();
		

		//SCORE AND TEXT DISPLAY
		glDisable(GL_LIGHTING);
		int topHeight = height / 8;
		glViewport(0, height - topHeight, width, topHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, width, 0, topHeight);

		drawText(width, topHeight);
		glEnable(GL_LIGHTING);
	}
	
	if (cameraMode4) { //new "entier sky view" 
		cameraX = cameraRadius * sin(phi) * cos(theta);
		cameraZ = cameraRadius * sin(phi) * sin(theta);
		cameraY = cameraRadius * cos(phi);

		glLoadIdentity();
		glViewport(0, 0, width, height);
		gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);
		declareModels();
	}


	

}

void mouseMotion(int x, int y) {
	if (leftDown) {
		double xoffset = x - lastX;
		double yoffset = lastY - y;  // reversed: y-coordinates go top to bottom
		lastX = x;
		lastY = y;

		theta += (xoffset * mouseSensetivity);
		phi += (-yoffset * mouseSensetivity);
	}
	


	handleCamera();

}

void gameEnd(int i) {
	score = 0;
}


void gameTimer(int i) {
	if (!paused && started) {
		gameTime = gameTime - 1;
	}
	if(gameTime > 0){
		glutTimerFunc(1000, gameTimer, 0);
	}
	else {
		PlaySoundW(TEXT("gameEndMessage.wav"), NULL, SND_FILENAME | SND_ASYNC);
		glutTimerFunc(6000, gameEnd, 0);
	}
}



//this creates shapes and sends them to be displayed
void MyDisplay() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (shadeMode == 0) {
		glShadeModel(GL_SMOOTH);
	}
	else {
		glShadeModel(GL_FLAT);
	}
	
	handleCamera();
	// end drawing
	glFlush();
}

void enemyDodgeMode(position pos[]) {
	if (enemyMotion) {
		for (int i = 0; i < numOfRobots; i++) {
			pos[i].cords[0] -= 0.01 * moveDir;
		}
	}
	glutPostRedisplay();
}

void idle() {
	if (!paused) {
		bulletUpdate();
		enemyDodgeMode(enemies);
	}
	glutPostRedisplay();
}


void timer(int value) {
	if (!paused) {
		danceTheta += 0.3;
	}
	glutPostRedisplay();
	if (dancing) {
		glutTimerFunc(16, timer, 0); // Repeat
	}
	
}

void processMenu(int option) {
	switch (option) {
	//x rotation cases
	case 1:  glRotatef(30, 1, 0, 0); break;
	case 2:  glRotatef(60, 1, 0, 0); break;
	case 3:  glRotatef(90, 1, 0, 0); break;
	case 4:  glRotatef(120, 1, 0, 0); break;
	case 5:  glRotatef(150, 1, 0, 0); break;
	case 6:  glRotatef(180, 1, 0, 0); break;
	case 7:  glRotatef(210, 1, 0, 0); break;
	case 8:  glRotatef(240, 1, 0, 0); break;
	case 9:  glRotatef(270, 1, 0, 0); break;
	case 10: glRotatef(300, 1, 0, 0); break;
	case 11: glRotatef(330, 1, 0, 0); break;
	case 12: glRotatef(360, 1, 0, 0); break;

	//y rotation cases
	case 13: glRotatef(30, 0, 1, 0); break;
	case 14: glRotatef(60, 0, 1, 0); break;
	case 15: glRotatef(90, 0, 1, 0); break;
	case 16: glRotatef(120, 0, 1, 0); break;
	case 17: glRotatef(150, 0, 1, 0); break;
	case 18: glRotatef(180, 0, 1, 0); break;
	case 19: glRotatef(210, 0, 1, 0); break;
	case 20: glRotatef(240, 0, 1, 0); break;
	case 21: glRotatef(270, 0, 1, 0); break;
	case 22: glRotatef(300, 0, 1, 0); break;
	case 23: glRotatef(330, 0, 1, 0); break;
	case 24: glRotatef(360, 0, 1, 0); break;
		
	//z rotation cases
	case 25: glRotatef(30, 0, 0, 1); break;
	case 26: glRotatef(60, 0, 0, 1); break;
	case 27: glRotatef(90, 0, 0, 1); break;
	case 28: glRotatef(120, 0, 0, 1); break;
	case 29: glRotatef(150, 0, 0, 1); break;
	case 30: glRotatef(180, 0, 0, 1); break;
	case 31: glRotatef(210, 0, 0, 1); break;
	case 32: glRotatef(240, 0, 0, 1); break;
	case 33: glRotatef(270, 0, 0, 1); break;
	case 34: glRotatef(300, 0, 0, 1); break;
	case 35: glRotatef(330, 0, 0, 1); break;
	case 36: glRotatef(360, 0, 0, 1); break;

	//projection cases
	case 37: gluPerspective(100, (double)width / height, 0, zFar); break;
	case 38: glOrtho(-5, 5, -5, 5, -5, 5); break;

	//head color cases
	case 39: headColor.setVector(.9, 0 ,0); break;
	case 40: headColor.setVector(0, .8, 0); break;
	case 41: headColor.setVector(0, 0, .7); break;
	case 42: headColor.setVector(1, 1, 0); break;

	//arms color cases
	case 43: rightArmColor.setVector(.9, 0, 0); leftArmColor.setVector(.9, 0, 0);  break;
	case 44: rightArmColor.setVector(0, 0.8, 0); leftArmColor.setVector(0, 0.8, 0); break;
	case 45: rightArmColor.setVector(0, 0, 0.7); leftArmColor.setVector(0, 0, 0.7); break;
	case 46: rightArmColor.setVector(1, 1, 0); leftArmColor.setVector(1, 1, 0); break;

	//torso color cases
	case 47: torsoColor.setVector(.9, 0, 0); break;
	case 48: torsoColor.setVector(0, .8, 0); break;
	case 49: torsoColor.setVector(0, 0, .7); break;
	case 50: torsoColor.setVector(1, 1, 0); break;

	//legs color cases
	case 51: rightLegColor.setVector(.9, 0, 0); leftLegColor.setVector(.9, 0, 0); break;
	case 52: rightLegColor.setVector(0, 0.8, 0); leftLegColor.setVector(0, 0.8, 0); break;
	case 53: rightLegColor.setVector(0, 0, 0.7); leftLegColor.setVector(0, 0, 0.7); break;
	case 54: rightLegColor.setVector(1, 1, 0); leftLegColor.setVector(1, 1, 0); break;
	}

	glutPostRedisplay();
}
//unfortunately no hoist makes this gross
class menu {
public:
	menu() {
		int xRotateMenu = glutCreateMenu(processMenu);
		glutAddMenuEntry("30", 1);
		glutAddMenuEntry("60", 2);
		glutAddMenuEntry("90", 3);
		glutAddMenuEntry("120", 4);
		glutAddMenuEntry("150", 5);
		glutAddMenuEntry("180", 6);
		glutAddMenuEntry("210", 7);
		glutAddMenuEntry("240", 8);
		glutAddMenuEntry("270", 9);
		glutAddMenuEntry("300", 10);
		glutAddMenuEntry("330", 11);
		glutAddMenuEntry("360", 12);

		int yRotateMenu = glutCreateMenu(processMenu);
		glutAddMenuEntry("30", 13);
		glutAddMenuEntry("60", 14);
		glutAddMenuEntry("90", 15);
		glutAddMenuEntry("120", 16);
		glutAddMenuEntry("150", 17);
		glutAddMenuEntry("180", 18);
		glutAddMenuEntry("210", 19);
		glutAddMenuEntry("240", 20);
		glutAddMenuEntry("270", 21);
		glutAddMenuEntry("300", 22);
		glutAddMenuEntry("330", 23);
		glutAddMenuEntry("360", 24);

		int zRotateMenu = glutCreateMenu(processMenu);
		glutAddMenuEntry("30", 25);
		glutAddMenuEntry("60", 26);
		glutAddMenuEntry("90", 27);
		glutAddMenuEntry("120", 28);
		glutAddMenuEntry("150", 29);
		glutAddMenuEntry("180", 30);
		glutAddMenuEntry("210", 31);
		glutAddMenuEntry("240", 32);
		glutAddMenuEntry("270", 33);
		glutAddMenuEntry("300", 34);
		glutAddMenuEntry("330", 35);
		glutAddMenuEntry("360", 36);

		int projection = glutCreateMenu(processMenu);
		glutAddMenuEntry("Orthographic", 37);
		glutAddMenuEntry("Perspective", 38);
		
		int headColor = glutCreateMenu(processMenu);
		glutAddMenuEntry("red", 39);
		glutAddMenuEntry("green", 40);
		glutAddMenuEntry("blue", 41);
		glutAddMenuEntry("yellow", 42);

		int armsColor = glutCreateMenu(processMenu);
		glutAddMenuEntry("red", 43);
		glutAddMenuEntry("green", 44);
		glutAddMenuEntry("blue", 45);
		glutAddMenuEntry("yellow", 46);

		int torsoColor = glutCreateMenu(processMenu);
		glutAddMenuEntry("red", 47);
		glutAddMenuEntry("green", 48);
		glutAddMenuEntry("blue", 49);
		glutAddMenuEntry("yellow", 50);

		int legsColor = glutCreateMenu(processMenu);
		glutAddMenuEntry("red", 51);
		glutAddMenuEntry("green", 52);
		glutAddMenuEntry("blue", 53);
		glutAddMenuEntry("yellow", 54);


		int menu = glutCreateMenu(processMenu);
		glutAddSubMenu("X rotation", xRotateMenu);
		glutAddSubMenu("Y rotation", yRotateMenu);
		glutAddSubMenu("Z rotation", zRotateMenu);
		glutAddSubMenu("Projection type", projection);
		glutAddSubMenu("Head color", headColor);
		glutAddSubMenu("Torso color", torsoColor);
		glutAddSubMenu("Arms color", armsColor);
		glutAddSubMenu("Legs color", legsColor);
	}
};

void processSpecialKeys(int key, int x, int y) {
		switch (key) {
		case GLUT_KEY_UP:
			//move camera
			cameraX += cameraSpeed * cameraLookX;
			cameraY += cameraSpeed * cameraLookY;
			cameraZ += cameraSpeed * cameraLookZ;
			handleCamera();
			break;
		case GLUT_KEY_DOWN:
			//move camera
			cameraX -= cameraSpeed * cameraLookX;
			cameraY -= cameraSpeed * cameraLookY;
			cameraZ -= cameraSpeed * cameraLookZ;
			handleCamera();
			break;
		case GLUT_KEY_RIGHT:
			//rotate camera
			//rotate camera
			theta += 0.1;
			handleCamera();
			break;
		case GLUT_KEY_LEFT:
			//rotate camera
			theta -= 0.1;
			handleCamera();
			break;
		case GLUT_KEY_F1:
			//Full screen vs window mode
			if (!isFullscreen) {
				glutFullScreen();  // go fullscreen
				isFullscreen = true;
			}
			else {
				glutReshapeWindow(width, height);
				glutPositionWindow(windowPosX, windowPosY);
				isFullscreen = false;
			}


			handleCamera();
			break;
		case GLUT_KEY_F2:
			//First person toggle vs entire scene view
			cameraMode1 = !cameraMode1;
			cameraMode4 = !cameraMode4;
			if (cameraMode1) {
				phi = 0;
				cameraX = 5;
				cameraY = 0;
				cameraZ = 0;

				cameraLookX = 1;
				cameraLookY = 0;
				cameraLookZ = 0;
			}
			else if (cameraMode4) {
				phi = 3.14 / 4;
			}
			handleCamera();
			break;
		case GLUT_KEY_F3:
			//disable sound



			handleCamera();
			break;
		case GLUT_KEY_F4:
			if (shadeMode == 0) { shadeMode = 1;}
			else { shadeMode = 0; }

			handleCamera();
			break;
		}
	
}

void processKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 'p':
			drawMode = 'p';
			handleCamera();
		break;
	case 'l':
		//toggle light mode
		if (lightMode == 0) { lightMode = 1; }
		else { lightMode = 0; }
		handleCamera();
		break;
	case 'w':
		drawMode = 'w';
		handleCamera();
		break;
	case 's':
		drawMode = 's';
		handleCamera();
		break;
	case 'c':
		drawMode = 'c';
		drawAxis = false;
		handleCamera();
		break;
	case 'a':
		drawAxis = !drawAxis;
		handleCamera();
		break;
	case 't':
		//TALKING BONUS SECTION
		//TALKING BONUS SECTION
		//TALKING BONUS SECTION
		PlaySoundW(TEXT("speech.wav"), NULL, SND_FILENAME | SND_ASYNC);
		std::cout << "Robot speaking" << std::endl;
		break;
	case 'd':
		dancing = !dancing;
		if (music && dancing) {
			crazyMode = !crazyMode;
			PlaySoundW(TEXT("dance.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else {
			PlaySoundW(NULL, NULL, SND_FILENAME | SND_ASYNC);
		}
		timer(0);
		break;
	case 'm':
		enemyMotion = !enemyMotion;
		break;
	case 'b':
		if (bulletSpeed == 0.01) {
			bulletSpeed = 0.02;
		}
		else if (bulletSpeed == 0.02) {
			bulletSpeed = 0.03;
		}
		else if (bulletSpeed == 0.03) {
			bulletSpeed = 0.01;
		}
		break;
	case ' ':
		// Shoot at look directrion
		bulletPos[0] = cameraX;
		bulletPos[1] = cameraY;
		bulletPos[2] = cameraZ;
		PlaySoundW(TEXT("pew.wav"), NULL, SND_FILENAME | SND_ASYNC);
		shoot(cameraLookX, cameraLookY, cameraLookZ);
		started = true;
		if (gameTime == 0) {
			gameTime = 30;
			glutTimerFunc(1000, gameTimer, 0);
		}

		break;
	case 27:
		paused = true;
		break;
	}
	
}

void switchMotion(int mult) {
	moveDir *= mult;
	glutTimerFunc(1000, switchMotion, -1);
}

void processMouse(int button, int state, int x, int y) {
	
	//unfortunately would need to be a nested switch which imo isnt much more readable than just having an if chain so here it is
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		//blackAndWhite = !blackAndWhite;
		if (state == GLUT_DOWN) {
			if (paused) {
				std::cout << x << " " << y << "\n";
				if (x > width/4.7 && y > height/4 && x <width/1.23 && y < height/2.85) {
					//newgame region
					gameTime = 30;
					score = 0;
					paused = false;
					started = false;
				}
				if (x > width / 4.7 && y > height / 2 && x < width / 1.48 && y < height / 1.66) {
					//resume
					paused = false;
				}
				if (x > width / 4.7 && y > height / 1.77 && x < width / 2.17 && y < height / 1.17) {
					//exit
					exit(0);
				}
			}
			else {
				leftDown = true;   // button pressed
			}
			
			






		}
		else if (state == GLUT_UP) {
			leftDown = false;   // button released
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}
	else if (button == 3) { //scroll up
		cameraRadius -= .1; 
		if (cameraRadius < 0.1) { cameraRadius = 0.1; }
	}
	else if (button == 4) { //scroll down
		cameraRadius += .1;
	}
	
	else {
		leftDown = false;
	}
	handleCamera();

	
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB); // RGB mode

	glutInitWindowSize(width, height); // window size
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Dakota Deets KSUID#811214188");
	glClearColor(0.5, 0.5, 0.5, 1.0);

	headTexture = loadTexture("fsmile.jpg");
	torsoTexture = loadTexture("blue.jpg");
	armTexture = loadTexture("yellow.jpg");
	legTexture = loadTexture("green.jpg");
	hurtTexture = loadTexture("red.jpg");

	grassTexture = loadTexture("grass.jpg");
	backgroundTexture = loadTexture("skybox.jpg");

	menuTexture = loadTexture("menu.jpg");
	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, (double)width / height, 0.1, zFar);
	glMatrixMode(GL_MODELVIEW);
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("f1car.fbx",aiProcess_Triangulate | aiProcess_FlipUVs);
	
	for (int i = 0; i < numOfRobots; i++) {
		robotHit[i] = false;
		enemies[i].cords[0] = randomIntFromRange(150);
		enemies[i].cords[1] = 0;
		enemies[i].cords[2] = randomIntFromRange(150);
	}



	
	gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);
	srand(time(NULL));

	menu a;

	std::cout << "PLEASE READ THIS WINDOW FOR INSTRUCTIONS\n\n" 
		<< "OLD STUFF WE DONT CARE ABOUT ANYMORE\n"
		<< "p: vertes/point only mode\n"
		<< "w: wireframe mode\n"
		<< "s: surface mode\n"
		<< "a: toggle display axis\n"
		<< "c: clear the screen\n"
		<< "Right Mouse: opens right click menu\n\n"

		<< "ASSIGNMENT 4 SPECIFIC STUFF:\n"
		<< "F4: toggle shading modw between flat\n"
		<< "L: togle diffuse light or point light\n"
		<< "ESC: bring up pause menu\n"
		<< "LASTLY: Observe imported 3d model at 0, 0, 0\n\n"


		<< "GENERAL CONTROLLS: (press or hold)\n"
		<< "F1: full screen toggle\n"
		<< "F2: swap between fpv and esv\n"
		<< "Left Mouse: control cam in esv\n\n"


		<< "GAME CONTROLLS:\n"
		<< "UP ARROW: move camera forward\n"
		<< "LEFT ARROW: rotate camera left\n"
		<< "RIGHT ARROW: rotate camera right\n"
		<< "BACK ARROW: move camera backward\n"
		<< "m: enable robot motion for higher difficulty\n"
		<< "SPACE: shoot\n"
		<< "SCORING STARTS THE MOMENT YOU FIRE YOUR FIRST BULLET\n\n"

		<< "Press ESC open the pause menu\n\n"


		<< "t: speak\n"
		<< "d: dance\n";

	glutDisplayFunc(MyDisplay); // call the drawing function
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(processMouse);
	glutMotionFunc(mouseMotion);
	glutIdleFunc(idle);
	glutTimerFunc(2000, switchMotion, -1);

	glutMainLoop();
	return 0;
}