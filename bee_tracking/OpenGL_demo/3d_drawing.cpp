#include <GL/glut.h>
#include <math.h>

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

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
#define PI 3.14159f
#define CIRCLE_NUM 5
#define RADIUS 100.f
#define STEP 0.5f
			GLfloat x = 0, y = 0, z = -RADIUS, angle = 0.0;
			for (angle = 0; angle <= 2.0f * PI * CIRCLE_NUM; angle += 0.05f)
			{
				x = RADIUS * sin(angle);
				y = RADIUS * cos(angle);
				z += STEP;
				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}

	{
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
		{
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(200.0f, 0.0f, 0.0f);
			glVertex3f(200.0f, 0.0f, 0.0f);
			glVertex3f(193.0f, 3.0f, 0.0f);
			glVertex3f(200.0f, 0.0f, 0.0f);
			glVertex3f(193.0f, -3.0f, 0.0f);
		}
		glEnd();

		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINES);
		{
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 200.0f, 0.0f);
			glVertex3f(0.0f, 200.0f, 0.0f);
			glVertex3f(3.0f, 193.0f, 0.0f);
			glVertex3f(0.0f, 200.0f, 0.0f);
			glVertex3f(-3.0f, 193.0f, 0.0f);
		}
		glEnd();
	}

	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		{
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 200.0f);
			glVertex3f(0.0f, 0.0f, 200.0f);
			glVertex3f(0.0f, 3.0f, 193.0f);
			glVertex3f(0.0f, 0.0f, 200.0f);
			glVertex3f(0.0f, -3.0f, 193.0f);
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
	GLfloat nRange = 200.0f;

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

int main(int argc, char* argv[])
{
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