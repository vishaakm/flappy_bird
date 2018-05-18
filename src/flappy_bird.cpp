#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>

//Pipe
int pipeSpawn[3] = { 300,500,700 };
int translatePipePos[3] = { 0,100,0 };
int pipeCollisionBox[3][4][2] = { { { 0,0 },{ 0,0 },{ 0,0 },{ 0,0 } },{ { 0,0 },{ 0,0 },{ 0,0 },{ 0,0 } },{ { 0,0 },{ 0,0 },{ 0,0 },{ 0,0 } } };

//Bird
int wingRotateAngle = 0;
int birdPos = 0;
int birdFlyDist = 0;
int birdCollisionBox[2][2] = { { -180,10 },{ -180,-10 } };

//Game logic
int introStatus = 0;
int gameOver = 0;
int btnState = 0;
int score = 0;
int reset = 0;

void resetGame() {
	if (reset == 1) {
		gameOver = 0;
		reset = 0;
		score = 0;
		birdPos = 0;
		birdFlyDist = 0;

		//Reseting Pipe Position
		pipeSpawn[0] = 300;
		pipeSpawn[1] = 500;
		pipeSpawn[2] = 700;
		translatePipePos[0] = 0;
		translatePipePos[1] = 100;
		translatePipePos[2] = 0;

		glutPostRedisplay();
	}
}

void drawPipe(int index, int pipeXAxis, int pipeYAxis)
{	
	//Collision Box
	pipeCollisionBox[index][0][0] = pipeXAxis;
	pipeCollisionBox[index][0][1] = pipeYAxis + 50;
	pipeCollisionBox[index][1][0] = pipeXAxis + 50;
	pipeCollisionBox[index][1][1] = pipeYAxis + 50;
	pipeCollisionBox[index][2][0] = pipeXAxis + 50;
	pipeCollisionBox[index][2][1] = pipeYAxis - 50;
	pipeCollisionBox[index][3][0] = pipeXAxis;
	pipeCollisionBox[index][3][1] = pipeYAxis - 50;

	/*glPushMatrix();
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex2i(pipeCollisionBox[index][0][0], pipeCollisionBox[index][0][1]);
	glVertex2i(pipeCollisionBox[index][1][0], pipeCollisionBox[index][1][1]);
	glVertex2i(pipeCollisionBox[index][2][0], pipeCollisionBox[index][2][1]);
	glVertex2i(pipeCollisionBox[index][3][0], pipeCollisionBox[index][3][1]);
	glEnd();*/

	//Top pipe
	glPushMatrix();
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex2i(pipeXAxis, 800);
	glVertex2i(pipeXAxis + 50, 800);
	glVertex2i(pipeXAxis + 50, 50 + pipeYAxis);
	glVertex2i(pipeXAxis, 50 + pipeYAxis);
	//Pipe head
	glColor3f(0.137255, 0.556863, 0.137255);
	glVertex2i(pipeXAxis - 3, 70 + pipeYAxis);
	glVertex2i(pipeXAxis + 50 + 3, 70 + pipeYAxis);
	glVertex2i(pipeXAxis + 50 + 3, 50 + pipeYAxis);
	glVertex2i(pipeXAxis - 3, 50 + pipeYAxis);
	glEnd();

	//Bottom pipe
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex2i(pipeXAxis, -800);
	glVertex2i(pipeXAxis + 50, -800);
	glVertex2i(pipeXAxis + 50, -50 + pipeYAxis);
	glVertex2i(pipeXAxis, -50 + pipeYAxis);
	//Pipe Head
	glColor3f(0.137255, 0.556863, 0.137255);
	glVertex2i(pipeXAxis - 3, -70 + pipeYAxis);
	glVertex2i(pipeXAxis + 50 + 3, -70 + pipeYAxis);
	glVertex2i(pipeXAxis + 50 + 3, -50 + pipeYAxis);
	glVertex2i(pipeXAxis - 3, -50 + pipeYAxis);
	glEnd();
	glPopMatrix();
}

