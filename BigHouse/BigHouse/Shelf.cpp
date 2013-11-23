#include "stdafx.h"
#include "Shelf.h"
#include <gl/GL.h>


Shelf::Shelf() {
	height_ = 160;
	length_ = 80;
	width_ = 50;
  height_floor_ = 25;
  count_floor_ = 6;
	std::pair<int, std::vector<Triangle3D*>> stock;
	for(int i = 0; i < count_floor_; i ++) {
		stock.first = height_floor_;
		stocks_.push_back(stock);
	}
}

Shelf::~Shelf() {

}

void Shelf::DrawFaceShelf(Rec &rec) {
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDisable(GL_CULL_FACE);
	glBegin(GL_POLYGON);
  	glNormal3fv(rec.n.v);
	  glVertex3fv(rec.p[0].v);
		glVertex3fv(rec.p[1].v);
		glVertex3fv(rec.p[2].v);
		glVertex3fv(rec.p[3].v);
	glEnd();
}

void Shelf::DrawCube(double width, double length, double height) {
	Rec rec;
	rec.n.Set(0, 0, 1);
	rec.p[0].Set(0, 0 , height);
	rec.p[1].Set(width, 0, height);
	rec.p[2].Set(width, length, height);
	rec.p[3].Set(0, length, height);
	DrawFaceShelf(rec);

	rec.n.Set(0, 0, -1);
  rec.p[0].Set(0, 0 , 0);
	rec.p[1].Set(width, 0, 0);
	rec.p[2].Set(width, length, 0);
	rec.p[3].Set(0, length, 0);
	DrawFaceShelf(rec);

  rec.n.Set(1, 0, 0);
  rec.p[0].Set(width, 0 , 0);
	rec.p[1].Set(width, length, 0);
	rec.p[2].Set(width, length, height);
	rec.p[3].Set(width, 0, height);
	DrawFaceShelf(rec);

	rec.n.Set(-1, 0, 0);
  rec.p[0].Set(0, 0 , 0);
	rec.p[1].Set(0, length, 0);
	rec.p[2].Set(0, length, height);
	rec.p[3].Set(0, 0, height);
	DrawFaceShelf(rec);

  rec.n.Set(0, 1, 0);
	rec.p[0].Set(0, length , 0);
	rec.p[1].Set(width, length, 0);
	rec.p[2].Set(width, length, height);
	rec.p[3].Set(0, length, height);
	DrawFaceShelf(rec);

  rec.n.Set(0, 1, 0);
	rec.p[0].Set(0, 0 , 0);
	rec.p[1].Set(width, 0, 0);
	rec.p[2].Set(width, 0, height);
	rec.p[3].Set(0, 0, height);
	DrawFaceShelf(rec);
}

void Shelf::ShelfStructure() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_CULL_FACE);
	glPushMatrix();
	glTranslated(0,0,1);
	DrawCube(width_, length_, 10); // de
	glPopMatrix();
	glPushMatrix();
	DrawCube(4, 4, height_);
	glTranslated(0, length_ - 4, 0);
	DrawCube(4, 4, height_);
	glPopMatrix();
}

void Shelf::DrawShelfFloor() {
	glPushMatrix();
	for(int i = 0; i < stocks_.size(); i ++){
	//	glPushMatrix();
		glTranslated(0,0,stocks_.at(i).first);
	  	DrawCube(width_, length_, 1);
		//glPopMatrix();
	}
	glPopMatrix();
}

void Shelf::DrawShelf() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  ShelfStructure();
	DrawShelfFloor();
}