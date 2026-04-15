//Dakota Deets feb 2026
//SPRITE SHEET AND ANIMATOR ARE MY BONUS


#include <GL/glut.h>
#include <iostream>
#include <math.h> //only used for sqrt and pow cause im lazy
#include <Windows.h>
#include <thread>
#include <mmsystem.h>
#include <vector>
#include <string>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//super ultra mega lobal variable
bool paused = true;

//class definitions
GLuint loadTexture(const char* filename)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
	GLuint texture;



	if (!data) {
		std::cout << "no texture loaded\n";
		return 0;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//alpha or no alpha
	GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	stbi_image_free(data);
	return texture;
}

#pragma region class definitions

class point {
public:
	double x;
	double y;

	point() {}

	point(double px, double py) {
		x = px;
		y = py;
	}

	void setPosition(double px, double py) {
		x = px;
		y = py;
	}
};


class vector2d {
public:
	double x;
	double y;

	vector2d() {
		x = 0;
		y = 0;
	}

	vector2d(double a, double b) {
		x = a;
		y = b;
	}

	void setVector(double a, double b) {
		x = a;
		y = b;
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

	void setVector(double a, double b, double c) {
		x = a;
		y = b;
		z = c;
	}
};


class axis {
public:
	double x;
	double y;

	void draw() {
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(x, y);
		glVertex2f(x + 100, y);
		glColor3f(0, 1, 0);
		glVertex2f(x, y);
		glVertex2f(x, y + 100);
		glEnd();
	}


	void setPos(double px, double py) {
		x = px;
		y = py;
	}
};

class square {
public:
	double s = 0;
	double x = 0;
	double y = 0;
	vector3d color = vector3d(1, 1, 1);

	square() {
		s = 1;
		x = 0;
		y = 0;
	}

	square(double l, double px, double py, vector3d c) {
		s = l;
		x = px;
		y = py;
		color = c;
	}

	void setColor(vector3d c) {
		color = c;
	}

	void setPos(double px, double py) {
		x = px;
		y = py;
	}

	void draw() {
		glColor3f(color.x, color.y, color.z);
		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x, y + s);
		glVertex2f(x + s, y + s);
		glVertex2f(x + s, y);
		glEnd();
	}
};

class triangle {
public:
	double b = 0;
	double h = 0;
	double x = 0;
	double y = 0;
	vector3d color = vector3d(1, 1, 1);

	triangle() {
		b = 0;
		h = 0;
		x = 0;
		y = 0;
	}

	triangle(double base, double height, double px, double py, vector3d c) {
		b = base;
		h = height;
		x = px;
		y = py;
		color = c;
	}

	void setColor(vector3d c) {
		color = c;
	}

	void setPos(double px, double py) {
		x = px;
		y = py;
	}

	void draw() {
		glColor3f(color.x, color.y, color.z);
		glBegin(GL_TRIANGLES);
		glVertex2f(x, y);
		glVertex2f(x + b, y);
		glVertex2f(x + (b / 2), y + h);
		glEnd();
	}
};

//SPRITE SHEET BONUS CODE BELOW

class spriteSheet {
public:
	GLuint texture;
	int columns;
	int rows;
	std::vector<int> framesPerRow; //keeps track of sorter vs longer animation rows

	spriteSheet() {}

	spriteSheet(const char* file, int cols, int r, const std::vector<int>& frames)
	{
		texture = loadTexture(file);
		columns = cols;
		rows = r;
		framesPerRow = frames;
	}

	double getFrameWidth() {
		return 1.0 / columns;
	}

	double getFrameHeight() {
		return 1.0 / rows;
	}

	int getFramesInRow(int row) {
		return framesPerRow[row];
	}

};


//ANIMATOR BONUS CODE BELOW
class animator {
public:
	spriteSheet* sheet;

	int currentFrame = 0;
	int currentRow = 0;


	double frameTime = 0.03;
	double timer = 0.0;

	animator() {}

	animator(spriteSheet* s)
	{
		sheet = s;
	}

	void setAnimation(int row)
	{
		if (currentRow != row) {
			currentRow = row;
			currentFrame = 0;
			timer = 0.0;
		}
	}

	void update(double deltaTime, bool paused)
	{
		if (!paused) {
			timer += deltaTime;

			if (timer >= frameTime) {
				int maxFrames = sheet->getFramesInRow(currentRow);
				currentFrame = (currentFrame + 1) % maxFrames;
				timer = 0.0;
			}
		}
		
	}

