#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <math.h>
#include <iostream>
#define PI 3.14159265
int WIDTH = 1280;
int HEIGHT = 720;

char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;


class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(0, 2, 3);
Vector At(0, 1, 0);
Vector Up(0, 1, 0);
class movingCube
{
public:
	movingCube() {

		model_car.Load("Models/MURCIELAGO640.3ds");
		currentX = 0;
		currentZ = 0;
		currentSpeed = 0;
		currentAcceleration = 0.005;
		maxSpeed = 200;
		currentAngle = 0;
	}

	void accelerate() {
		//	currentX += currentSpeed;
		if (currentSpeed < maxSpeed)
			currentSpeed += currentAcceleration;
		//std::cout << currentSpeed << std::endl;
	}

	void decelerate() {
		//	currentX += currentSpeed;
		currentSpeed -= currentAcceleration;
	}

	void draw() {
		glPushMatrix();
		glTranslatef(currentZ, 0, -currentX);
		std::cout << currentZ << " " << currentX << std::endl;
		if (currentAngle < 90) {
			Eye.z = -currentX + 10;
			Eye.x = currentZ;
			At.z = -currentX;
			At.x = currentZ;
		}

		if (currentAngle > 100) {
			Eye.z = -currentX - 10;
			Eye.x = currentZ;
			At.z = -currentX;
			At.x = currentZ;
		}
		//glColor3f(0.0f, 0.0f, 0.0f);
		//glutSolidCube(2);
		glRotatef(-90.f - currentAngle, 0, 1, 0);
		model_car.Draw();
		glPopMatrix();
		if (moveUp)
			accelerate();
		if (moveDown)
			decelerate();
		if (turnLeft)
			left();
		if (turnRight)
			right();
		if (!moveDown && !moveUp)
			naturalDeceleration();

	}

	void left() {
		currentAngle -= 0.5;
		if (currentAngle == -360) {
			currentAngle = 0;
		}
	}

	void right() {
		currentAngle += 0.5;
		if (currentAngle == 360) {
			currentAngle = 0;
		}

	}

	void naturalDeceleration() {
		if (currentSpeed > 0)
			currentSpeed -= ((0.25) * currentAcceleration);
	}

	void move() {
		currentX += (cos(currentAngle * PI / 180) * currentSpeed);
		currentZ += (sin(currentAngle * PI / 180) * currentSpeed);
		if (std::round(currentX) > 210)
			currentX = 210;
		if (std::round(currentZ) <= -5)
			currentZ = -5;
		if (std::round(currentZ) >= 18)
			currentZ = 18;
	}
public:
	float currentX, currentZ, currentAcceleration, currentSpeed, maxSpeed, currentAngle;  Model_3DS model_car;
	bool moveUp, moveDown, turnLeft, turnRight;

};

movingCube m;


int cameraZoom = 0;

// Model Variables
//Model_3DS model_car;
Model_3DS model_tree;
Model_3DS model_wall;

// Textures
GLTexture tex_ground;
GLTexture tex_asph;
GLTexture tex_turn;
GLTexture tex_front;
GLTexture tex_back;
GLTexture tex_up;
GLTexture tex_down;
GLTexture tex_left;
GLTexture tex_right;
//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture
	glPushMatrix();
	glTranslated(Eye.x, 0, Eye.z);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(2, 2);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-100, 0, -100);
	glTexCoord2f(7, 2);
	glVertex3f(100, 0, -100);
	glTexCoord2f(7, 7);
	glVertex3f(100, 0, 100);
	glTexCoord2f(2, 7);
	glVertex3f(-100, 0, 100);
	glEnd();
	glPopMatrix();

	//glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture
	glBindTexture(GL_TEXTURE_2D, tex_asph.texture[0]);
	// Start Lane
	for (int i = 0, r = 0; i < 200; i += 10, r += 5) {
		glPushMatrix();
		//glTranslatef(-i/7, 0, -i/1.09);
		glTranslated(0, 0.01, -i);
		glRotated(180, 0, 1, 0);
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);	// Set quad normal direction.
		glTexCoord2f(2, 2);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
		glVertex3f(6, 0, -5);
		glTexCoord2f(3, 2);
		glVertex3f(-6, 0, -5);
		glTexCoord2f(3, 3);
		glVertex3f(-6, 0, 5);
		glTexCoord2f(2, 3);
		glVertex3f(6, 0, 5);
		glEnd();
		glPopMatrix();
	}

	// Returun Lane
	for (int i = 0, r = 0; i < 200; i += 10, r += 5) {
		glPushMatrix();
		//glTranslatef(-i/7, 0, -i/1.09);
		glTranslated(12.1, 0.01, -i);
		//glRotated(180, 0, 1, 0);
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);	// Set quad normal direction.
		glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
		glVertex3f(6, 0, -5);
		glTexCoord2f(1, 0);
		glVertex3f(-6, 0, -5);
		glTexCoord2f(1, 1);
		glVertex3f(-6, 0, 5);
		glTexCoord2f(0, 1);
		glVertex3f(6, 0, 5);
		glEnd();
		glPopMatrix();
	}

	////U-Turn
	glBindTexture(GL_TEXTURE_2D, tex_turn.texture[0]);
	glPushMatrix();
	glTranslatef(3.6, 0.01, 0);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(15, 0, -210);
	glTexCoord2f(1, 0);
	glVertex3f(-10, 0, -210);
	glTexCoord2f(1, 1);
	glVertex3f(-10, 0, -195);
	glTexCoord2f(0, 1);
	glVertex3f(15, 0, -195);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}


