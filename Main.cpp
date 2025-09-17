//Dakota Deets september 2025
#include <GL/glut.h>
#include <iostream>
#include <math.h>


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
#pragma endregion


//Global variables
char drawMode = 'w'; 
bool drawAxis = true; 
bool blackAndWhite = false;

int width = 800;
int height = 600;

vector3d headColor(1, 1, 0);
vector3d torsoColor(0, 0, 0.5);
vector3d rightArmColor(1, 1, 0);
vector3d leftArmColor(1, 1, 0);
vector3d rightLegColor(0, 0.75, 0);
vector3d leftLegColor(0, 0.75, 0);

double cameraRadius = 5;

long double theta = 0;
long double phi = 3.14/2;

long double cameraX = 5;
long double cameraY = 0;
long double cameraZ = 0;

long double cameraChangeX = 0;
long double cameraChangeY = 0;
long double cameraChangeZ = 0;



//this creates shapes and sends them to be displayed
void MyDisplay() {
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	

	axis origin(drawAxis);
	double saturation;
	saturation = sqrt(pow(headColor.x, 2) + pow(headColor.y, 2) + pow(headColor.z, 2));
	// robot primatives
	!blackAndWhite ? glColor3f(headColor.x, headColor.y, headColor.z) : glColor3f(saturation, saturation, saturation); // R,G,B
	box head(-0.5, -0.5, -0.5, 1, 1, 1, drawMode);

	saturation = sqrt(pow(torsoColor.x, 2) + pow(torsoColor.y, 2) + pow(torsoColor.z, 2));
	!blackAndWhite ? glColor3f(torsoColor.x, torsoColor.y, torsoColor.z) : glColor3f(saturation, saturation, saturation); // R,G,B
	box torso(-1, -2.5, -0.5, 2, 2, 1, drawMode);

	saturation = sqrt(pow(rightArmColor.x, 2) + pow(rightArmColor.y, 2) + pow(rightArmColor.z, 2));
	!blackAndWhite ? glColor3f(rightArmColor.x, rightArmColor.y, rightArmColor.z) : glColor3f(saturation, saturation, saturation); // R,G,B
	box rightArm(-2, -2.5, -0.5, 1, 2, 1, drawMode);

	saturation = sqrt(pow(leftArmColor.x, 2) + pow(leftArmColor.y, 2) + pow(leftArmColor.z, 2));
	!blackAndWhite ? glColor3f(leftArmColor.x, leftArmColor.y, leftArmColor.z) : glColor3f(saturation, saturation, saturation); // R,G,B
	box leftArm(1, -2.5, -0.5, 1, 2, 1, drawMode);

	saturation = sqrt(pow(rightLegColor.x, 2) + pow(rightLegColor.y, 2) + pow(rightLegColor.z, 2));
	!blackAndWhite ? glColor3f(rightLegColor.x, rightLegColor.y, rightLegColor.z) : glColor3f(saturation, saturation, saturation); // R,G,B
	box rigtLeg(-1, -4.5, -0.5, 1, 2, 1, drawMode);

	saturation = sqrt(pow(leftLegColor.x, 2) + pow(leftLegColor.y, 2) + pow(leftLegColor.z, 2));
	!blackAndWhite ? glColor3f(leftLegColor.x, leftLegColor.y, leftLegColor.z) : glColor3f(saturation, saturation, saturation); // R,G,B
	box leftLeg(0, -4.5, -0.5, 1, 2, 1, drawMode);

	
	// end drawing
	glFlush();
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
		//implement clearing screen and only sxowing background
		drawMode = 'c';
		drawAxis = false;
		break;
	case 'a':
		drawAxis = !drawAxis;
		break;
	case 't':
		//implement speaking functionality
		std::cout << "Robot speaking" << std::endl;
		break;
	case 'i':
		//rotate camera
		phi -= 0.1;
		break;
	case 'j':
		//rotate camera
		theta -= 0.1;
		break;
	case 'k':
		//rotate camera
		phi += 0.1;
		break;
	case 'l':
		//rotate camera
		theta += 0.1;
		break;
	case 27:
		exit(0);
		break;
	}
	
	cameraChangeX = abs(cameraX - cameraRadius * sin(phi) * cos(theta));
	cameraChangeY = abs(cameraY - cameraRadius * cos(phi));
	cameraChangeZ = abs(cameraZ - cameraRadius * sin(phi) * sin(theta));


	cameraX = cameraRadius * sin(phi) * cos(theta);
	cameraZ = cameraRadius * sin(phi) * sin(theta);
	cameraY = cameraRadius * cos(phi);

	glLoadIdentity();
	gluPerspective(100, (double)width / height, 0, 100);
	gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);
	glutPostRedisplay();
}

void processMouse(int button, int state, int x, int y) {
	
	//unfortunately would need to be a nested switch which imo isnt much more readable than just having an if chain so here it is
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		blackAndWhite = !blackAndWhite;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

	}
	else if (button == 3) { //scroll up
		cameraRadius -= .1; 
		if (cameraRadius < 0.1) { cameraRadius = 0.1; }
	}
	else if (button == 4) { //scroll down
		cameraRadius += .1;
	}
	


	cameraChangeX = abs(cameraX - cameraRadius * sin(phi) * cos(theta));
	cameraChangeY = abs(cameraY - cameraRadius * cos(phi));
	cameraChangeZ = abs(cameraZ - cameraRadius * sin(phi) * sin(theta));


	cameraX = cameraRadius * sin(phi) * cos(theta);
	cameraZ = cameraRadius * sin(phi) * sin(theta);
	cameraY = cameraRadius * cos(phi);

	glLoadIdentity();
	gluPerspective(100, (double)width / height, 0, 100);
	gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);
	glutPostRedisplay();
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


	std::cout << "p: vertes/point only mode\n"
		<< "w: wireframe mode\n"
		<< "s: surface mode\n"
		<< "a: toggle display axis\n"
		<< "c: clear the screen\n"
		<< "Right Mouse: display menu\n"
		<< "Left Mouse: toggle BW and Color\n"
		<< "\n\nCAMERA CONTROLLS: (press or hold)\n"
		<< "i: rotate camera up\n"
		<< "j: rotate camera left\n"
		<< "k: rotate camera right\n"
		<< "l: rotate camera left\n"
		<< "SCROLL WHEEL: zoom in and out\n";

	glutDisplayFunc(MyDisplay); // call the drawing function
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouse);
	glutMainLoop();
	return 0;
}