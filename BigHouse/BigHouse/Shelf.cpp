#include "stdafx.h"
#include "Shelf.h"
#include <gl/GL.h>


Shelf::Shelf(int width, int length, int height, int count_floor)
	:height_(height),
	length_(length),
	width_(width),
	count_floor_(count_floor) {
	height_floor_ = (int)height/(count_floor_ + 1);
	std::pair<Floor, std::vector<Triangle3D*>> stock;
	for(int i = 0; i < count_floor_; i ++) {
		stock.first.height_floor = height_floor_;
		stock.first.color.Set(1, 1, 1);
		stocks_.push_back(stock);
	}
	bbmin_.Set(0, 0, 0);
	bbmax_.Set(width_, length_, height_);
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
	rec.p[1].Set(0, length, 0);
	rec.p[2].Set(width, length, 0);
	rec.p[3].Set(width, 0, 0);
	DrawFaceShelf(rec);

  rec.n.Set(1, 0, 0);
  rec.p[0].Set(width, 0 , 0);
	rec.p[1].Set(width, length, 0);
	rec.p[2].Set(width, length, height);
	rec.p[3].Set(width, 0, height);
	DrawFaceShelf(rec);

	rec.n.Set(-1, 0, 0);
  rec.p[0].Set(0, 0 , 0);
	rec.p[1].Set(0, 0, height);
	rec.p[2].Set(0, length, height);
	rec.p[3].Set(0, length, 0);
	DrawFaceShelf(rec);

  rec.n.Set(0, 1, 0);
	rec.p[0].Set(0, length , 0);
	rec.p[1].Set(0, length, height);
	rec.p[2].Set(width, length, height);
  rec.p[3].Set(width, length, 0);

	DrawFaceShelf(rec);

  rec.n.Set(0, -1, 0);
	rec.p[0].Set(0, 0 , 0);
	rec.p[1].Set(width, 0, 0);
	rec.p[2].Set(width, 0, height);
	rec.p[3].Set(0, 0, height);
	DrawFaceShelf(rec);
}

void Shelf::ShelfStructure() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_CULL_FACE);
	glColor3f(1, 0, 0);
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
		glTranslated(0,0,stocks_.at(i).first.height_floor);
		if(count_floor_ == i && stocks_.at(i).second.empty()) {
			glColor3f(0, 0, 1);
		} else {
		  glColor3f(1, 1, 0);
		}
	  DrawCube(width_, length_, 1);
	}
	glPopMatrix();
}

void Shelf::ReSetSelectFloor() {
	count_floor_ = -1;
}

void Shelf::DrawShelf() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  ShelfStructure();
	DrawShelfFloor();
	glPushMatrix();
	DrawCommodity();
	glPopMatrix();
}
void Shelf::DrawPoint() {
	glPointSize(6.0);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	glVertex3fv(point_on_floor_.v);
	glEnd();
}

void Shelf::GetBoundingBox(Vector3D &bbmin, Vector3D &bbmax) const{
	bbmin = bbmin_;
	bbmax = bbmax_;
}

void Shelf::SetBoundingBox(Vector3D &bbmin/*, Vector3D &bbmax*/){
	bbmin_ = bbmin;
	Vector3D p(width_, length_, height_);
	bbmax_ = bbmin + p;
}

void Shelf::GetBBmin(Vector3D &bbmin) const {
	bbmin = bbmin_;
}


void Shelf::PointMouseOnFloor(Vector3D &dir, Vector3D &pos) {
	Vector3D oz(0, 0, 1);
	if(dir.scalar(oz) == 0) {
		return;
	}
	Vector3D O(0, 0, 0);
	double t = (O.scalar(oz) - oz.scalar(pos))/(oz.scalar(dir));
	Vector3D p = dir*t;
	Vector3D point_plane = pos + p;
	int height = 0;
	bool has_point = false;
	Vector3D point_floor;
	count_floor_ = -1;
	for(int i = 0; i < stocks_.size(); i ++) {
		height  += stocks_.at(i).first.height_floor;
		Vector3D P1 = bbmin_;
		P1.v[2] = height;
		Vector3D P2 = bbmax_;
		P2.v[2] = height;
		Vector3D temp;
		if(LineCutSurface(dir, point_plane, oz,P1, P2, temp)) {
			if(!has_point) {
				point_floor = temp;
				count_floor_ = i;
				has_point = true;
			} else {
				Vector3D u = point_floor - temp;
				if(u.scalar(dir) > 0) {
					count_floor_ = i;
					point_floor = temp;
				}
			}
		}
	}
	point_on_floor_ = point_floor - bbmin_;
	//if(has_point) {
	//	stocks_.at(count).first.color.Set(0,0,1);
	//}

}

void Shelf::DrawCommodity() {
	glPushMatrix();
	glColor3f(0, 0, 1);
	glShadeModel(GL_SMOOTH);
	for(int i = 0 ; i < stocks_.size(); i ++) {

		glTranslated(0, 0, stocks_.at(i).first.height_floor); // trans z
	  glPushMatrix();
		if(!stocks_.at(i).second.empty()) {
			for(int j = 0; j < stocks_.at(i).first.s_r.i; j++) {
				if(j != 0)
				  glTranslated(stocks_.at(i).first.s_b.x,0,0); // trans x
				glPushMatrix();
				for(int k = 0 ; k < stocks_.at(i).first.s_r.j; k++) {
					if(k != 0)
					  glTranslated(0, stocks_.at(i).first.s_b.y, 0); //trans y
					glBegin(GL_TRIANGLES) ;
					for(int l = 0; l < stocks_.at(i).second.size(); l ++) {
						glNormal3fv(stocks_.at(i).second.at(l)->normal.v);
						glVertex3fv(stocks_.at(i).second.at(l)->m_v0.v);
						glVertex3fv(stocks_.at(i).second.at(l)->m_v1.v);
						glVertex3fv(stocks_.at(i).second.at(l)->m_v2.v);
					}
 					glEnd();
				}
				glPopMatrix();
			}
  	}
		glPopMatrix();
	}
	glPopMatrix();
	//glBegin()
}

bool Shelf::LineCutSurface(Vector3D &dir,
	                                Vector3D &pos,
																	Vector3D &n,
																	Vector3D &A,
																	Vector3D &B,
																	Vector3D &E) {
	if(n.scalar(dir) == 0 ) {
		return false;
	}
	double t = (n.scalar(A) - n.scalar(pos))/(n.scalar(dir));
	Vector3D temp = dir*t;
  E = pos + temp;
	for(int i = 0; i < 3; i ++) {
		if(n.v[i] == 0) {
			if(!(A.v[i] < E.v[i] && E.v[i] < B.v[i])) {
				return false;
			}
		}
	}
	return true;
}

void Shelf::SetCadToShelf(std::pair<Floor, std::vector<Triangle3D*>> &body) {
	if(count_floor_ != -1) {
		stocks_.at(count_floor_).first.s_r.i = (int)width_/body.first.s_b.x;
		stocks_.at(count_floor_).first.s_r.j = (int)length_/body.first.s_b.y;
		stocks_.at(count_floor_).first.s_b = body.first.s_b;
		stocks_.at(count_floor_).second = body.second;
		//stocks_.assign
	}
}