	void draw(double x, double y, double width, double height)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, sheet->texture);

		double frameW = sheet->getFrameWidth();
		double frameH = sheet->getFrameHeight();

		//UV mapping
		double u0 = currentFrame * frameW;
		double v0 = 1.0 - ((currentRow + 1) * frameH);;
		double u1 = u0 + frameW;
		double v1 = v0 + frameH;

		glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
		glTexCoord2f(u0, v0); glVertex2f(x, y);
		glTexCoord2f(u1, v0); glVertex2f(x + width, y);
		glTexCoord2f(u1, v1); glVertex2f(x + width, y + height);
		glTexCoord2f(u0, v1); glVertex2f(x, y + height);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}
};

class player {
public:
	double x = 0;
	double y = 0;
	double w = 0;
	double h = 0;

	spriteSheet* s;
	animator a;

	player(double px, double py, double width, double height, spriteSheet* sheet) {
		x = px;
		y = py;
		w = width;
		h = height;
		s = sheet;
		a = animator(s);
	}

	void setAnimation(int row) {
		a.setAnimation(row);
	}

	void update(double dt)
	{
		a.update(dt, paused);
	}

	void draw() {
		a.draw(x, y, w, h);
	}

	void move(double dx, double dy) {
		x += dx;
		y += dy;
	}
};


//a block is any wall the player can colide with, moving or otherwise
class block {
public:
	double x;
	double y;
	double w; //width and heigt of hitbox (and texture)
	double h;

	spriteSheet* s;
	animator a;

	block(double px, double py, double width, double height, spriteSheet* sheet) {
		x = px;
		y = py;
		w = width;
		h = height;
		s = sheet;
		a = animator(s);
	}

	void setAnimation(int row) {
		a.setAnimation(row);
	}

	void update(double dt, bool paused)
	{
		a.update(dt, paused);
	}

	void draw() {
		a.draw(x, y, w, h);
	}

	void move(double dx, double dy) {
		x += dx;
		y += dy;
	}
};


//a coin is a collectable the player can colide with
class coin {
public:
	double x;
	double y;
	double w; //width and heigt of hitbox (and texture)
	double h;

	int value; //worth of coin

	spriteSheet* s;
	animator a;

	coin(double px, double py, double width, double height, spriteSheet* sheet, int v) {
		x = px;
		y = py;
		w = width;
		h = height;
		s = sheet;
		value = v;
		a = animator(s);
	}

	void setAnimation(int row) {
		a.setAnimation(row);
	}

	void update(double dt)
	{
		a.update(dt, paused);
	}

	void draw() {
		a.draw(x, y, w, h);
	}

	void move(double dx, double dy) {
		x += dx;
		y += dy;
	}
};

//a goal is anything the player can leave the current level with, to another level or the end of the game ect.
class goal {
public:
	double x;
	double y;
	double w; //width and heigt of hitbox (and texture)
	double h;

	spriteSheet* s;
	animator a;

	int destination;

	goal(){}

	goal(double px, double py, double width, double height, spriteSheet* sheet, int id) {
		x = px;
		y = py;
		w = width;
		h = height;
		s = sheet;
		a = animator(s);
		destination = id;
	}

	void setAnimation(int row) {
		a.setAnimation(row);
	}

	void update(double dt)
	{
		a.update(dt, paused);
	}

	void draw() {
		a.draw(x, y, w, h);
	}

	void move(double dx, double dy) {
		x += dx;
		y += dy;
	}
};

//a collection of blocks, goals and collectables
class level {
public:
	std::vector<block> walls;
	std::vector<coin> coins;

	goal end;

	level() {}

	level(std::vector<block>& w, std::vector<coin>& c, goal e) {
		walls = w;
		coins = c;
		end = e;
	}

	void draw(){
		for (block b : walls) {
			b.draw();
		}
		for (coin c : coins) {
			c.draw();
		}
		end.draw();
	}

};

class sceneManager {
public:
	std::vector<level> stages;
	int currentStage;

	sceneManager(){}

	sceneManager(std::vector<level> s, int i) {
		stages = s;
		currentStage = i;
	}


	void drawScene() {
		stages[currentStage].draw();
	}

	void setStage(int i) {
		currentStage = i;
	}


};


class textBox {
public:
	double x;
	double y;
	double w;
	double h;

	std::string text = "";

	vector3d boxColor;
	vector3d textColor;

	bool inFocus;

	int caratPos = 0;

	textBox() {}

	textBox(double px, double py, double width, double height, std::string t, vector3d bC, vector3d tC, bool focused) {
		x = px;
		y = py;
		w = width;
		h = height;
		text = t;
		boxColor = bC;
		textColor = tC;
		inFocus = focused;
	}

	void setText(std::string newText) {
		text = newText;
	}

