#include "stdafx.h"
#include "CircleShelf.h"
CirclShelf::CirclShelf(double r,//ban kinh ke
	                     double h,// chieu cao ke
											 double sp, // goc bat dau cua hinh tron
											 double ep,// goc ket thuc- voi ke hinh tron thi lay sp = 0, ep = 360 do
											 double angle, // goc don vi de tao ra mot hinh chu nhat tren mat tru cang nho thi cang min mang
											 double count_floor)// so san
 :radius_(r),
  height_(h) {
	std::pair<Floor, std::vector<Triangle3D*>> stock;
	height_floor_ = h / count_floor;
	for(int i = 0; i < count_floor; i ++) {
		stock.first.height_floor = height_floor_;
		stocks_.push_back(stock);
	}
	Origin_.Set(0 , 0, 0);
	count_floor_ = -1;

}

CirclShelf::~CirclShelf() {

}

bool CirclShelf::IsLineCutBody(const Vector3D &dir, const Vector3D& pos, Vector3D &p) {
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
void CirclShelf::DrawShelf() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_CULL_FACE);
	glColor3f(1, 0, 0);
	ShelfStructure(radius_, height_, 0, 360, 18); 
	glColor3f(1, 1, 0);
	DrawFloor(radius_, 1, 0, 360, 5);

}
void CirclShelf::SetCadToShelf(std::pair<Floor , std::vector<Triangle3D*>> &body) {

}
void CirclShelf::GetOriginBody(Vector3D &p_origin) {
	p_origin = Origin_;
}
void CirclShelf::SetOriginBody(Vector3D &p_move) {
	Origin_ = Origin_ + p_move;
	Vector3D temp(0, 0, height_);
	max_origin_ = Origin_ + temp;
}
void CirclShelf::PointMouseOnFloor(Vector3D &dir, Vector3D &pos) {
	count_floor_ = - 1;
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
			if(count_floor_ == -1){
			  temp = v;
		  	count_floor_ = i;
			} else {
				Vector3D sp = temp - v;
				if(sp.scalar(dir) < 0) {
					temp = v;
					count_floor_ = i;
				}
			}
		}
	}
}
void CirclShelf::ReSetSelectFloor() {
	count_floor_ = - 1;
}

void CirclShelf::ShelfStructure(double r, double h, double sp, double ep, double angle) {
	DrawCylinder(r, 10, sp, ep, 5);
	DrawCylinder(3, h, sp, ep, angle);

}

void CirclShelf::DrawFloor(double r, double h, double sp, double ep, double angle) {
	glPushMatrix();
	for(int i = 0; i < stocks_.size(); i ++) {
		glTranslatef(0 , 0, stocks_.at(i).first.height_floor);
		if(count_floor_ == i) {
			glColor3f(0, 0, 1);
		} else {
			glColor3f(1, 1, 0);
		}
		DrawCylinder(r, 1, sp, ep, angle);
	}
	glPopMatrix();

}
