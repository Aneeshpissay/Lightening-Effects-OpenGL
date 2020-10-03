#include <math.h>
#include <stdio.h>
#include <stdlib.h>	
#include <GL/glut.h>
#include <string.h>
#include <windows.h>

void myKeyboardFunc(unsigned char key, int x, int y);
void mySpecialKeyFunc(int key, int x, int y);

void drawScene(void);

void initRendering();
void resizeWindow(int w, int h);
static int window;

int mode = 1;
int then = 0;
float Noemit[4] = { 0.0, 0.0, 0.0, 1.0 };
float SphShininess = 10;
float SphAmbDiff[6][4] = {
	{0.5, 0.0, 0.0, 1.0},
	{0.5, 0.5, 0.0, 1.0},
	{0.0, 0.5, 0.0, 1.0},
	{0.0, 0.5, 0.5, 1.0},
	{0.0, 0.0, 0.5, 1.0},
	{0.5, 0.0, 0.5, 1.0}
};


unsigned int count = 0;
float angle = 0.0;
float transf = 2.5;
float SphSpecular[4] = { 1.0, 1.0, 1.0, 1.0 };
float Blanklight[4] = {0.0,0.0,0.0,0.0};
float ambientLight[4] = { 0.2, 0.2, 0.2, 1.0 };
float Lt0amb[4] = {1.0,1.0,1.0, 1.0 };
float Lt0diff[4] = { 1.0, 1.0, 1.0, 1.0 };
float Lt0spec[4] = { 1.0, 1.0, 1.0, 1.0 };
float halflight[4] = {0.5,0.5,0.5,1.0};
float zeroPos[4] = { 0.0, 0.0, 0.0, 1.0 };
float dirI[4] = { 1, 0, 0, 0 };

int LightIsPositional = 0;
int ballMode = 1;

int RunMode = 1;


float CurrentAngle = 0.0;
float AnimateStep = 0.5;
float ballangle = 0.0;
float ballstep = 0.5;


void names(int x, int y, const char* string, void* font)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);

	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

void square()
{
	glTranslatef(-1.0, -1.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 2.0);
	glVertex2f(2.0, 2.0);
	glVertex2f(2.0, 0.0);
	glEnd();
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r':
		RunMode = 1 - RunMode;
		if (RunMode == 1) {
			glutPostRedisplay();
		}
		break;
	case 's':
		RunMode = 1;
		drawScene();
		RunMode = 0;
		break;
	case 'l':
		LightIsPositional = 1 - LightIsPositional;
		if (RunMode == 0) {
			drawScene();
		}
		break;
	case 't':
		transf = -transf;
		break;	
	case 'q': count++;
		angle = 5.0;
		break;
	case 'a':count--;
		angle = 5.0; break;
	case 'z':
		exit(1);
	}
}

void mySpecialKeyFunc(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		if (AnimateStep < 5.0) {
			AnimateStep *= sqrt(2.0);
		}
		break;
	case GLUT_KEY_DOWN:
		if (AnimateStep > 1.0e-3) {
			AnimateStep /= sqrt(2.0);
		}
		break;
	}
}

void text()
{

	names(-12, 6, "Press q to add sphere", GLUT_BITMAP_HELVETICA_18);
	names(-12, 5, "Press a to delete sphere", GLUT_BITMAP_HELVETICA_18);
	names(-12, 4, "Press l toggle local light mode", GLUT_BITMAP_HELVETICA_18);
	names(-12, -4, "Press t to toggle and reverse the angles of spheres", GLUT_BITMAP_HELVETICA_18);
	names(-12, -5, "Press up arrow to increase speed", GLUT_BITMAP_HELVETICA_18);
	names(-12, -6, "Press down arrow to decrease speed", GLUT_BITMAP_HELVETICA_18);
	names(8, 6, "Press r to toggle", GLUT_BITMAP_HELVETICA_18);
	names(8, 5, "Press s to single step", GLUT_BITMAP_HELVETICA_18);
	names(8, 4, "Press RMB for menu options", GLUT_BITMAP_HELVETICA_18);
	names(8, 3, "Press z to exit", GLUT_BITMAP_HELVETICA_18);
}