void pipeSpawner(int unused)
{
	if (pipeSpawn[0] == -300 && gameOver != 1) {
		pipeSpawn[0] = 300;
		translatePipePos[0] = (rand() % 20) * 10;
		score++;
	}
	else
		pipeSpawn[0] -= 1;

	if (pipeSpawn[1] == -300 && gameOver != 1) {
		pipeSpawn[1] = 300;
		translatePipePos[1] = (rand() % 20) * 10;
		score++;
	}
	else
		pipeSpawn[1] -= 1;

	if (pipeSpawn[2] == -300 && gameOver != 1) {
		pipeSpawn[2] = 300;
		translatePipePos[2] = (rand() % 20) * 10;
		score++;
	}
	else pipeSpawn[2] -= 1;

	glutPostRedisplay();
	glutTimerFunc(10, pipeSpawner, 0);
}

void drawBird()
{
	glPushMatrix();
	glTranslatef(0, birdPos, 0);
	//Bird Y Axis
	birdCollisionBox[0][1] = 10 + birdPos;
	birdCollisionBox[1][1] = -10 + birdPos;
	
	glColor3f(1, 1, 0);
	glBegin(GL_QUADS);
	//Body
	glVertex2i(-200, 10);
	glVertex2i(-200 + 20, 10);
	glVertex2i(-200 + 20, -10);
	glVertex2i(-200, -10);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLES);
	//Beak
	glVertex2i(-200 + 20, 3);
	glVertex2i(-200 + 25, 0);
	glVertex2i(-200 + 20, -3);
	glEnd();

	glRotatef(wingRotateAngle, 1, 0, 0);
	glBegin(GL_TRIANGLES);
	//Wing
	glVertex2i(-200 + 10, 5);
	glVertex2i(-200 + 5, -5);
	glVertex2i(-200 + 15, -5);
	glEnd();
	glPopMatrix();
}

int birdCollision() {
	int i;
	for (i = 0; i < 3; i++) {
		if (pipeCollisionBox[i][0][0] <= -180 && pipeCollisionBox[i][1][0] >= -200) {
			if ((birdCollisionBox[0][1] >= pipeCollisionBox[i][0][1] && birdCollisionBox[0][0] == -180) || (birdCollisionBox[1][1] <= pipeCollisionBox[i][2][1] && birdCollisionBox[1][0] == -180)) {
				return 1;
			}
			else if ((birdCollisionBox[0][1] >= pipeCollisionBox[i][0][1] && birdCollisionBox[0][0] == -200) || (birdCollisionBox[1][1] <= pipeCollisionBox[i][2][1] && birdCollisionBox[1][0] == -200)) {
				return 1;
			}
		}
	}
	
	return 0;
}

void birdFly(int unused)
{
	if (birdCollision() == 1) {
		gameOver = 1;
		reset = 1;
	}
	if (birdFlyDist != 0 && birdPos != 300) {
		birdPos++;
		birdFlyDist--;
	}
	else {
		btnState = 0;
		return;
	}
	glutPostRedisplay();
	glutTimerFunc(10, birdFly, unused);
}

void birdFall()
{
	if (birdCollision() == 1) {
		gameOver = 1;
		reset = 1;
	}

	if (btnState == 1)
		return;

	if (birdPos != -300 && btnState == 0) {
		birdPos--;
	}

	if (birdPos == -300 || birdPos == 300) {
		gameOver = 1;
		reset = 1;
	}
	glutPostRedisplay();
}

void birdPhysics(int unused)
{
	if (wingRotateAngle == 180)
		wingRotateAngle = 0;
	else wingRotateAngle = 180;

	birdFall();

	glutTimerFunc(10, birdPhysics, 0);
}

void birdMovementMouse(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		birdFlyDist = 30;
		btnState = 1;
		birdFly(0);
	}
}

void birdMovementKey(unsigned char btn, int x, int y) {
	if (btn == ' ') {
		birdFlyDist = 30;
		btnState = 1;
		birdFly(0);
	}
	
	if (btn == ' ' && introStatus == 0) {
		introStatus = 1;
		reset = 1;
		resetGame();
	}

	if (btn == ' ' && gameOver == 1) {
		resetGame();
	}
}

