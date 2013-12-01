#include "stdafx.h"
#include"RecShelf.h"
#include "Struct.h"

RecShelf::RecShelf(int width, int length, int height, int count_floor, TypeRecShelf type) 
	:height_(height),
	length_(length),
	width_(width),
	count_floor_(count_floor),
  type_(type){
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

RecShelf::RecShelf() {

}

RecShelf::~RecShelf() {

}
void RecShelf::DrawFaceShelf(Rect &rec) {
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

void RecShelf::DrawCube(double width, double length, double height) {
	Rect rec;
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

void RecShelf::ShelfStructure(double width, double length, double height, TypeRecShelf type) {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_CULL_FACE);
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(0,0,1);
	DrawCube(width, length, 10); // de
	glPopMatrix();
	glPushMatrix();
	if(type == BACK || type == FONT) {
		if(type == BACK) {
			glTranslatef(width, 0, 0);
		} else {
			glTranslatef(- 4, 0, 0);
		}
		DrawCube(4, 4, height);
		glTranslated(0, length - 4, 0);
		DrawCube(4, 4, height);
	} else {
		if(type == LEFT) {
		  glTranslatef(0, -4 , 0);
		} else {
			glTranslatef(0, length , 0);
		}
		DrawCube(4, 4, height);
		glTranslated(width - 4, 0, 0);
		DrawCube(4, 4, height);
	}

	glPopMatrix();
}

void RecShelf::DrawShelfFloor(int width,
	                            int length, 
															int heigth,
															int count,
															std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks) {
	glPushMatrix();
	for(int i = 0; i < stocks.size(); i ++){
		glTranslated(0, 0, stocks.at(i).first.height_floor);
		if(count == i && stocks.at(i).second.empty()) {
			glColor3f(0, 0, 1);
		} else {
		  glColor3f(1, 1, 0);
		}
	  DrawCube(width, length, heigth);
	}
	glPopMatrix();
}

//void RecShelf::ReSetSelectFloor() {
//	count_floor_ = -1;
//}

void RecShelf::DrawShelf() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	ShelfStructure(width_ ,length_, height_, type_);
	DrawShelfFloor(width_, length_, 1, count_floor_, stocks_);
	DrawCommodity(stocks_);
}
//void RecShelf::DrawPoint() {
//	glPointSize(6.0);
//	glColor3f(1, 0, 0);
//	glBegin(GL_POINTS);
//	//glVertex3fv(point_on_floor_.v);
//	glEnd();
//}

void RecShelf::GetBoundingBox(Vector3D &bbmin, Vector3D &bbmax) const{
	bbmin = bbmin_;
	bbmax = bbmax_;
}

void RecShelf::SetBoundingBox(Vector3D &bbmin){
	bbmin_ = bbmin;
	Vector3D p(width_, length_, height_);
	bbmax_ = bbmin + p;
}

void RecShelf::GetBBmin(Vector3D &bbmin) const {
	bbmin = bbmin_;
}

int RecShelf::FindPointMouseOnFloor(Vector3D &dir,
	                                   Vector3D &pos,
																		  Vector3D &bbmin,
														          Vector3D &bbmax,
																		 std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks) {
	Vector3D oz(0, 0, 1);
	if(dir.scalar(oz) == 0) {
		return - 1;
	}
	int count = -1;
	Vector3D O(0, 0, 0);
	double t = (O.scalar(oz) - oz.scalar(pos))/(oz.scalar(dir));
	Vector3D p = dir*t;
	Vector3D point_plane = pos + p;
	int height = 0;
	bool has_point = false;
	Vector3D point_floor;
	for(int i = 0; i < stocks.size(); i ++) {
		height  += stocks.at(i).first.height_floor;
		Vector3D P1 = bbmin;
		P1.v[2] = height;
		Vector3D P2 = bbmax;
		P2.v[2] = height;
		Vector3D temp;
		if(LineCutSurface(dir, point_plane, oz,P1, P2, temp)) {
			if(!has_point) {
				point_floor = temp;
				count = i;
				has_point = true;
			} else {
				Vector3D u = point_floor - temp;
				if(u.scalar(dir) < 0) {
					count = i;
					point_floor = temp;
				}
			}
		}
	}
	return count;
}


void RecShelf::PointMouseOnFloor(Vector3D &dir, Vector3D &pos) {
	count_floor_ = FindPointMouseOnFloor(dir, pos, bbmin_, bbmax_,stocks_);
	//Vector3D oz(0, 0, 1);
	//if(dir.scalar(oz) == 0) {
	//	return;
	//}
	//Vector3D O(0, 0, 0);
	//double t = (O.scalar(oz) - oz.scalar(pos))/(oz.scalar(dir));
	//Vector3D p = dir*t;
	//Vector3D point_plane = pos + p;
	//int height = 0;
	//bool has_point = false;
	//Vector3D point_floor;
	//count_floor_ = -1;
	//for(int i = 0; i < stocks_.size(); i ++) {
	//	height  += stocks_.at(i).first.height_floor;
	//	Vector3D P1 = bbmin_;
	//	P1.v[2] = height;
	//	Vector3D P2 = bbmax_;
	//	P2.v[2] = height;
	//	Vector3D temp;
	//	if(LineCutSurface(dir, point_plane, oz,P1, P2, temp)) {
	//		if(!has_point) {
	//			point_floor = temp;
	//			count_floor_ = i;
	//			has_point = true;
	//		} else {
	//			Vector3D u = point_floor - temp;
	//			if(u.scalar(dir) < 0) {
	//				count_floor_ = i;
	//				point_floor = temp;
	//			}
	//		}
	//	}
	//}
}

void RecShelf::DrawCommodity(std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks) {
	glPushMatrix();
	glColor3f(0, 0, 1);
	glShadeModel(GL_SMOOTH);
	for(int i = 0 ; i < stocks.size(); i ++) {

		glTranslated(0, 0, stocks.at(i).first.height_floor); // trans z
	  glPushMatrix();
		if(!stocks.at(i).second.empty()) {
			for(int j = 0; j < stocks.at(i).first.s_r.i; j++) {
				if(j != 0)
				  glTranslated(stocks.at(i).first.s_b.x,0,0); // trans x
				glPushMatrix();
				for(int k = 0 ; k < stocks.at(i).first.s_r.j; k++) {
					if(k != 0)
					  glTranslated(0, stocks.at(i).first.s_b.y, 0); //trans y
					glBegin(GL_TRIANGLES) ;
					for(int l = 0; l < stocks.at(i).second.size(); l ++) {
						glNormal3fv(stocks.at(i).second.at(l)->normal.v);
						glVertex3fv(stocks.at(i).second.at(l)->m_v0.v);
						glVertex3fv(stocks.at(i).second.at(l)->m_v1.v);
						glVertex3fv(stocks.at(i).second.at(l)->m_v2.v);
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

bool RecShelf::LineCutSurface(Vector3D &dir,
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

void RecShelf::SetCadToShelf(std::pair<Floor, std::vector<Triangle3D*>> &body) {
	if(count_floor_ != -1) {
		stocks_.at(count_floor_).first.s_r.i = (int)width_/body.first.s_b.x;
		stocks_.at(count_floor_).first.s_r.j = (int)length_/body.first.s_b.y;
		stocks_.at(count_floor_).first.s_b = body.first.s_b;
		stocks_.at(count_floor_).second = body.second;
		//stocks_.assign
	}
}

//**********************************&&*************************

bool RecShelf::LineCutBoundingBox(const Vector3D &dir, const Vector3D &pos, Vector3D &bbmin, Vector3D &bbmax, Vector3D &p_on_bb) {
	Vector3D normal;
	Vector3D E;
	bool has_a_point = false;
	for(int i = 0; i < 3; i++) {
		Vector3D p = bbmax;
		p.v[i] = bbmin.v[i];
		normal.Set(0, 0, 0);
		normal.v[i] = 1;
		if(LineCutSurface((Vector3D)dir, (Vector3D)pos, normal, bbmin, p, E)) {
			if(!has_a_point) {
				p_on_bb = E;
			  has_a_point = true;
			} else {
				Vector3D temp = p_on_bb - E;
				if(temp.scalar(dir) < 0) {
					p_on_bb = E;
				}
			}
		}
	}
	for(int i = 0; i < 3; i++) {
		Vector3D p = bbmin;
		p.v[i] = bbmax.v[i];
		normal.Set(0, 0, 0);
		normal.v[i] = 1;
		if(LineCutSurface((Vector3D)dir, (Vector3D)pos, normal, p, bbmax, E)) {
			if(!has_a_point) {
				p_on_bb = E;
			  has_a_point = true;
			} else {
				Vector3D temp = p_on_bb - E;
				if(temp.scalar(dir) < 0) {
					p_on_bb = E;
				}
			}
		}
	}
	return has_a_point;
}


bool RecShelf::IsLineCutBody(const Vector3D &dir, const Vector3D& pos, Vector3D &p) {
	return LineCutBoundingBox(dir, pos, bbmin_, bbmax_, p);
}

void RecShelf::GetOriginBody(Vector3D &p_origin) {
	p_origin = bbmin_;
}

void RecShelf::SetOriginBody(Vector3D &p_move) {
	bbmin_ = bbmin_ + p_move;
  Vector3D p(width_, length_, height_);
	bbmax_ = bbmin_ + p;

}
