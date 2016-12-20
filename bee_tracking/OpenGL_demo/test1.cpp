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
			glVertex3f(43.14f, 273.89f, 5.0f);
			glVertex3f(56.7963f, 276.692f, 10.0f);
			glVertex3f(69.7696f, 278.564f, 15.0f);
			glVertex3f(81.6148f, 279.915f, 20.0f);
			glVertex3f(91.6158f, 280.762f, 25.0f);
			glVertex3f(100.392f, 281.193f, 30.0f);
			glVertex3f(108.406f, 281.442f, 35.0f);
			glVertex3f(115.232f, 281.518f, 40.0f);
			glVertex3f(121.127f, 281.514f, 45.0f);
			glVertex3f(126.604f, 281.624f, 50.0f);
			glVertex3f(131.735f, 281.607f, 55.0f);
			glVertex3f(137.056f, 280.631f, 60.0f);
			glVertex3f(141.793f, 279.098f, 65.0f);
			glVertex3f(146.299f, 277.342f, 70.0f);
			glVertex3f(150.024f, 274.96f, 75.0f);
			glVertex3f(153.759f, 271.419f, 80.0f);
			glVertex3f(157.327f, 267.448f, 85.0f);
			glVertex3f(160.635f, 262.208f, 90.0f);
			glVertex3f(163.792f, 255.982f, 95.0f);
			glVertex3f(167.558f, 249.269f, 100.0f);
			glVertex3f(171.181f, 241.397f, 105.0f);
			glVertex3f(175.48f, 232.837f, 110.0f);
			glVertex3f(180.907f, 220.63f, 115.0f);
			glVertex3f(185.037f, 214.075f, 120.0f);
			glVertex3f(189.817f, 204.236f, 125.0f);
			glVertex3f(194.708f, 195.265f, 130.0f);
			glVertex3f(199.292f, 186.964f, 135.0f);
			glVertex3f(204.345f, 179.305f, 140.0f);
			glVertex3f(209.503f, 172.908f, 145.0f);
			glVertex3f(214.524f, 167.679f, 150.0f);
			glVertex3f(219.594f, 163.854f, 155.0f);
			glVertex3f(224.169f, 161.599f, 160.0f);
			glVertex3f(228.528f, 161.165f, 165.0f);
			glVertex3f(231.975f, 161.962f, 170.0f);
			glVertex3f(234.935f, 164.9f, 175.0f);
			glVertex3f(237.162f, 169.671f, 180.0f);
			glVertex3f(238.368f, 175.683f, 185.0f);
			glVertex3f(238.246f, 183.309f, 190.0f);
			glVertex3f(236.653f, 192.103f, 195.0f);
			glVertex3f(234.098f, 200.861f, 200.0f);
			glVertex3f(231.218f, 209.697f, 205.0f);
			glVertex3f(227.847f, 217.864f, 210.0f);
			glVertex3f(224.701f, 225.497f, 215.0f);
			glVertex3f(220.59f, 232.537f, 220.0f);
			glVertex3f(215.313f, 239.458f, 225.0f);
			glVertex3f(210.031f, 245.67f, 230.0f);
			glVertex3f(202.9f, 251.574f, 235.0f);
			glVertex3f(194.548f, 257.026f, 240.0f);
			glVertex3f(185.773f, 261.34f, 245.0f);
			glVertex3f(176.266f, 264.429f, 250.0f);
			
		}
		glEnd();
	}

	{
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
		{
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(300.0f, 0.0f, 0.0f);
			glVertex3f(300.0f, 0.0f, 0.0f);
			glVertex3f(293.0f, 3.0f, 0.0f);
			glVertex3f(300.0f, 0.0f, 0.0f);
			glVertex3f(293.0f, -3.0f, 0.0f);
		}
		glEnd();

		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINES);
		{
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 300.0f, 0.0f);
			glVertex3f(0.0f, 300.0f, 0.0f);
			glVertex3f(3.0f, 293.0f, 0.0f);
			glVertex3f(0.0f, 300.0f, 0.0f);
			glVertex3f(-3.0f, 293.0f, 0.0f);
		}
		glEnd();
	}

	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		{
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 300.0f);
			glVertex3f(0.0f, 0.0f, 300.0f);
			glVertex3f(0.0f, 3.0f, 293.0f);
			glVertex3f(0.0f, 0.0f, 300.0f);
			glVertex3f(0.0f, -3.0f, 293.0f);
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
	GLfloat nRange = 400.0f;

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