	void draw(player *p) {
		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
		glVertex2f(x + p->x, y + p->y);
		glVertex2f(x + p->x, y + h + p->y);
		glVertex2f(x + w + p->x, y + h + p->y);
		glVertex2f(x + w + p->x, y + p->y);
		glEnd();

		//render text
		glColor3f(textColor.x, textColor.y, textColor.z);
		glRasterPos2d(x + p->x + 2, y + (h / 2) + p->y);
		for (int i = 0; i < text.size(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[i]);
		}
	}


	void clicked() {
		inFocus = true;
	}

	void focusLost() {
		inFocus = false;
	}

};

class label {
public:
	double x;
	double y;


	std::string text = "";

	vector3d textColor;


	label() {}

	label(double px, double py, std::string t, vector3d tC) {
		x = px;
		y = py;
		text = t;
		textColor = tC;
	}

	void setText(std::string newText) {
		text = newText;
	}

	void draw(player* p) {
		//render text
		glColor3f(textColor.x, textColor.y, textColor.z);
		glRasterPos2d(x + p->x + 2, y + p->y);
		for (int i = 0; i < text.size(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[i]);
		}
	}

};

//buttons dont actually do anything, all functionality is tucked away in classes 
class button {
public:
	double height;
	double width;
	double x;
	double y;

	std::string text;

	vector3d boxColor;
	vector3d textColor;

	bool isVisible;

	bool buttonValueToggle;

	button() {}

	button(double h, double w, double px, double py, vector3d bColor, vector3d tColor, bool visible, bool value, std::string t) {
		height = h;
		width = w;
		x = px;
		y = py;
		boxColor = bColor;
		textColor = tColor;
		isVisible = visible;
		buttonValueToggle = value;
		text = t;
	}

	//NEEDS to be in relation to the player cause my game logic is so fucked smh
	void draw(player *p) {
		if (isVisible) {
			//draw box
			glColor3f(boxColor.x, boxColor.y, boxColor.z);
			glBegin(GL_QUADS);
			glVertex2f(x + p->x, y + p->y);
			glVertex2f(x + p->x, y + height + p->y);
			glVertex2f(x + width + p->x, y + height + p->y);
			glVertex2f(x + width + p->x, y + p->y);
			glEnd();

			//render text
			glColor3f(textColor.x, textColor.y, textColor.z);
			glRasterPos2d(x + p->x + 2, y + (height / 2) + p->y);
			for (int i = 0; i < text.size(); i++) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[i]);
			}
		}
		

	}


};

class gameObjectPannel {
public:
	std::vector<button> gameObjects;

	int xOffset = -605;
	int yOffset = -130;
	gameObjectPannel() {}

	gameObjectPannel(level l) {
		int bY = 530;
		int blockCount = 1;
		int coinCount = 1;
		for (block w : l.walls) {
			gameObjects.push_back(button(50, 120, 10 + xOffset, bY + yOffset, vector3d(.5, .5, .5), vector3d(0, 0, 0), 1, 0, ("block" + std::to_string(blockCount))));
			bY -= 60;
			blockCount += 1;
		}
		for (coin c : l.coins) {
			gameObjects.push_back(button(50, 120, 10 + xOffset, bY + yOffset, vector3d(.5, .5, .5), vector3d(0, 0, 0), 1, 0, ("coin" + std::to_string(coinCount))));
			bY -= 60;
			coinCount += 1;
		} 
	}

	void draw(player *p) {
		glColor3f(.2, .2, .2);
		glBegin(GL_QUADS);
		glVertex2f(xOffset + p->x, yOffset + p->y);
		glVertex2f(xOffset + p->x, yOffset + 600 + p->y);
		glVertex2f(xOffset + 160 + p->x, yOffset + 600 + p->y);
		glVertex2f(xOffset + 160 + p->x, yOffset + p->y);
		glEnd();

		for (button b : gameObjects) {
			b.draw(p);
		}
	}
};


class propertiesPannel {
public:
	std::vector<textBox> properties;

	int pID;
	int pType;


	int xOffset = 510;
	int yOffset = -130;

	propertiesPannel() {}

	propertiesPannel(block b, int parentID) {
		properties.push_back(textBox(520, yOffset + 500, 100, 50, std::to_string(b.x), vector3d(1, 1, 1), vector3d(0, 0 ,0), 0));
		properties.push_back(textBox(520, yOffset + 440, 100, 50, std::to_string(b.y), vector3d(1, 1, 1), vector3d(0, 0, 0), 0));
		pID = parentID;
		pType = 1;
	}

