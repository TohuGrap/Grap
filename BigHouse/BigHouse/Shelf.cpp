

#include "stdafx.h"
#include "Shelf.h"

Shelf::Shelf() {
}

Shelf::~Shelf() {

}

void Shelf::DrawFaceShelf(Rect &rect) {
	glBegin(GL_POLYGON);
  	glNormal3fv(rect.n.v);
	  glVertex3fv(rect.p[0].v);
		glVertex3fv(rect.p[1].v);
		glVertex3fv(rect.p[2].v);
		glVertex3fv(rect.p[3].v);
	glEnd();
}

void Shelf::DrawCylinder(double r, double h, double sp, double ep,
												 double angle, bool color_blue) {
  /*
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	gluCylinder(quadratic,0.1f,0.1f,3.0f,32,32);
	*/

	double temp = M_PI/180.0;
	float x = 0.0;
	float y = 0.0;

	double se = ep - sp;
	int size = (int)(se/angle);
	double smooth_angle = se/size;
	double s_a = sp;
	float x0 = cos(sp*temp)*r;
	float y0 = sin(sp*temp)*r;
  Vector3D nor;
	nor.Set(0, 0, - 1);
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < size; i ++) {
		s_a += smooth_angle;
		x = cos(s_a*temp)*r;
		y = sin(s_a*temp)*r;
		glNormal3fv(nor.v);
		glVertex3f(0, 0, 0);
		glVertex3f(x, y, 0);
		glVertex3f(x0, y0, 0);
		x0 = x;
		y0 = y;
	}
	glEnd();

	s_a = sp;
	nor.Set(0, 0, 1);
	x0 = cos(sp*temp)*r;
	y0 = sin(sp*temp)*r;
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < size; i ++) {
		s_a += smooth_angle;
		x = cos(s_a*temp)*r;
		y = sin(s_a*temp)*r;
		glBegin(GL_TRIANGLES);
		glNormal3fv(nor.v);
		glVertex3f(0, 0, h);
		glVertex3f(x0, y0, h);
		glVertex3f(x, y, h);
		x0 = x;
		y0 = y;
	}
	glEnd();

	if(color_blue) {
		glColor3f(0, 0, 1);
	}
  s_a = sp;
	x0 = cos(sp*temp)*r;
	y0 = sin(sp*temp)*r;
	for(int i = 0; i < size; i ++) {
		s_a += smooth_angle;
		x = cos(s_a*temp)*r;
		y = sin(s_a*temp)*r;
		nor.Set((x + x0)/2.0, (y + y0)/2.0, 0); 
		glBegin(GL_POLYGON);
		glNormal3fv(nor.v);
		glVertex3f(x0, y0, 0);
		glVertex3f(x, y, 0);
		glVertex3f(x, y, h);
		glVertex3f(x0, y0, h);
		glEnd();
		x0 = x;
		y0 = y;
	}
}

void Shelf::DrawAllSizeOZ(DWORD TextList3D,
													double height_base,
										      std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks) {
	if(stocks.empty()) {
		return;
	}
	glPushMatrix();
	glTranslatef(0, 0, height_base);
	Vector3D A(0, 0, 0);
	Vector3D B(0, - 60, 0);
	DrawALine(A, B);
	for(int i = 1; i < stocks.size(); i ++) {
		glTranslatef(0, 0, stocks.at(i).first.height_floor);
		DrawSizeOZ(stocks.at(i).first.height_floor, TextList3D);
	}
	glPopMatrix();
}

void Shelf::DrawSizeOZ(double height,
	                     DWORD TextList3D) {
	glPushMatrix();

	Vector3D A(0, 0, 0);
	Vector3D B(0, - 60, 0);
	DrawALine(A, B);

	Vector3D P1(0, - 40, 0);
	Vector3D P0(0, - 40, - height);
	DrawARawOZ(P0, P1); //draw raws 
	DrawALine(P0, P1);

	glTranslatef(0, - 50, - height/2.0 - 11); 
	glRotatef(180, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	CString str = _T("");
	str.Format(_T("%.2f"), height);
	glListBase(TextList3D);
	int length = str.GetLength();
	glScalef(10,10,10);
	glCallLists(length, GL_SHORT , str);

	glPopMatrix();
}

void Shelf::DrawALine(Vector3D & A, Vector3D &B) {
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3fv(A.v);
	glVertex3fv(B.v);
	glEnd();
}

void Shelf::DrawARawOZ(Vector3D & A, Vector3D &B) {
	glPushMatrix();
	glPushMatrix();
	double size = 10;
	Vector3D P0(0, 0, 0);
	Vector3D P1(0, size/3.0, size);
	Vector3D P2(0, -size/3.0, size);
	glTranslatef(A.v[0], A.v[1], A.v[2]);
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	glNormal3f(1, 0, 0);
	glVertex3fv(P0.v);
	glVertex3fv(P1.v);
	glVertex3fv(P2.v);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(B.v[0], B.v[1], B.v[2]);
	glRotatef(180, 1, 0 ,0);
	glBegin(GL_TRIANGLES);
	glNormal3f(1, 0, 0);
	glVertex3fv(P0.v);
	glVertex3fv(P1.v);
	glVertex3fv(P2.v);
	glEnd();
	glPopMatrix();

	glPopMatrix();
}
