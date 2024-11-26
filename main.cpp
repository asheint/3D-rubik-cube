#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <SOIL2.h>

constexpr float PI = 3.14159265358979323846;

int width;
int height;

GLfloat camXInit = 0.0; GLfloat camYInit = 4.0; GLfloat camZInit = 0.0;
GLfloat sceRXInit = 0.0; GLfloat sceRYInit = -30.0; GLfloat sceRZInit = 0.0;
GLfloat sceTXInit = 0.0; GLfloat sceTYInit = 0.0; GLfloat sceTZInit = -2;

//variables to move the camera
GLfloat camX = camXInit; GLfloat camY = camYInit; GLfloat camZ = camZInit;

//variables to move the look at position
GLfloat lookX = 0.0; GLfloat lookY = 0.0; GLfloat lookZ = 0.0;

//variables to move the scene
GLfloat sceRX = sceRXInit; GLfloat sceRY = sceRYInit; GLfloat sceRZ = sceRZInit;
GLfloat sceTX = sceTXInit; GLfloat sceTY = sceTYInit; GLfloat sceTZ = sceTZInit;

////variables to move the objects
GLfloat objRX = 0.0; GLfloat objRY = 0.0; GLfloat objRZ = 0.0;
GLfloat objTX = 0.0; GLfloat objTY = 0.0; GLfloat objTZ = 0.0;


//To on/off grids and axes
int gridOn = 0;
int axesOn = 0;

void drawGrid() {
	GLfloat step = 1.0f;
	GLint line;

	glBegin(GL_LINES);
	for (line = -20; line <= 20; line += step) {
		glVertex3f(line, -0.4, 20);
		glVertex3f(line, -0.4, -20);

		glVertex3f(20, -0.4, line);
		glVertex3f(-20, -0.4, line);
	}
	glEnd();
}

void drawAxes() {

	glBegin(GL_LINES);

	glLineWidth(1.5);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-200, 0, 0);
	glVertex3f(200, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, -200, 0);
	glVertex3f(0, 200, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, -200);
	glVertex3f(0, 0, 200);

	glEnd();
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
}


float start = 2.25;
int topLayerAngle = 0;
int middleLayerAngle = 0;
int bottomLayerAngle = 0;