	propertiesPannel(coin c, int parentID) {
		properties.push_back(textBox(520, yOffset + 500, 100, 50, std::to_string(c.x), vector3d(1, 1, 1), vector3d(0, 0, 0), 0));
		properties.push_back(textBox(520, yOffset + 440, 100, 50, std::to_string(c.y), vector3d(1, 1, 1), vector3d(0, 0, 0), 0));
		properties.push_back(textBox(520, yOffset + 380, 100, 50, std::to_string(c.value), vector3d(1, 1, 1), vector3d(0, 0, 0), 0));
		pID = parentID;
		pType = 2;

	}

	void draw(player* p) {
		glColor3f(.2, .2, .2);
		glBegin(GL_QUADS);
		glVertex2f(xOffset + p->x, yOffset + p->y);
		glVertex2f(xOffset + p->x, yOffset + 600 + p->y);
		glVertex2f(xOffset + 160 + p->x, yOffset + 600 + p->y);
		glVertex2f(xOffset + 160 + p->x, yOffset + p->y);
		glEnd();

		for (textBox tB : properties) {
			tB.draw(p);
		}
	}


	void focusLost() {
		properties.clear();
	}
};

class editor {
public:
	std::vector<button> editorButtons;
	
	bool fileMenu = false;
	bool helpMenu = false;

	//define all buttons
	button file;
	button quit;
	button help;
	button about;
	button controls;
	button contact;

	//define 'level'
	std::vector<block> editorBlocks;
	std::vector<coin> editorCoins;
	std::vector<goal> editorGoals;
	level editorLevel;

	gameObjectPannel leftPannel;

	propertiesPannel rightPannel;

	editor(double screenHeight, double screenWidth, level l) {
		//define all buttons
		file = button(30, 20, -605, (screenHeight/2) - 10, vector3d(.5, .5, .5), vector3d(0, 0, 0), 1, 0, std::string("file"));
		quit = button(30, 20, -605, (screenHeight / 2) - 40, vector3d(.3, .3, .3), vector3d(0, 0, 0), 0, 0, std::string("quit"));


		help = button(30, 40, -585, (screenHeight / 2) - 10, vector3d(.5, .5, .5), vector3d(0, 0, 0), 1, 0, std::string("help"));
		about = button(30, 40, -585, (screenHeight / 2) - 40, vector3d(.3, .3, .3), vector3d(0, 0, 0), 0, 0, std::string("about"));
		controls = button(30, 40, -585, (screenHeight / 2) - 70, vector3d(.3, .3, .3), vector3d(0, 0, 0), 0, 0, std::string("controls"));
		contact = button(30, 40, -585, (screenHeight / 2) - 100, vector3d(.3, .3, .3), vector3d(0, 0, 0), 0, 0, std::string("contact"));

		//push all buttons to list
		editorButtons.push_back(file);
		editorButtons.push_back(quit);

		editorButtons.push_back(help);
		editorButtons.push_back(about);
		editorButtons.push_back(controls);
		editorButtons.push_back(contact);

		editorLevel = l;
		editorBlocks = l.walls;
		editorCoins = l.coins;
		editorGoals.clear();
		editorGoals.push_back(l.end);

		leftPannel = gameObjectPannel(l);

	}
	
	void draw(player *p) {

		leftPannel.draw(p);

		rightPannel.draw(p);

		for (button b : editorButtons) {
				b.draw(p);
			
		}

		

	}
	
	void fileClicked() {
		fileMenu = true;
	}

	void helpClicked() {
		helpMenu = true;
	}

	void focusLost() {
		fileMenu = false;
		helpMenu = false;
	}

	void aboutClicked() {
		helpMenu = true;
	}

	void contactClicked() {
		helpMenu = true;
	}

	void controllsClicked() {
		std::cout << "CONTROLLS:\n";
		std::cout << "arrow keys: move\n"
			<< "space: jump\n"
			<< "a: show axis in editor\n"
			<< "g: reset level/game\n";
	}
	
	void updateVisibility() {
		if (fileMenu) {
			editorButtons[1].isVisible = true; //1 is the code for the quit button
		}else{
			editorButtons[1].isVisible = false;
		}
		if (helpMenu) {
			editorButtons[3].isVisible = true; //3 is about
			editorButtons[4].isVisible = true; //4 is controls
			editorButtons[5].isVisible = true; //5 is contact
		}
		else {
			editorButtons[3].isVisible = false;
			editorButtons[4].isVisible = false;
			editorButtons[5].isVisible = false;
		}
	}


};

#pragma endregion



#pragma region global variables
//global variables

//player sprite offsets
double ox = 35;
double oy = 15;

player* p;
spriteSheet* playerSprite;
square redSquare(30, 50, 300, vector3d(1, 0, 0));
triangle greenTriangle(30, 30, 200, 300, vector3d(0, 1, 0));
axis a;

int screenWidth = 1280;
int screenHeight = 960;

bool jumped = false;

double camX;
double camY;

