//Dakota Deets september 2025
#include <GL/glut.h>
#include <iostream>
#include <math.h> //only used for sqrt and pow cause im lazy
#include <Windows.h>
#include <mmsystem.h>
//Global variables

bool drawAxis = true;
bool blackAndWhite = false;
char drawMode = 'w';
bool music = true;
bool dancing = false;

int width = 800;
int height = 600;



double cameraSpeed = 1;
double danceTheta = 0;

long double theta = 0;
long double phi = 3.14 / 2;

int cameraMode = 4;

double cameraRadius = 5;
long double cameraX = 5;
long double cameraY = 0;
long double cameraZ = 0;

long double cameraLookX = 0;
long double cameraLookY = 0;
long double cameraLookZ = -1;

//needs to be before variables because no hoist
#pragma region class definitions



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
	box(double x, double y, double z, double l, double h, double w, char mode) {
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
			glBegin(GL_QUADS);
			glVertex3d(x, y, z);
			glVertex3d(x + l, y, z);
			glVertex3d(x + l, y + h, z);
			glVertex3d(x + l, y, z);

			glVertex3d(x, y, z);
			glVertex3d(x, y, z + w);
			glVertex3d(x, y + h, z + w);
			glVertex3d(x, y + h, z);

			glVertex3d(x, y, z);
			glVertex3d(x + l, y, z);
			glVertex3d(x + l, y, z + w);
			glVertex3d(x, y, z + w);

			glVertex3d(x, y, z + w);
			glVertex3d(x, y + h, z + w);
			glVertex3d(x + l, y + h, z + w);
			glVertex3d(x + l, y, z + w);

			glVertex3d(x, y + h, z);
			glVertex3d(x + l, y + h, z);
			glVertex3d(x + l, y + h, z + w);
			glVertex3d(x, y + h, z + w);

			glVertex3d(x + l, y, z);
			glVertex3d(x + l, y, z + w);
			glVertex3d(x + l, y + h, z + w);
			glVertex3d(x + l, y + h, z);

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


class robot {
public:
	robot(double x, double y, double z, double saturation) {
		// robot primatives
		!blackAndWhite ? glColor3f(headColor.x, headColor.y, headColor.z) : glColor3f(saturation, saturation, saturation); // R,G,B
		box head(-0.5+x, -0.5+y, -0.5+z, 1, 1, 1, drawMode);

		saturation = sqrt(pow(torsoColor.x, 2) + pow(torsoColor.y, 2) + pow(torsoColor.z, 2));
		!blackAndWhite ? glColor3f(torsoColor.x, torsoColor.y, torsoColor.z) : glColor3f(saturation, saturation, saturation); // R,G,B
		box torso(-1+x, -2.5+y, -0.5+z, 2, 2, 1, drawMode);


		saturation = sqrt(pow(rightArmColor.x, 2) + pow(rightArmColor.y, 2) + pow(rightArmColor.z, 2));
		!blackAndWhite ? glColor3f(rightArmColor.x, rightArmColor.y, rightArmColor.z) : glColor3f(saturation, saturation, saturation); // R,G,B
		box rightArm(-2+x, (!dancing ? -2.5 : sin(danceTheta))+y, -0.5+z, 1, 2, 1, drawMode);

		saturation = sqrt(pow(leftArmColor.x, 2) + pow(leftArmColor.y, 2) + pow(leftArmColor.z, 2));
		!blackAndWhite ? glColor3f(leftArmColor.x, leftArmColor.y, leftArmColor.z) : glColor3f(saturation, saturation, saturation); // R,G,B
		box leftArm(1+x, (!dancing ? -2.5 : cos(danceTheta))+y, -0.5+z, 1, 2, 1, drawMode);

		saturation = sqrt(pow(rightLegColor.x, 2) + pow(rightLegColor.y, 2) + pow(rightLegColor.z, 2));
		!blackAndWhite ? glColor3f(rightLegColor.x, rightLegColor.y, rightLegColor.z) : glColor3f(saturation, saturation, saturation); // R,G,B
		box rigtLeg(-1+x, -4.5+y, -0.5+z, 1, 2, 1, drawMode);

		saturation = sqrt(pow(leftLegColor.x, 2) + pow(leftLegColor.y, 2) + pow(leftLegColor.z, 2));
		!blackAndWhite ? glColor3f(leftLegColor.x, leftLegColor.y, leftLegColor.z) : glColor3f(saturation, saturation, saturation); // R,G,B
		box leftLeg(0+x, -4.5+y, -0.5+z, 1, 2, 1, drawMode);
	}
};


#pragma endregion



void handleCamera(int cameraMode) {
	switch (cameraMode) {
	case 1:
		cameraLookX = 1 * sin(phi) * cos(theta);
		cameraLookZ = 1 * sin(phi) * sin(theta);
		cameraLookY = 1 * cos(phi);

		glLoadIdentity();
		gluPerspective(100, (double)width / height, 0, 100);
		gluLookAt(cameraX, cameraY, cameraZ, cameraLookX + cameraX, cameraLookY + cameraY, cameraLookZ + cameraZ, 0, 1, 0);
		glutPostRedisplay();
		
		break;
	case 4:
		cameraX = cameraRadius * sin(phi) * cos(theta);
		cameraZ = cameraRadius * sin(phi) * sin(theta);
		cameraY = cameraRadius * cos(phi);

		glLoadIdentity();
		gluPerspective(100, (double)width / height, 0, 100);
		gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);
		glutPostRedisplay();
		break;

	}


	
}