void Draw_Skybox(float x, float y, float z, float width, float height, float length)
{
	// Center the Skybox around the given x,y,z position
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;


	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, tex_front.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, tex_back.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, tex_left.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, tex_right.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, tex_up.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, tex_down.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glEnd();

}
//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw Ground
	RenderGround();
	Draw_Skybox(Eye.x, Eye.y, Eye.z, 135, 135, 135);
	// Draw House Model
	for (int i = 0; i < 200; i += 10) {
		glPushMatrix();
		glTranslatef(6, 0, -i);
		glScalef(0.7, 0.7, 0.7);
		model_tree.Draw();
		glPopMatrix();
	}

	m.draw();
	m.move();
	//Eye.x = m.currentZ;
	//Eye.z = -m.currentX;
	//At.x = m.currentZ;
	//At.z = -m.currentX;
	// Draw Tree Model
	/*glPushMatrix();
	glRotatef(-90.f, 0, 1, 0);
	model_car.Draw();
	glPopMatrix();*/


	glutSwapBuffers();
}

//=======================================================================
// Keyboard Function
//=======================================================================
void Keyboard(unsigned char key, int x, int y) {
	if (key == 'w')
		m.moveUp = true;
	if (key == 's')
		m.moveDown = true;
	if (key == 'd')
		m.turnRight = true;
	if (key == 'a')
		m.turnLeft = true;
}

void KeyboardUp(unsigned char key, int x, int y) {
	if (key == 'w')
		m.moveUp = false;
	if (key == 's')
		m.moveDown = false;
	if (key == 'd')
		m.turnRight = false;
	if (key == 'a')
		m.turnLeft = false;
}

//=======================================================================
// Motion Function
//=======================================================================
/*void myMotion(int x, int y)
{
y = HEIGHT - y;

if (cameraZoom - y > 0)
{
//Eye.x += -1;
Eye.z += -1;
//At.x += -1;
At.z += -1;
}
else
{
//Eye.x += 1;
Eye.z += 1;
At.z += 1;
//At.x += 1;
}

//if (cameraZoom - x > 0)
//{
//	Eye.x += -1;
//	//Eye.z += -1;
//	At.x += -1;
//	//At.z += -1;
//}
//else
//{
//	Eye.x += 1;
//	//Eye.z += 1;
//	//At.z += 1;
//	At.x += 1;
//}


cameraZoom = y;

glLoadIdentity();	//Clear Model_View Matrix

gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
glLightfv(GL_LIGHT0, GL_POSITION, light_position);

glutPostRedisplay();	//Re-draw scene
}
*/
//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

void Anim() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glRotatef(90.f - m.currentAngle, 0, 1, 0);

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	glutPostRedisplay();
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}



//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	//model_car.Load("Models/MURCIELAGO640.3ds");
	model_tree.Load("Models/tree/tree1.3ds");
	model_wall.Load("Models/wall/wall.3ds");
	// Loading texture files
	tex_ground.Load("Textures/grass 2.bmp");
	tex_turn.Load("Textures/uturn.bmp");
	tex_asph.Load("Textures/asph - Copy.bmp");
	tex_front.Load("Textures/front.bmp");
	tex_back.Load("Textures/back.bmp");
	tex_up.Load("Textures/up.bmp");
	tex_down.Load("Textures/down.bmp");
	tex_left.Load("Textures/left.bmp");
	tex_right.Load("Textures/right.bmp");
}

//=======================================================================
// Main Function
//=======================================================================




void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	//glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutIdleFunc(Anim);

	glutReshapeFunc(myReshape);


	myInit();

	LoadAssets();

	glutMainLoop();
}