void anticlocklayout(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (RunMode == 1) {

		CurrentAngle += AnimateStep;
		if (CurrentAngle > 360.0) {
			CurrentAngle -= 360.0 * floor(CurrentAngle / 360.0);
		}
	}

	if (ballMode == 1) {

		ballangle += ballstep;
		if (ballangle > 360.0) {
			ballangle -= 360.0 * floor(ballangle / 360.0);
		}
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glRotatef(ballangle, 0.0, 0.0, 1.0);
	glTranslatef(transf, 0.0, 0.0);
	glutSolidSphere(1.0, 20, 20);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glRotatef(CurrentAngle, 0.0, 0.0, 1.0);
	glTranslatef(7.0, 0.0, 0.0);
	glutSolidSphere(0.3, 10, 10);

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Lt0spec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Noemit);

	if (LightIsPositional == 1) {
		glLightfv(GL_LIGHT0, GL_POSITION, zeroPos);
	}
	else {
		glLightfv(GL_LIGHT0, GL_POSITION, dirI);
	}
	glPopMatrix();

	if (count > 6)
	{
		count = 0;
		ballMode = 1;
	}

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glutSolidTetrahedron();
	glPopMatrix();
	text();
}

void clocklayout(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (RunMode == 1) {

		CurrentAngle -= AnimateStep;
		if (CurrentAngle < 360.0) {
			CurrentAngle -= 360.0 * floor(CurrentAngle / 360.0);
		}
	}

	if (ballMode == 1) {

		ballangle -= ballstep;
		if (ballangle > 360.0) {
			ballangle -= 360.0 * floor(ballangle / 360.0);
		}
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glRotatef(ballangle, 0.0, 0.0, 1.0);
	glTranslatef(transf, 0.0, 0.0);
	glutSolidSphere(1.0, 20, 20);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glRotatef(CurrentAngle, 0.0, 0.0, 1.0);
	glTranslatef(7.0, 0.0, 0.0);
	glutSolidSphere(0.3, 20, 20);

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Lt0spec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Noemit);
	if (LightIsPositional == 1) {
		glLightfv(GL_LIGHT0, GL_POSITION, zeroPos);
	}
	else {
		glLightfv(GL_LIGHT0, GL_POSITION, dirI);
	}
	glPopMatrix();

	if (count > 6)
	{
		count = 0;
		ballMode = 1;
	}

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glutSolidTetrahedron();
	glPopMatrix();
	text();
}
void posanglespheres(void) {
	for (unsigned int i = 0; i < count; i++)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, SphAmbDiff[i]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SphSpecular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, SphShininess + (i * 20));
		glPushMatrix();
		glRotatef(angle * (float)i, 0.0, 0.0, 1.0);
		glTranslatef(transf, 0.0, 0.0);
		glutSolidSphere(1.0, 20, 20);
		glPopMatrix();
	}
}

void neganglespheres(void) {
	for (unsigned int i = 0; i < count; i++)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, SphAmbDiff[i]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SphSpecular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, SphShininess + (i * 20));
		glPushMatrix();
		glRotatef(-angle * (float)i, 0.0, 0.0, 1.0);
		glTranslatef(transf, 0.0, 0.0);
		glutSolidSphere(1.0, 20, 20);
		glPopMatrix();
	}
}

void anticlockspheres(void) {
	anticlocklayout();
	switch (count)
	{
	case 0:break;
	case 2:angle = 180.0; break;
	case 3:angle = 120.0; break;
	case 4:angle = 90.0; break;
	case 5:angle = 72.0; break;
	case 6:angle = 60.0; break;
	}
	posanglespheres();
}

void neganticlockspheres(void) {
	anticlocklayout();
	switch (count)
	{
	case 0:break;
	case 2:angle = 180.0; break;
	case 3:angle = 120.0; break;
	case 4:angle = 90.0; break;
	case 5:angle = 72.0; break;
	case 6:angle = 60.0; break;
	}
	neganglespheres();
}

void clockspheres(void) {
	clocklayout();
	switch (count)
	{
	case 0:break;
	case 2:angle = 180.0; break;
	case 3:angle = 120.0; break;
	case 4:angle = 90.0; break;
	case 5:angle = 72.0; break;
	case 6:angle = 60.0; break;
	}
	posanglespheres();
}

