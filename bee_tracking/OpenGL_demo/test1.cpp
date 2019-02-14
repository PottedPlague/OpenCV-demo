#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <iterator>
#include <fstream>
#include <iostream>

using namespace std;

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

//vector<double> coor_x;
//vector<double> coor_y;
//vector<double> coor_z;
vector<vector<vector<double>>> tracks;

void RenderScene();
void SetupRC();
void ChangeSize(GLsizei w, GLsizei h);
void SpecialKeys(int key, int x, int y);


int main(int argc, char* argv[])
{
	/*ifstream isx("D:\\pic\\coordinate_x.txt");
	istream_iterator<double> startx(isx), endx;
	copy(startx, endx, back_inserter(coor_x));

	ifstream isy("D:\\pic\\coordinate_y.txt");
	istream_iterator<double> starty(isy), endy;
	copy(starty, endy, back_inserter(coor_y));*/

	ifstream is("D:/pic/tracks.txt");
	istream_iterator<double> start(is), end;
	copy(start, end, back_inserter(tracks));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("3D tracks");
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

	for (int k = 0; k < tracks.size(); k++)
	{
		glColor3f(0.0f, 1.0f, 0.0f);
		glPointSize(3);
		glBegin(GL_LINES);
		{
			for (int i = 1; i < tracks[k].size(); i++)
			{
				glVertex3f(tracks[k][i][0], tracks[k][i][1], tracks[k][i][2]);
				glVertex3f(tracks[k][i-1][0], tracks[k][i-1][1], tracks[k][i-1][2]);
			}
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