double cameraWidth = 640;
double cameraHeight = 480;
point screenCenter = point(320, 240);

double lastTime = 0; //for keeping track of delta time 

bool drawAxis = true;

bool drawing = true;

bool squareTriangleState = false;

bool playMusic = true;
int musicTrack = 3;

double vx = 0;
double vy = 0;

int textBoxFocusedId;

//key press flags
bool left = false;
bool right = false;
bool up = false;
bool down = false;


int score = 0;
int gameTime = 60; //measured in seconds
int frameCount = 0;
bool gameEnd = false;
bool title = true;
bool win = false;


std::vector<int> noAnimationList;

spriteSheet* brickSprite;
spriteSheet* coinSprite;
spriteSheet* portalSprite;

std::vector<coin> coins;
std::vector<block> walls;
goal firstLevelEnd;

level firstLevel;
sceneManager scenes;


editor e(screenHeight, screenWidth, firstLevel);




#pragma endregion


#pragma region glut funcs and game logic

void playSound(const char* filename, const char* alias, bool loop) {
	if (loop) {
		// stop any existing loop first
		PlaySoundA(NULL, NULL, 0);
		std::thread([filename]() {
			PlaySoundA(filename, NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
			}).detach();
	}
	else {
		char cmd[256];
		char errMsg[256];
		MCIERROR err;

		sprintf_s(cmd, "close %s", alias);
		mciSendStringA(cmd, NULL, 0, NULL);

		sprintf_s(cmd, "open \"%s\" alias %s", filename, alias);
		err = mciSendStringA(cmd, NULL, 0, NULL);
		if (err) {
			mciGetErrorStringA(err, errMsg, 256);
			printf("open error: %s\n", errMsg);
			return;
		}

		sprintf_s(cmd, "play %s from 0", alias);
		err = mciSendStringA(cmd, NULL, 0, NULL);
		if (err) {
			mciGetErrorStringA(err, errMsg, 256);
			printf("play error: %s\n", errMsg);
		}
	}
}

void stopSound(const char* alias) {
	char cmd[256];
	sprintf_s(cmd, "close %s", alias);
	mciSendStringA(cmd, NULL, 0, NULL);
	PlaySoundA(NULL, NULL, SND_PURGE);
}


//aabb, we ddont need anything fancy yet
bool willPlayerCollide(player * p, double dx, double dy, std::vector<block>& blocks, std::vector<coin>& coins, goal end) {

	//player and blocks
	for (block b : blocks) {
		double pLeft = p->x + dx + ox;
		double pRight = p->x + (p->w) + dx - ox;
		double pBottom = p->y + dy + oy;
		double pTop = p->y + (p->h) + dy -oy;

		double bLeft = b.x;
		double bRight = b.x + (b.w);
		double bBottom = b.y;
		double bTop = b.y + (b.h);

		
		if (pRight > bLeft && pLeft < bRight && pTop > bBottom && pBottom < bTop) {
			jumped = false;
			return true;
		}
	}
	//player and coins
	//actual itterator based loop because we may need to remove coins after colelcted
	for (auto c = coins.begin(); c != coins.end();) {
		double pLeft = p->x + dx + ox;
		double pRight = p->x + (p->w) + dx - ox;
		double pBottom = p->y + dy + oy;
		double pTop = p->y + (p->h) + dy - oy;

		double cLeft = c->x;
		double cRight = c->x + (c->w);
		double cBottom = c->y;
		double cTop = c->y + (c->h);

		if (pRight > cLeft && pLeft < cRight && pTop > cBottom && pBottom < cTop) {
			score += c->value;
			c =coins.erase(c);
			playSound("sounds\\\\coin.wav", "win", 0);
		}
		else {
			++c;
		}
	}
	

	//player and goal
	double pLeft = p->x + dx + ox;
	double pRight = p->x + (p->w) + dx - ox;
	double pBottom = p->y + dy + oy;
	double pTop = p->y + (p->h) + dy - oy;

	double gLeft = end.x;
	double gRight = end.x + (end.w);
	double gBottom = end.y;
	double gTop = end.y + (end.h);

	if (pRight > gLeft && pLeft < gRight && pTop > gBottom && pBottom < gTop) {
		stopSound("shopLoop");
		playSound("sounds\\\\ff7win.wav", "win", 0);
		win = true;
	}

	return false;
}


void chooseMusic(int x) {
	switch (x) {
	case 1:
		playSound("sounds\\\\dance.wav", "dance", 1);
		break;
	case 2:
		playSound("sounds\\\\theBreakCoreSideQuest.wav", "breakCore", 1);
		break;
	case 3:
		playSound("sounds\\\\shopLoop.wav", "shopLoop" , 1);
		break;
	default:
		stopSound("game");
		stopSound("dance");
		stopSound("breakcore");
		break;
	}
}

//this is where i plan to declare all my static assets
void declareModels() {
	
	if (drawing && !title) {
		redSquare.draw();
		greenTriangle.draw();
		p->draw();
		scenes.drawScene();



		if (drawAxis) {
			a.draw();
		}
	}
	
}


void mouseMotion(int x, int y) {

}


void buildLevels() {
	noAnimationList.push_back(1);
	brickSprite = new spriteSheet("textures\\\\brick.png", 1, 1, noAnimationList);
	coinSprite = new spriteSheet("textures\\\\coin.png", 1, 1, noAnimationList);
	portalSprite = new spriteSheet("textures\\\\portal.png", 1, 1, noAnimationList);


	block b(0, 32, 256, 64, brickSprite);
	walls.push_back(b);
	b = block(256, 64, 256, 64, brickSprite);
	walls.push_back(b);
	b = block(512, 32, 256, 64, brickSprite);
	walls.push_back(b);
	b = block(768, 32, 256, 64, brickSprite);

	coin c(300, 128, 64, 64, coinSprite, 1);
	coins.push_back(c);
	c = coin(600, 96, 64, 64, coinSprite, 1);
	coins.push_back(c);

	goal end(700, 128, 64, 64, portalSprite, 0);
	firstLevelEnd = end;




	level l(walls, coins, end);
	firstLevel = l;


	std::vector<level> levels;
	levels.push_back(firstLevel);
	sceneManager s(levels, 0);
	scenes = s;
}



void drawTimer(int seconds) {
	std::string timerText = "time:" + std::to_string(seconds);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2d(10, screenHeight - 20);
	for (int i = 0; i < timerText.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, timerText[i]);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawScore(int score) {
	std::string scoreText = "score:" + std::to_string(score);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2d(10, screenHeight - 40);
	for (int i = 0; i < scoreText.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
	}
		

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawPaused() {
	std::string pausedText = "paused";
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2d(screenWidth / 2 - 20, screenHeight / 2);
	for (int i = 0; i < pausedText.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, pausedText[i]);
	}
		

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawTitle() {
	std::string titleText = "totally awesome title to my totally awesome game";
	std::string startText = "press space to start";
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2d(screenWidth / 2 - 200, screenHeight / 2);
	for (int i = 0; i < titleText.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, titleText[i]);
	}

	glRasterPos2d(screenWidth / 2 - 100, screenHeight / 2 -20);
	for (int i = 0; i < startText.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, startText[i]);
	}

		

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawGameOver() {
	std::string loseText = "you died :(";
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0f, 0, 0);
	glRasterPos2d(screenWidth / 2 - 40, screenHeight / 2);
	for (int i = 0; i < loseText.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, loseText[i]);
	}


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawWin() {
	std::string winText = "you win :D";
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0, 1, 0);
	glRasterPos2d(screenWidth / 2 - 40, screenHeight / 2);
	for (int i = 0; i < winText.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, winText[i]);
	}


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

