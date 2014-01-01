
#include "stdafx.h"
#include "CircleShelf.h"
CircleShelf::CircleShelf(double radius,
	                     double height,
											 double start_angle,
											 double end_angle,
											 double flat_angle,
											 UINT floor_count) :
	radius_(radius),
  height_(height),
	start_angle_(start_angle),
	end_angle_(end_angle),
	flat_angle_(flat_angle),
  smooth_angle_(flat_angle),
	fcounts(floor_count) {
		std::pair<Floor, std::vector<Triangle3D*>> stock;
		height_floor_ = (height - height/15.0) / fcounts;
		for(int i = 0; i < floor_count; ++i) {
			stock.first.height_floor = height_floor_;
			stocks_.push_back(stock);
		}
		Origin_.Set(0 , 0, 0);
	  floor_count_ = -1;
		//is_circle_shelf_ = true;
}

CircleShelf::~CircleShelf() {

}

bool CircleShelf::IsLineCutBody(const Vector3D &dir, const Vector3D& pos, Vector3D &p) {
	double x = pos.v[0] - Origin_.v[0];
	double y = pos.v[1] - Origin_.v[1];
	double b = (dir.v[0]*x + dir.v[1]*y);
	double a = dir.v[0]*dir.v[0] + dir.v[1]*dir.v[1];
	double c = x*x + y*y - radius_*radius_;
	if(a == 0) {
		if(c > 0) {
			return false;
		} else {
			p.Set(pos.v[0], pos.v[1], 0);
			return true;
		}
	}
	double delta = b*b - a*c;
	if(delta < 0) {
		return false;
	} else if(delta == 0) { 
		float t = - b /a;
		Vector3D u = (Vector3D)dir*t;
		u = u + (Vector3D)pos;
		if(!(u.v[2] >= Origin_.v[2] && u.v[2] <= max_origin_.v[2]))
			return false;
		p = u;
		return true;
	} else {
		double t1 = -b -sqrt(delta);
		t1 = t1/a;
		double t2 = -b +sqrt(delta);
		t2 = t2 /a;
		Vector3D u = (Vector3D)dir*t1;
		u = (Vector3D)pos + u;
		Vector3D v = (Vector3D)dir*t2;
		v = v + (Vector3D)pos;
		Vector3D w = v - u;
		Vector3D k = dir;
		if(k.scalar(w) < 0 && u.v[2] >= Origin_.v[2] && u.v[2] <= max_origin_.v[2]) {
		  p = u;
			return true;
		} else if(v.v[2] >= Origin_.v[2] && v.v[2] <= max_origin_.v[2] ){
			p = v;
			return true;
		}
	}
	Vector3D oz(0, 0, 1);
	if(oz.scalar(dir) == 0) {
		return false;
	}
	double t = (oz.scalar(Origin_) - oz.scalar(pos))/(oz.scalar(dir));
	Vector3D u = (Vector3D)dir*t;
	u = (Vector3D)pos + u;
	Vector3D v = u - Origin_;
	if(v.abs() <= radius_) {
		p = u;
		return true;
	}
	t = (oz.scalar(max_origin_) - oz.scalar(pos))/(oz.scalar(dir));
	u = (Vector3D)dir*t;
	u = u + (Vector3D)pos;
	v = u - max_origin_;
	if(v.abs() <= radius_) {
		p = u;
		return true;
	}
	return false;
}
void CircleShelf::DrawShelf() {
	glColor3f(1.0, 0, 0);
	CircleShelfFrame(radius_, height_, start_angle_, end_angle_, flat_angle_); 
	glColor3f(1.0, 1.0, 0);
	double h_solo = height_/12.0;
	DrawFloor(radius_, h_solo/5.0, start_angle_, end_angle_, flat_angle_, h_solo);
  DrawCommodity(stocks_, h_solo, radius_);
	glPushMatrix();
		DrawCommodity(stocks_, h_solo, radius_);
		glRotatef(90, 0, 0, 1);
		DrawCommodity(stocks_, h_solo, radius_);
		glRotatef(90, 0, 0, 1);
		DrawCommodity(stocks_, h_solo, radius_);
		glRotatef(90, 0, 0, 1);
		DrawCommodity(stocks_, h_solo, radius_);
		glRotatef(90, 0, 0, 1);
	glPopMatrix();
	glPopMatrix();
}
void CircleShelf::SetCadToShelf(std::pair<Floor , std::vector<Triangle3D*>> &body) {
	if(floor_count_ != -1) {
		int i = (int)(radius_/body.first.s_b.x);
		int j = (int)(radius_/body.first.s_b.y);
		stocks_.at(floor_count_).first.s_r.i = i;
		stocks_.at(floor_count_).first.s_r.j = j;
		stocks_.at(floor_count_).first.s_b = body.first.s_b;
		stocks_.at(floor_count_).first.o_floor.v[0] = - i*body.first.s_b.x;
		stocks_.at(floor_count_).first.o_floor.v[1] = - j*body.first.s_b.y;
		stocks_.at(floor_count_).first.o_floor.v[2] = 0;
		stocks_.at(floor_count_).second = body.second;
	}
}
void CircleShelf::GetOriginBody(Vector3D &p_origin) {
	p_origin = Origin_;
}
void CircleShelf::SetOriginBody(Vector3D &p_move) {
	Origin_ = Origin_ + p_move;
	Vector3D temp(0, 0, height_);
	max_origin_ = Origin_ + temp;
}
void CircleShelf::PointMouseOnFloor(Vector3D &dir, Vector3D &pos) {
	floor_count_ = - 1;
	Vector3D oz(0, 0, 1);
	if(oz.scalar(dir) == 0) {
		return;
	}
	Vector3D temp;
	Vector3D O = Origin_;
	double count = 0;
	for(int i = 0; i < stocks_.size(); i ++) {
		count = count + stocks_.at(i).first.height_floor;
		O.v[2] = count;
		double t = (oz.scalar(O) - oz.scalar(pos))/(oz.scalar(dir));
		Vector3D u = (Vector3D)dir*t;
		u = (Vector3D)pos + u;
		Vector3D v = u - O;
		if(v.abs() <= radius_ ) {
			if(floor_count_ == -1){
			  temp = v;
		  	floor_count_ = i;
			} else {
				Vector3D sp = temp - v;
				if(sp.scalar(dir) < 0) {
					temp = v;
					floor_count_ = i;
				}
			}
		}
	}
}
void CircleShelf::ReSetSelectFloor() {
	floor_count_ = - 1;
}

