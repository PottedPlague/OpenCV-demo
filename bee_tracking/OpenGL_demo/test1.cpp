#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <iterator>
#include <fstream>
#include <iostream>

using namespace std;

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

vector<double> coor_x;
vector<double> coor_y;

void RenderScene();
void SetupRC();
void ChangeSize(GLsizei w, GLsizei h);
void SpecialKeys(int key, int x, int y);


int main(int argc, char* argv[])
{
	ifstream isx("D:\\pic\\coordinate_x.txt");
	istream_iterator<double> startx(isx), endx;
	copy(startx, endx, back_inserter(coor_x));

	ifstream isy("D:\\pic\\coordinate_y.txt");
	istream_iterator<double> starty(isy), endy;
	copy(starty, endy, back_inserter(coor_y));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Point examples");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	SetupRC();
	glutMainLoop();

	return 1;
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	{
		glPointSize(9);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		{
			glVertex3f(0.0f, 0.0f, 0.0f);
		}
		glEnd();
	}

	{
		glColor3f(0.0f, 1.0f, 0.0f);
		glPointSize(3);
		glBegin(GL_POINTS);
		{
			for (int i = 0; i < coor_x.size(); i++)
				glVertex3f(coor_x[i], coor_y[i], -100.0f + 2.0f * i);
		}
		glEnd();
	}

	{
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
		{
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(500.0f, 0.0f, 0.0f);
			glVertex3f(500.0f, 0.0f, 0.0f);
			glVertex3f(480.0f, 20.0f, 0.0f);
			glVertex3f(500.0f, 0.0f, 0.0f);
			glVertex3f(480.0f, -20.0f, 0.0f);
		}
		glEnd();

		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINES);
		{
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 500.0f, 0.0f);
			glVertex3f(0.0f, 500.0f, 0.0f);
			glVertex3f(20.0f, 480.0f, 0.0f);
			glVertex3f(0.0f, 500.0f, 0.0f);
			glVertex3f(-20.0f, 480.0f, 0.0f);
		}
		glEnd();
	}

	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		{
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 500.0f);
			glVertex3f(0.0f, 0.0f, 500.0f);
			glVertex3f(0.0f, 20.0f, 480.0f);
			glVertex3f(0.0f, 0.0f, 500.0f);
			glVertex3f(0.0f, -20.0f, 480.0f);
		}
		glEnd();
	}
	glPopMatrix();

	glutSwapBuffers();
}

void SetupRC()
{
	glClearColor(0, 0.0, 0.0, 1);
	glColor3f(1.0f, 0.0f, 0.0f);
}

void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat nRange = 800.0f;

	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;

	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;

	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;

	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;

	if (xRot >356.0f)
		xRot = 0.0f;

	if (xRot < -1.0f)
		xRot = 355.0f;

	if (yRot > 356.0f)
		yRot = 0.0f;

	if (yRot < -1.0f)
		yRot = 355.0f;

	glutPostRedisplay();
}

