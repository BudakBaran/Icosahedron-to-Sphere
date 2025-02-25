#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <GL\freeglut.h>
#include <math.h>
#include <vector>

/*-------------------------------------------------------
-- General mathematical calculations starting -----------
-- I use mouse click for subdividing of a icosahedron ---
-------------------------------------------------------*/
GLfloat angle1 = 0.0;
int subNum = 0;
const float A = .525f;
const float B = .850f;
const float C = 0.0f;
static GLfloat vert[12][3] =
{
	 {-A,C,B}, {A,C,B}, {-A,C,-B}, {A,C,-B},
	 {C,B,A}, {C,B,-A}, {C,-B,A}, {C,-B,-A},
	 {B,A,C}, {-B,A, C}, {B,-A,C}, {-B,-A, C}
};
static GLuint tri[20][3] =
{
	 {0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
	 {8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
	 {7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
	 {6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
};

// Standart normalize code with using adresses
void normalize(float *v) {
	// a^2 + b^2 + c^2 = d^2 has length
	GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	// every component divide by length for have normalized vector
	v[0] /= d; v[1] /= d; v[2] /= d;

}

// better visulation for light system I have to draw normals too
void drawPolygon(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3]) {
	glNormal3fv(v1);
	glVertex3fv(v1);
	glNormal3fv(v2);
	glVertex3fv(v2);
	glNormal3fv(v3);
	glVertex3fv(v3);
}

void recursiveDivide(GLfloat* v1, GLfloat* v2, GLfloat* v3, int subNumb){

	float normalized1[3], normalized2[3], normalized3[3]; int i;
	if (subNumb == 0) {
		// every recursive function have to have root vector situation for printing.
		drawPolygon(v1, v2, v3);
		
		return;
	}
	for (i = 0; i < 3; i++) {
		// we have to have sub vectors for subdivided triangles( center point of combinate every vector ).
		normalized1[i] = (v1[i] + v2[i]) / 2.0;
		normalized2[i] = (v2[i] + v3[i]) / 2.0;
		normalized3[i] = (v3[i] + v1[i]) / 2.0;
	}
	// send starting adresses for change they in this function (normalize every point)
	normalize(&normalized1[0]);
	normalize(&normalized2[0]);
	normalize(&normalized3[0]);
	// every subdivide creates their function again and again untill subNum == 0 after subNum == 0 it return before function and get in second
	//recursive function so that way we have every triangle's subdivided triangle(all combination have to be included)
	recursiveDivide(v1, normalized1, normalized3, subNumb - 1); // v1 -> in normalized1 and normalized3
	recursiveDivide(v2, normalized2, normalized1, subNumb - 1);// v2 -> in normalized2 and normalized1
	recursiveDivide(v3, normalized3, normalized2, subNumb - 1);// v3 -> in normalized2 and normalized3
	recursiveDivide(normalized1, normalized2, normalized3, subNumb - 1); // center triangle

	// assume we have big triangle consisting of 4 triangles center triangle have mid points of each normal and other triangles created by
	//(the last recursive creates center triangle)
	// one center triangle normal + half normal of big triangle(first 3 recursive makes this) 

}

void drawObject()
{
	//  Draw Icosahedron start
	glBegin(GL_TRIANGLES);
	// subdivide have to >= 0
	if (subNum < 0) {
		subNum = 0;
	}
	// Every triangle in your coordinate system have to created or have to be subdivided
	for (int i = 0; i < 20; i++) {
		
		recursiveDivide(&vert[tri[i][0]][0], &vert[tri[i][1]][0], &vert[tri[i][2]][0], subNum);
	}
	glEnd();

}

/*---------------------------------------------
-- General mathematical calculations Ending--
-----------------------------------------------*/



/*-----------------------------------------------------------------------------------
-- General display options starting(Generally combination of lab 1 and lab 2 :(   )--
-------------------------------------------------------------------------------------*/
void write(GLfloat x, GLfloat y, char* text)
{
	char* p;

	glPushMatrix();
	glTranslatef(x, y, 0);
	for (p = text; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	glPopMatrix();
}
void myMouseFunc(int button, int action, int x, int y)
{
	// Mouse right and left click add or substract subNum for divide
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		subNum += 1;
		// for refreshing icosaheron to sphere we have to reset a list 1.
		glNewList(1, GL_COMPILE);
		drawObject();
		glEndList();
	}
	else if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) {
		subNum -= 1;
		// for refreshing icosaheron to sphere we have to reset a list 1.
		glNewList(1, GL_COMPILE);
		drawObject();
		glEndList();
	}
}

void idle(void)
{
	// For turning used in lab code
	angle1 = (GLfloat)fmod(angle1 + 0.01, 360);

	glutPostRedisplay();
}

void visible(int vis)
{
	if (vis == GLUT_VISIBLE)
		glutIdleFunc(idle);
	else
		glutIdleFunc(NULL);
}

void display(void)
{

	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { -1.0, 1.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glShadeModel(GL_FLAT);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat mat_a[] = { 0.1, 0.5, 0.8, 1.0 };
	GLfloat mat_d[] = { 0.1, 0.5, 0.8, 1.0 };
	GLfloat mat_s[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat low_sh[] = { 5.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_sh);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


	/////////////////////////////////////////////////////////
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(-0.3, -0.3, 0.0);
	glRotatef(angle1, 1.0, 5.0, 0.0);
	glCallList(1);        
	glPopMatrix();
	
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1600, 0, 1600);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	write(120, 425, (char*)"Left Mouse Click ++");
	write(120, 240, (char*)"Right Mouse Click --");
	write(120, 40, (char*)"For Subdivide");
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glMatrixMode(GL_MODELVIEW);
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Tesselate Function");
	glutDisplayFunc(display);
	glutVisibilityFunc(visible);
	
	glNewList(1, GL_COMPILE); 
	drawObject();
	glEndList();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(2.0);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(50.0, 1.0, 1.0, 10.0); //fov, aspect ratio, near, far
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.);  //eye, center, up
	glTranslatef(0.0, 0.6, -1.0);
	glutMouseFunc(myMouseFunc);
	glutMainLoop();
	return 0;
}