void renderBitmapString(float x, float y, void *font, const char *string) {
	const char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void drawScore() {
	char buffer[15] = { '\0' };
	sprintf_s(buffer, "Score : %d", score);
	glColor3f(0, 0, 0);
	renderBitmapString(-290, 270, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
}

void introScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	glColor3f(0, 0, 1);
	renderBitmapString(-150, 265, GLUT_BITMAP_TIMES_ROMAN_24, "CANARA ENGINEERING COLLEGE");
	glColor3f(0.7, 0, 1);
	renderBitmapString(-160, 230, GLUT_BITMAP_TIMES_ROMAN_24, "DEPARTMENT OF COMPUTER SCIENCE");
	renderBitmapString(-20, 205, GLUT_BITMAP_TIMES_ROMAN_24, "AND");
	renderBitmapString(-60, 180, GLUT_BITMAP_TIMES_ROMAN_24, "ENGINEERING");
	glColor3f(1, 0.5, 0);
	renderBitmapString(-90, 120, GLUT_BITMAP_TIMES_ROMAN_24, "A MINI PROJECT ON");
	glColor3f(1, 0, 0);
	renderBitmapString(-120, 90, GLUT_BITMAP_TIMES_ROMAN_24, "FLAPPY BIRD ~ THE GAME");
	glColor3f(1, 0.5, 0);
	renderBitmapString(-290, -30, GLUT_BITMAP_TIMES_ROMAN_24, "BY:");
	glColor3f(0.5, 0, 0.5);
	renderBitmapString(-260, -60, GLUT_BITMAP_TIMES_ROMAN_24, "VISHAAK M           (4CB15CS121)");
	renderBitmapString(-260, -90, GLUT_BITMAP_TIMES_ROMAN_24, "SANJAN ALVA      (4CB15CS102)");
	glColor3f(1, 0.5, 0);
	renderBitmapString(40, -30, GLUT_BITMAP_TIMES_ROMAN_24, "GUIDES:");
	glColor3f(0.5, 0.2, 0.2);
	renderBitmapString(120, -60, GLUT_BITMAP_TIMES_ROMAN_24, "SATHISH SHENOY");
	renderBitmapString(120, -90, GLUT_BITMAP_TIMES_ROMAN_24, "SANTHOSH D K");
	glColor3f(1, 0.1, 1);
	renderBitmapString(-80, -275, GLUT_BITMAP_TIMES_ROMAN_24, "Press Space to Start");

	glFlush();
}

void gameOverScreen() {
	char buffer[15] = { '\0' };

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	glColor3f(1, 0, 0);
	renderBitmapString(-40, 90, GLUT_BITMAP_TIMES_ROMAN_24, "Game Over");

	glColor3f(1, 0.5, 0);
	sprintf_s(buffer, "Score : %d", score);
	renderBitmapString(-30, -30, GLUT_BITMAP_TIMES_ROMAN_24,buffer);
	
	glColor3f(1, 0.1, 1);
	renderBitmapString(-80, -275, GLUT_BITMAP_TIMES_ROMAN_24, "Press Space to Restart");

	glFlush();
}

void menu(int option) {
	if (option == 1) {
		resetGame();
	}
	if (option == 2) {
		exit(0);
	}
}

void display()
{
	if(introStatus == 0)
		introScreen();
	else if (gameOver == 1)
		gameOverScreen();
	else {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.196078, 0.6, 0.8, 1);
		glLoadIdentity();

		drawPipe(0, pipeSpawn[0], translatePipePos[0]);
		drawPipe(1, pipeSpawn[1], translatePipePos[1]);
		drawPipe(2, pipeSpawn[2], translatePipePos[2]);
		drawScore();
		drawBird();

		glFlush();
	}
}

void init()
{
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-300, 300, -300, 300);
	glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(40, 40);
	glutCreateWindow("Flappy Bird");
	glutDisplayFunc(display);
	glutMouseFunc(birdMovementMouse);
	glutKeyboardFunc(birdMovementKey);

	glutCreateMenu(menu);
	glutAddMenuEntry("Restart", 1);
	glutAddMenuEntry("Exit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	pipeSpawner(0);
	birdPhysics(0);

	glutMainLoop();
}