//this creates shapes and sends them to be displayed
void MyDisplay() {
	
	glClear(GL_COLOR_BUFFER_BIT);
	

	axis origin(drawAxis);
	double saturation;
	saturation = sqrt(pow(headColor.x, 2) + pow(headColor.y, 2) + pow(headColor.z, 2));
	
	robot steve(0.0, 0.0, 0.0, saturation);
	robot jim(5.0, 0.0, 5.0, saturation);
	robot fred(-5.0, 0.0, 5.0, saturation);
	robot dan(5.0, 0.0, -5.0, saturation);
	robot ryan(-5.0, 0.0, -5.0, saturation);
	// end drawing
	glFlush();
}

void timer(int value) {
	danceTheta += 0.3;
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
	case 37: gluPerspective(100, (double)width / height, 0, 100); break;
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
			handleCamera(cameraMode);
			break;
		case GLUT_KEY_DOWN:
			//move camera
			cameraX -= cameraSpeed * cameraLookX;
			cameraY -= cameraSpeed * cameraLookY;
			cameraZ -= cameraSpeed * cameraLookZ;
			handleCamera(cameraMode);
			break;
		case GLUT_KEY_RIGHT:
			//move camera
			if (cameraLookX < 0 && cameraLookZ < 0) {
				cameraZ += cameraSpeed * cameraLookX;
				cameraX -= cameraSpeed * cameraLookZ;
			}
			else if (cameraLookX >= 0 && cameraLookZ < 0) { 
				cameraZ += cameraSpeed * cameraLookX;
				cameraX -= cameraSpeed * cameraLookZ;
			}
			else if (cameraLookX < 0 && cameraLookZ >= 0) { 
				cameraZ += cameraSpeed * cameraLookX;
				cameraX -= cameraSpeed * cameraLookZ;
			}
			else if (cameraLookX >= 0 && cameraLookZ >= 0) { 
				cameraZ += cameraSpeed * cameraLookX;
				cameraX -= cameraSpeed * cameraLookZ;
			}


			std::cout << "coords: " << cameraX << " " << cameraZ << "\n";
			std::cout << "changes: " << cameraSpeed * cameraLookX << " " << cameraSpeed * cameraLookZ << "\n";
			

			handleCamera(cameraMode);
			break;
		case GLUT_KEY_LEFT:
			//move camera
			if (cameraLookX < 0 && cameraLookZ < 0) {
				cameraZ -= cameraSpeed * cameraLookX;
				cameraX += cameraSpeed * cameraLookZ;
			}
			else if (cameraLookX >= 0 && cameraLookZ < 0) {
				cameraZ -= cameraSpeed * cameraLookX;
				cameraX += cameraSpeed * cameraLookZ;
			}
			else if (cameraLookX < 0 && cameraLookZ >= 0) {
				cameraZ -= cameraSpeed * cameraLookX;
				cameraX += cameraSpeed * cameraLookZ;
			}
			else if (cameraLookX >= 0 && cameraLookZ >= 0) {
				cameraZ -= cameraSpeed * cameraLookX;
				cameraX += cameraSpeed * cameraLookZ;
			}
			handleCamera(cameraMode);
			break;
		case GLUT_KEY_F1:
			//First Person Camera mode
			cameraX = 5;
			cameraY = 0;
			cameraZ = 0;

			cameraLookX = 1;
			cameraLookY = 0;
			cameraLookZ = 0;
			cameraMode = 1;
			handleCamera(cameraMode);
			break;
		case GLUT_KEY_F2:
			//First person + mirror mode
			cameraMode = 2;
			handleCamera(cameraMode);
			break;
		case GLUT_KEY_F3:
			//whole scene mode
			cameraMode = 3;
			handleCamera(cameraMode);
			break;
		case GLUT_KEY_F4:
			//orbit camera mode
			cameraX = 5;
			cameraY = 0;
			cameraZ = 0;

			cameraLookX = 0;
			cameraLookY = 0;
			cameraLookZ = 0;
			cameraMode = 4;
			handleCamera(cameraMode);
			break;
	}
}

void processKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 'p':
		drawMode = 'p';
		break;
	case 'w':
		drawMode = 'w';
		break;
	case 's':
		drawMode = 's';
		break;
	case 'c':
		drawMode = 'c';
		drawAxis = false;
		break;
	case 'a':
		drawAxis = !drawAxis;
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
		if (music) {
			PlaySoundW(TEXT("dance.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else {
			PlaySoundW(NULL, NULL, SND_FILENAME | SND_ASYNC);
		}
		timer(0);
		break;
	case 'm':
		music = !music;
		music ? NULL : PlaySoundW(NULL, NULL, SND_FILENAME | SND_ASYNC);
		break;
	case 'i':
		//rotate camera
		phi -= 0.1;
		handleCamera(cameraMode);
		break;
	case 'j':
		//rotate camera
		theta -= 0.1;
		handleCamera(cameraMode);
		break;
	case 'k':
		//rotate camera
		phi += 0.1;
		handleCamera(cameraMode);
		break;
	case 'l':
		//rotate camera
		theta += 0.1;
		handleCamera(cameraMode);
		break;
	
	case 27:
		exit(0);
		break;
	}
	
}

void processMouse(int button, int state, int x, int y) {
	
	//unfortunately would need to be a nested switch which imo isnt much more readable than just having an if chain so here it is
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		blackAndWhite = !blackAndWhite;
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
	
	handleCamera(cameraMode);

	
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB); // RGB mode
	
	glutInitWindowSize(width, height); // window size
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Dakota Deets KSUID#811214188");
	glClearColor(0.0, 0.0, 0.0, 1.0); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluPerspective(100, (double)width / height, 0, 100);
	

	
	gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);
	
	
	menu a;

	std::cout << "p: vertes/point only mode\n"
		<< "w: wireframe mode\n"
		<< "s: surface mode\n"
		<< "a: toggle display axis\n"
		<< "c: clear the screen\n"
		<< "Right Mouse: opens right click menu\n"
		<< "Left Mouse: toggle BW and Color\n"
		<< "\n\nCAMERA CONTROLLS: (press or hold)\n"
		<< "i: rotate camera up\n"
		<< "j: rotate camera left\n"
		<< "k: rotate camera right\n"
		<< "l: rotate camera left\n"
		<< "SCROLL WHEEL: zoom in and out\n\n\n"
		<< "t: speak\n"
		<< "d: dance\n";

	glutDisplayFunc(MyDisplay); // call the drawing function
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(processMouse);
	
	glutMainLoop();
	return 0;
}