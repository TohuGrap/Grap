#include "stdafx.h"
#include "Shelf.h"
#include <gl/GL.h>
#include <gl/GLU.h>


Shelf::Shelf() {
}

Shelf::~Shelf() {

}

void Shelf::DrawFaceShelf(Rect &rec) {
	glBegin(GL_POLYGON);
  	glNormal3fv(rec.n.v);
	  glVertex3fv(rec.p[0].v);
		glVertex3fv(rec.p[1].v);
		glVertex3fv(rec.p[2].v);
		glVertex3fv(rec.p[3].v);
	glEnd();
}

void Shelf::DrawCylinder(double r, double h, double sp, double ep , double angle, bool color_blue) {
	double temp = 3.1415/180.0;
	float x;
	float y;

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