void singleCube() {
	float cubesize = 1.0;
	glBegin(GL_QUADS);

	// Front Face (Red)
	glColor3f(1.0f, 0.0f, 0.0f); 
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	// Back Face (Orange)
	glColor3f(1.0f, 0.647f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Top Face (Yellow)
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Bottom Face (White)
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right Face (Green)
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Left Face (Blue)
	glColor3f(0.0f, 0.0f, 1.0f); 
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glEnd();
}

void singleLayer() {
	for (float i = -start; i <= start; i += start) {
		for (float k = -start; k <= start; k += start) {
			glPushMatrix();
			glTranslatef(i, 0, k);
			singleCube();
			glPopMatrix();
		}
	}
}

void drawRubiksCube() {
	GLfloat angles[] = { topLayerAngle, middleLayerAngle, bottomLayerAngle };
	GLfloat translations[] = { start, 0, -start };

	for (int i = 0; i < 3; ++i) {
		glPushMatrix();
		glRotatef(angles[i], 0, 1, 0);
		glTranslatef(0, start - i * start, 0);
		singleLayer();
		glPopMatrix();
	}
}

void setLighting() {

	// Lighting set up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);

	// Set lighting intensity and color - light 0
	GLfloat qaAmbientLight[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat qaDiffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	// Set the light position - light 0
	GLfloat qaLightPosition[] = { -10.0, 1.0, -.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

	// Set lighting intensity and color - light 1
	GLfloat qaAmbientLight1[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat qaDiffuseLight1[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat qaSpecularLight1[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, qaAmbientLight1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, qaDiffuseLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, qaSpecularLight1);

	// Set the light position - light 1
	GLfloat qaLightPosition1[] = { 10.0, 1.0, 0.5, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, qaLightPosition1);

	GLfloat SpecRef[] = { 0.7,0.7,0.7,1.0 };
	GLint Shine = 128;
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);                   //Enable color tracking
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//Set material to follow
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecRef);//Set specular reflectivity and shine
	glMateriali(GL_FRONT, GL_SHININESS, Shine);
}



void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	// camera orientation (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
	gluLookAt(0.0, 0.0 + camY, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// move the scene (all the rendered environment) using keyboard keys
	glTranslatef(sceTX, sceTY, sceTZ);
	glRotatef(sceRY, 0.0, 1.0, 0.0);

	setLighting();

	//Draw a grid on the xz plane
	glColor3f(0.1, 0.1, 0.1);
	if (gridOn == 1)
		drawGrid();
	//Draw the three axes
	if (axesOn == 1)
		drawAxes();

	drawRubiksCube();

	glPopMatrix();
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Define the Perspective projection frustum
	// (FOV_in_vertical, aspect_ratio, z-distance to the near plane from the camera position, z-distance to far plane from the camera position)
	gluPerspective(120.0, aspect_ratio, 0.001, 5000.0);

}

void keyboardSpecial(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		camY += 0.2;

	if (key == GLUT_KEY_DOWN)
		camY -= 0.2;

	if (key == GLUT_KEY_LEFT)
		sceRY += 2;

	if (key == GLUT_KEY_RIGHT)
		sceRY -= 2;

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'l')
		objRY += 1;

	if (key == 'r')
		objRY -= 1;

	if (key == 'Z')
		sceTZ += 1;

	if (key == 'z')
		sceTZ -= 1;

	if (key == 'a') {
		sceTX += 0.5;
		objTX -= 0.5;
	}
	if (key == 'A') {
		sceTX += 2;
		objTX -= 2;
	}

	if (key == 'd') {
		sceTX -= 0.5;
		objTX += 0.5;
	}
	if (key == 'D') {
		sceTX -= 2;
		objTX += 2;
	}

	if (key == 'w') {
		sceTZ += 0.5;
		objTZ -= 0.5;
	}
	if (key == 'W') {
		sceTZ += 2;
		objTZ -= 2;
	}

	if (key == 's') {
		sceTZ -= 0.5;
		objTZ += 0.5;
	}
	if (key == 'S') {
		sceTZ -= 2;
		objTZ += 2;
	}

	if (key == 'q') {
		sceTY += 0.5;
		objTY -= 0.5;
	}
	if (key == 'Q') {
		sceTY += 2;
		objTY -= 2;
	}

	if (key == 'e') {
		sceTY -= 0.5;
		objTY += 0.5;
	}
	if (key == 'E') {
		sceTY -= 2;
		objTY += 2;
	}

	if (key == '6')
		start += 1;

	if (key == '4')
		if (start > 2) {
			start -= 1;
		}
		else {
			start = 2;
		}


	int angleChange = 90;
	if (key == 't')
		topLayerAngle += angleChange;
	if (key == 'T')
		topLayerAngle -= angleChange;

	if (key == 'y')
		middleLayerAngle += angleChange;
	if (key == 'Y')
		middleLayerAngle -= angleChange;

	if (key == 'u')
		bottomLayerAngle += angleChange;
	if (key == 'U')
		bottomLayerAngle -= angleChange;

	if (key == '+')
		start += 0.05;
	if (key == '-') {
		if (start > 2) {
			start -= 0.05;
		}
	}

	if (key == 'r') {
		topLayerAngle = 0;
		bottomLayerAngle = 0;
		middleLayerAngle = 0;
	}
	if (key == 'R') {
		start = 2.25;
	}

	if (key == '.') {
		lookX = 0;
		lookY = 0;
		lookZ = 0;

		camX = camXInit;
		camY = camYInit;
		camZ = camZInit;

		sceRX = sceRXInit;
		sceRY = sceRYInit;
		sceRZ = sceRZInit;

		sceTX = sceTXInit;
		sceTY = sceTYInit;
		sceTZ = sceTZInit;

		objRX = 0;
		objRY = 0;
		objRZ = 0;

		objTX = 0;
		objTY = 0;
		objTZ = 0;
	}

	if (key == '6')
		glShadeModel(GL_FLAT);

	if (key == '7')
		glShadeModel(GL_SMOOTH);

	if (key == '!')
		glDisable(GL_LIGHT0); // Light at -x

	if (key == '1')
		glEnable(GL_LIGHT0);

	if (key == '@')
		glDisable(GL_LIGHT1); // Light at +x

	if (key == '2')
		glEnable(GL_LIGHT1);

	//Grids and axes
	if (key == '0')
		gridOn = 1;
	if (key == ')')
		gridOn = 0;
	if (key == '9')
		axesOn = 1;
	if (key == '(')
		axesOn = 0;


	glutPostRedisplay();
}

int main(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Rubik Cube");
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	init();
	glutMainLoop();
	return 0;
}