//this is the tipity top level render everything on the screen function. everything forks from this.
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_TEXTURE_2D);


	//editor viewport drawn first 
	glViewport(0, 0, screenWidth, screenHeight); //full screen
	//draw editor pannels here
	e.draw(p);

	//game viewport overlayed on top
	glViewport(screenWidth * .12, screenHeight * .1, screenWidth * 0.75, screenHeight * .8); //cener ish of screen
	if (!title && !gameEnd && !win) {
		declareModels();
		drawTimer(gameTime);
		drawScore(score);
		if (paused) {
			drawPaused();
		}
	}
	else if (gameEnd) {
		drawGameOver();
	}
	else if (win) {
		drawWin();
	}
	else {
		drawTitle();
	}

	

	
	glutSwapBuffers();
}

void repositionCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(screenCenter.x - (cameraWidth / 2), screenCenter.x + (cameraWidth / 2), screenCenter.y - (cameraHeight / 2), screenCenter.y + (cameraHeight / 2));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}




//technically everything here is running at 29.7 fps 
void ThirtyFPSLogic(int v) {
	if (!paused && !win && !gameEnd) {
		if (willPlayerCollide(p, vx, 0, scenes.stages[scenes.currentStage].walls, scenes.stages[scenes.currentStage].coins, scenes.stages[scenes.currentStage].end) && willPlayerCollide(p, 0, vy, scenes.stages[scenes.currentStage].walls, scenes.stages[scenes.currentStage].coins, scenes.stages[scenes.currentStage].end)) {
			vy = 0;
		} else if (willPlayerCollide(p, 0, vy, scenes.stages[scenes.currentStage].walls, scenes.stages[scenes.currentStage].coins, scenes.stages[scenes.currentStage].end)) {
			vy = 0;
			p->move(vx, 0);
		} else if (willPlayerCollide(p, vx, 0, scenes.stages[scenes.currentStage].walls, scenes.stages[scenes.currentStage].coins, scenes.stages[scenes.currentStage].end)) {
			p->move(0, vy);
		}
		else {
			p->move(vx, vy);
		}

		vy -= 0.2;

		if (frameCount % 33 == 0) {
			gameTime--;
			if (gameTime < 0) {
				gameEnd = true;
				playSound("sounds\\\\metalpipe.wav", "loose", 0);
			}
		}
		frameCount++;
	}
	
	//ajust camera to player
	double camX = ox + p->x - screenWidth / 2;
	double camY = oy + p->y - screenHeight / 2;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(camX, camX + screenWidth, camY, camY + screenHeight);
	glMatrixMode(GL_MODELVIEW);

	//ajust axis location to center of screen
	//a.setPos(p->x, p->y);

	if (p->y < -10) {
		gameEnd = true;
		p->y = 0;
		playSound("sounds\\\\metalpipe.wav", "loose", 0);
	}
	
	glutTimerFunc(33, ThirtyFPSLogic, 0);
}