void negclockspheres(void) {
	clocklayout();
	switch (count)
	{
	case 0:break;
	case 2:angle = 180.0; break;
	case 3:angle = 120.0; break;
	case 4:angle = 90.0; break;
	case 5:angle = 72.0; break;
	case 6:angle = 60.0; break;
	}
	neganglespheres();
}

void ads(void) {
	glLightfv(GL_LIGHT0, GL_AMBIENT, Lt0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Lt0spec);
}

void amb(void) {
	glLightfv(GL_LIGHT0, GL_AMBIENT, Blanklight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Lt0spec);
}

void ambhalf(void) {
	glLightfv(GL_LIGHT0, GL_AMBIENT, halflight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Lt0spec);
}

void diff(void) {
	glLightfv(GL_LIGHT0, GL_AMBIENT, Lt0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Blanklight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Lt0spec);
}

void diffhalf(void) {
	glLightfv(GL_LIGHT0, GL_AMBIENT, Lt0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, halflight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Lt0spec);
}

void spec(void) {
	glLightfv(GL_LIGHT0, GL_AMBIENT, Lt0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Blanklight);
}

void spechalf(void) {
	glLightfv(GL_LIGHT0, GL_AMBIENT, Lt0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, halflight);
}

void ambdiff(void) {
	glLightfv(GL_LIGHT0, GL_AMBIENT, Blanklight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Blanklight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Lt0spec);
}

void diffspec(void) {
	glLightfv(GL_LIGHT0, GL_AMBIENT, Lt0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Blanklight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Blanklight);
}

void ambspec(void) {
	glLightfv(GL_LIGHT0, GL_AMBIENT, Blanklight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Blanklight);
}

void adsnull(void) {
	glLightfv(GL_LIGHT0, GL_AMBIENT, Blanklight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Blanklight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Blanklight);
}

void drawScene(void)
{
	if (mode == 1) {
		clockspheres();
		ads();
	}
	else if (mode == 2) {
		anticlockspheres();
		ads();
	}
	else if (mode == 3) {
		negclockspheres();
	}
	else if (mode == 4) {
		neganticlockspheres();
	}
	else if (mode == 5) {
		clockspheres();
		amb();
	}
	else if (mode == 6) {
		anticlockspheres();
		amb();
	}
	else if (mode == 7) {
		clockspheres();
		ambhalf();
	}
	else if (mode == 8) {
		anticlockspheres();
		ambhalf();
	}
	else if (mode == 9) {
		clockspheres();
		diff();
	}
	else if (mode == 10) {
		anticlockspheres();
		diff();
	}
	else if (mode == 11) {
		clockspheres();
		diffhalf();
	}
	else if (mode == 12) {
		anticlockspheres();
		diffhalf();
	}
	else if (mode == 13) {
		clockspheres();
		spec();
	}
	else if (mode == 14) {
		anticlockspheres();
		spec();
	}
	else if (mode == 15) {
		clockspheres();
		spechalf();
	}
	else if (mode == 16) {
		anticlockspheres();
		spechalf();
	}
	else if (mode == 17) {
		clockspheres();
		ambdiff();
	}
	else if (mode == 18) {
		anticlockspheres();
		ambdiff();
	}
	else if (mode == 19) {
		clockspheres();
		diffspec();
	}
	else if (mode == 20) {
		anticlockspheres();
		diffspec();
	}
	else if (mode == 21) {
		clockspheres();
		ambspec();
	}
	else if (mode == 22) {
		anticlockspheres();
		ambspec();
	}
	else if (mode == 23) {
		clockspheres();
		adsnull();
	}
	else if (mode == 24) {
		anticlockspheres();
		adsnull();
	}
	glLineWidth(4.5);
	square();
	glFlush();
	glutSwapBuffers();

	if (RunMode == 1) {
		glutPostRedisplay();
	}
	if (count == 6) {
		glutPostRedisplay();
	}

}

