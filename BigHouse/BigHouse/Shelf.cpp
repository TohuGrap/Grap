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

void Shelf::DrawCylinder(double r, double h, double sp, double ep , double angle) {
	double temp = 3.1415/180.0;
	float x;
	float y;
	float x0 = cos(sp*temp)*r;
	float y0 = sin(sp*temp)*r;
	int count = 0;
	Vector3D nor;
	for(int i = sp + angle ; i < ep; i = i + angle) {
		x = cos(i*temp)*r;
		y = sin(i*temp)*r;
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
		count  = i;
	}

	x0 = cos(count*temp)*r;
	y0 = sin(count*temp)*r;
	x = cos(ep*temp)*r;
	y = sin(ep*temp)*r;
	nor.Set((x + x0)/2.0, (y + y0)/2.0, 0); 
	glBegin(GL_POLYGON);
	glNormal3fv(nor.v);
	glVertex3f(x0, y0, 0);
	glVertex3f(x, y, 0);
	glVertex3f(x, y, h);
	glVertex3f(x0, y0, h);
	glEnd();

	// 
	//glEnable(GL_DEPTH_TEST);
	x0 = cos(sp*temp)*r;
	y0 = sin(sp*temp)*r;
	nor.Set(0, 0, - 1);
	glBegin(GL_TRIANGLES);
	for(int i = sp + angle ; i <= ep; i = i + angle) {
		x = cos(i*temp)*r;
		y = sin(i*temp)*r;
		glNormal3fv(nor.v);
		glVertex3f(0, 0, 0);
		glVertex3f(x, y, 0);
		glVertex3f(x0, y0, 0);
		x0 = x;
		y0 = y;
	}
	glEnd();
	nor.Set(0, 0, 1);
	x0 = cos(sp*temp)*r;
	y0 = sin(sp*temp)*r;
	glBegin(GL_TRIANGLES);
//	for(int i = ep - angle ; i > 0; i = i - angle) {
		for(int i = sp + angle ; i <= ep; i = i + angle) {
		x = cos(i*temp)*r;
		y = sin(i*temp)*r;
		glBegin(GL_TRIANGLES);
		glNormal3fv(nor.v);
		glVertex3f(0, 0, h);
		glVertex3f(x0, y0, h);
		glVertex3f(x, y, h);
		x0 = x;
		y0 = y;
	}
	glEnd();
}