void CircleShelf::CircleShelfFrame(double r, double h, double sp, double ep, double angle) {
	DrawCylinder(r, h/12.0, sp, ep, angle);
	DrawCylinder(r/15.0, h, sp, ep, angle);
}

void CircleShelf::DrawFloor(double r, double h, double sp, double ep, double angle, double height_solo) {
	glPushMatrix();
  glTranslatef(0 , 0, height_solo);
	if(floor_count_ == 0) {
		glColor3f(0, 0, 1);
	  DrawCylinder(r, h, sp, ep, angle, true);
	}
	for(int i = 1; i < stocks_.size(); i ++) {
		glTranslatef(0 , 0, stocks_.at(i).first.height_floor);
		if(floor_count_ == i) {
			glColor3f(0, 0, 1);
		} else {
			glColor3f(1.0, 1.0, 0.0);
		}
		DrawCylinder(r, h, sp, ep, angle, true);
	}
	glPopMatrix();

}

void CircleShelf::DrawCommodity(std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks, double h_solo,
															 double radius) {
	glPushMatrix();
	glColor3f(0, 0, 1);
	glShadeModel(GL_SMOOTH);
	for(int i = 0 ; i < stocks.size(); i ++) {
		if(i ==  0) {
      glTranslated(0, 0, h_solo);
		} else {
		  glTranslated(0, 0, stocks.at(i).first.height_floor); // trans z
		}
		if(stocks.at(i).second.empty()) {
			continue;
		}
	  glPushMatrix();
		Vector3D pos_cad = stocks.at(i).first.o_floor;
		RectBody rect = stocks.at(i).first.s_b;
		glTranslatef(pos_cad.v[0], pos_cad.v[1], pos_cad.v[2]); // trab to origin - floor
		if(!stocks.at(i).second.empty()) {
			for(int j = 0; j < stocks.at(i).first.s_r.i; j++) {
				if(j != 0)
				  glTranslated(stocks.at(i).first.s_b.x,0,0); // trans x
				glPushMatrix();
				for(int k = 0 ; k < stocks.at(i).first.s_r.j; k++) {
					if(k != 0)
					  glTranslated(0, stocks.at(i).first.s_b.y, 0); //trans y
					if(IsCadInCircle(pos_cad, rect, radius)) { 
						glBegin(GL_TRIANGLES) ;
						for(int l = 0; l < stocks.at(i).second.size(); l ++) {
							glNormal3fv(stocks.at(i).second.at(l)->normal.v);
							glVertex3fv(stocks.at(i).second.at(l)->m_v0.v);
							glVertex3fv(stocks.at(i).second.at(l)->m_v1.v);
							glVertex3fv(stocks.at(i).second.at(l)->m_v2.v);
						}
 						glEnd();
					}
					pos_cad.v[1] = pos_cad.v[1] + rect.y ;//
				}
				glPopMatrix();
				pos_cad.v[1] = stocks.at(i).first.o_floor.v[1];
				pos_cad.v[0] = pos_cad.v[0] + rect.x ;//
			}
  	}
		glPopMatrix();
	}
	glPopMatrix();
	//glBegin()
}

bool CircleShelf::IsCadInCircle(Vector3D &o_floor, RectBody & rect, double radius) {
	Vector3D pos(0, 0, 0);
	if(o_floor.v[0] < - rect.x/2.0 && o_floor.v[1] < - rect.y/2.0) {
	 	if(o_floor.abs() <= radius) {
		  return true;
	  }
	}
	return false;
}