// Initialize OpenGL's rendering modes
void initRendering()
{
	glEnable(GL_DEPTH_TEST);	// Depth testing must be turned on

	glEnable(GL_LIGHTING);		// Enable lighting calculations
	glEnable(GL_LIGHT0);		// Turn on light #0.
	// Set global ambient light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	// Light 0 light values.  Its position is set in drawScene().
}

// Called when the window is resized
//		w, h - width and height of the window in pixels.
void resizeWindow(int w, int h)
{
	float viewWidth = 7.0;			// Actually this is half of the width
	float viewHeight = 7.0;			// Again, this is half of the height
	glViewport(0, 0, w, h);
	h = (h == 0) ? 1 : h;
	w = (w == 0) ? 1 : w;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h < w) {
		viewWidth *= (float)w / (float)h;
	}
	else {
		viewHeight *= (float)h / (float)w;
	}

	glOrtho(-viewWidth, viewWidth, -viewHeight, viewHeight, -1.0, 1.0);


}
void menu(int num) {
	if (num == 0) {
		glutDestroyWindow(window);
		exit(0);
	}
	else {
		mode = num;
	}
	glutPostRedisplay();
}

void createMenu(void) {
	int clockwith = glutCreateMenu(menu);
	glutAddMenuEntry("Ambient, Diffuse and Specular light", 1);
	glutAddMenuEntry("Negative angles", 3);
	glutAddMenuEntry("Half Ambient light", 7);
	glutAddMenuEntry("Half Diffuse light", 11);
	glutAddMenuEntry("Half Specular light", 15);
	int clockwithout = glutCreateMenu(menu);
	glutAddMenuEntry("Ambient light", 5);
	glutAddMenuEntry("Diffuse light", 9);
	glutAddMenuEntry("Specular light", 13);
	glutAddMenuEntry("Ambient and Diffuse light", 17);
	glutAddMenuEntry("Diffuse and Specular light", 19);
	glutAddMenuEntry("Ambient and Specular light", 21);
	glutAddMenuEntry("Ambient, Diffuse and Specular light", 23);
	int anticlockwith = glutCreateMenu(menu);
	glutAddMenuEntry("Ambient, Diffuse and Specular light", 2);
	glutAddMenuEntry("Negative angles", 4);
	glutAddMenuEntry("Half Ambient light", 8);
	glutAddMenuEntry("Half Diffuse light", 12);
	glutAddMenuEntry("Half Specular light", 16);
	int anticlockwithout = glutCreateMenu(menu);
	glutAddMenuEntry("Ambient light", 6);
	glutAddMenuEntry("Diffuse light", 10);
	glutAddMenuEntry("Specular light", 14);
	glutAddMenuEntry("Ambient and Diffuse light", 18);
	glutAddMenuEntry("Diffuse and Specular light", 20);
	glutAddMenuEntry("Ambient and Specular light", 22);
	glutAddMenuEntry("Ambient, Diffuse and Specular light", 24);
	int menuclock = glutCreateMenu(menu);
	glutAddSubMenu("With", clockwith);
	glutAddSubMenu("Without", clockwithout);
	int menuanticlock = glutCreateMenu(menu);
	glutAddSubMenu("With", anticlockwith);
	glutAddSubMenu("Without", anticlockwithout);
	glutCreateMenu(menu);
	glutAddSubMenu("Rotate spheres in clockwise direction", menuclock);
	glutAddSubMenu("Rotate spheres in anti-clockwise direction", menuanticlock);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv)
{
	printf("Arrow keys for control speed\n");
	printf("press \"r\" to run\n");
	printf("press \"q\" to increase number of balls\n");
	printf("press \"a\" for decresing number of balls\n");
	printf("Press \"t\" to toggle and reverse the angles of spheres");
	printf("Press \"l\" toggle local light mode\n");
	printf("press rmb for menu options\n");
	printf("press z to exit\n");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1366, 768);
	glutCreateWindow("Lighting Effects");
	createMenu();
	initRendering();
	glLineWidth(1.0);
	glutKeyboardFunc(myKeyboardFunc);
	glutSpecialFunc(mySpecialKeyFunc);
	glutReshapeFunc(resizeWindow);
	glutDisplayFunc(drawScene);
	glutMainLoop();
	return(0);
}