//most of this should be moved to the 30fps func eventually since idle runs significantly more often by comparison
void idle() {
	double currentTime = glutGet(GLUT_ELAPSED_TIME) / 100;
	double dt = currentTime - lastTime;
	lastTime = currentTime;
	p->update(dt);
	glutPostRedisplay();

}


void processSpecialKeys(int key, int x, int y) {
	if (textBoxFocusedId == -1) {
		switch (key) {
			case GLUT_KEY_UP:
				if (!up) {
					p->setAnimation(1);
					up = true;
				}
				break;
			case GLUT_KEY_DOWN:
				if (!down) {
					p->setAnimation(1);
					down = true;
				}
				break;
			case GLUT_KEY_RIGHT:
				if (!right) {
					p->setAnimation(1);
					vx += 2;
					right = true;
				}
				break;
			case GLUT_KEY_LEFT:
				if (!left) {
					p->setAnimation(1); //animation needs fliped horizontally but thats a problem for future me
					vx -= 2;
					left = true;
				}

				break;
			case GLUT_KEY_F1:

				break;
			case GLUT_KEY_F2:

				break;
			case GLUT_KEY_F3:

				break;
		}
	}
	
}

void processSpecialKeyReleases(int key, int x, int y) {
	//set flags + modify velocity vector then check if the player still has a wish direction
	switch (key) {
	case GLUT_KEY_UP:
		vy -= 2;
		up = false;
		break;
	case GLUT_KEY_DOWN:
		vy += 2;
		down = false;
		break;
	case GLUT_KEY_RIGHT:
		vx -= 2;
		right = false;
		break;
	case GLUT_KEY_LEFT:
		vx += 2;
		left = false;
		break;
	case GLUT_KEY_F1:

		break;
	case GLUT_KEY_F2:

		break;
	case GLUT_KEY_F3:

		break;

	}

	if (!up && !down && !left && !right) {
		p->setAnimation(0);
	}
}




void processKeys(unsigned char key, int x, int y) {
	if (textBoxFocusedId == -1) {
		switch (key) {
			case 'a': //togle axis
				drawAxis = !drawAxis;
				break;
			case 'c': //clear screen
				drawing = false;
				break;
			case 'm': //show screen
				drawing = true;
				break;
			case 'p': //is now the pause button acording to assignment
				paused = !paused;
				playMusic = !playMusic;
				if (playMusic) {
					chooseMusic(musicTrack);
				}
				else {
					stopSound("game");
					stopSound("breakCore");
					stopSound("dance");
				}
				break;
			case 32:

				if (!jumped) {
					vy = 5;
					jumped = true;
					playSound("sounds\\\\jump.wav", "jump", 0);
				}
				if (title) {
					title = false;
					paused = false;
					vy = 0;
				}
				break;

			case 27: //exit
				exit(1);
				break;
		}
	}
	else {
		switch (key) {
			case 27: //exit
				exit(1);
				break;
			default:
				char k = key;
				e.rightPannel.properties[textBoxFocusedId].text.push_back(k);
				break;
		}
	}
	

}

void processKeyReleases(unsigned char key, int x, int y) {
	switch (key) {
		//in case i need on release functionality sometime in the future
	}

}



bool xyInRegion(int x, int y, double regionX1, double regionY1, double regionX2, double regionY2) {
	if (x >= regionX1 && x <= regionX2 && y >= regionY1 && y <= regionY2) {
		return true;
	}
	else {
		return false;
	}
}

void processMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) { //check for editor stuff
			e.focusLost();
			textBoxFocusedId = -1;
			std::cout << x << " " << y << "\n";
			if (xyInRegion(x, y, 0, 0, 20, 25)) { //file button region
				e.fileClicked();
			}

			if (xyInRegion(x, y, 0, 26, 20, 50) && e.editorButtons[1].isVisible) { //quit button region
				exit(0);
			}

			if (xyInRegion(x, y, 21, 0, 60, 25)) { //help button region
				e.helpClicked();
			}

			//if click on game objects pannel
			int objID = 0;
			int leftPannleXOffset = -625;
			int leftPannelYOffset = 130;
			for (auto b : e.leftPannel.gameObjects) {
				if (xyInRegion(x, -y + 625, b.x - leftPannleXOffset, b.y + leftPannelYOffset, b.x + b.width - leftPannleXOffset, b.y + b.height + leftPannelYOffset)) {
					if (objID > e.editorBlocks.size() - 1) {
						e.rightPannel = propertiesPannel(e.editorCoins[objID - e.editorBlocks.size()], objID - e.editorBlocks.size());
						a.setPos(e.editorCoins[objID - e.editorBlocks.size()].x, e.editorCoins[objID - e.editorBlocks.size()].y);
					}
					else {
						e.rightPannel = propertiesPannel(e.editorBlocks[objID], objID);
						a.setPos(e.editorBlocks[objID].x, e.editorBlocks[objID].y);
					}
					
				}

				objID++;
			}


			//if click on properties pannel
			int rightPannleXOffset = -625;
			int rightPannelYOffset = 130;
			int textBoxCount = 0;
			for (auto p : e.rightPannel.properties) {
				if (xyInRegion(x, -y + 625, p.x - rightPannleXOffset, p.y + rightPannelYOffset, p.x + p.w - rightPannleXOffset, p.y + p.h + rightPannelYOffset)) {
					textBoxFocusedId = textBoxCount;
				}

				textBoxCount++;
			}

			e.updateVisibility();
		}
		else if (state == GLUT_UP) {
			//in case i need on release functionality sometime in the future
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			musicTrack++;
			if (musicTrack > 3) { musicTrack = 1; }
			chooseMusic(musicTrack);
		}
		else if (state == GLUT_UP) {
			//in case i need on release functionality sometime in the future
		}
	}
	else if (button == 3) { //scroll up
		cameraHeight *= .99;
		cameraWidth *= .99;
		repositionCamera();
	}
	else if (button == 4) { //scroll down
		cameraHeight *= 1.01;
		cameraWidth *= 1.01;
		repositionCamera();
	}
	else {

	}
}






#pragma endregion


int main(int argc, char** argv) {
	p = new player(100, 275, 100, 100, playerSprite);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); // RGB mode

	glutInitWindowSize(screenWidth, screenHeight); // window size
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Dakota Deets KSUID#811214188");
	glClearColor(0, 0, 0, 1.0);

	camX = -ox + p->x - screenWidth / 2;
	camY = -oy + p->y - screenHeight / 2;

	

	srand(time(NULL));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	std::vector<int> playerFrameList;  // would love to develop a system to clean this up later
	playerFrameList.push_back(4);
	playerFrameList.push_back(8);
	playerFrameList.push_back(8);
	playerFrameList.push_back(10);
	playerFrameList.push_back(9);
	playerFrameList.push_back(7);
	playerFrameList.push_back(6);
	playerFrameList.push_back(8);
	playerFrameList.push_back(13);
	playerFrameList.push_back(10);
	playerFrameList.push_back(12);
	playerFrameList.push_back(6);
	playerFrameList.push_back(8);
	playerFrameList.push_back(8);
	playerFrameList.push_back(8);
	playerFrameList.push_back(6);

	playerSprite = new spriteSheet("textures\\\\player.png", 16, 16, playerFrameList);
	p = new player(100, 275, 100, 100, playerSprite);

    camX = -ox + p->x - screenWidth / 2;
    camY = -oy + p->y - screenHeight / 2;

	buildLevels();


	e = editor(screenHeight, screenWidth, firstLevel);

	glutDisplayFunc(MyDisplay); // call the drawing function
	glutKeyboardFunc(processKeys);
	glutKeyboardUpFunc(processKeyReleases);
	glutSpecialFunc(processSpecialKeys);
	glutSpecialUpFunc(processSpecialKeyReleases);
	glutMouseFunc(processMouse);
	glutMotionFunc(mouseMotion);
	glutIdleFunc(idle);
	glutTimerFunc(33, ThirtyFPSLogic, 0);

	//chooseMusic(musicTrack);

	glutMainLoop();
	return 0;
}