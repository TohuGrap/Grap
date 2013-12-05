#include "stdafx.h"
#include "RecShelfFont_Back.h"
RectShelfFront_Back::RectShelfFront_Back(float width,
	                                   float length,
																		 float height, 
																		 UINT floor_count,
																		 DirectionShelf shelf_direction) :
	height_(height),
	length_(length),
	width_(width),
	shelf_direction_(shelf_direction) {
	floor_height_ = (int)height/(floor_count);
	std::pair<Floor, std::vector<Triangle3D*>> stock;
	for(int i = 0; i < floor_count; i++) {
		stock.first.height_floor = floor_height_;
		stocks_font_.push_back(stock);
		stocks_back_.push_back(stock);
	}

	bbmin_back_.Set(0, 0, 0);
  floor_count_ = -1;
	type_ = FLOOR_FONT;
}

RectShelfFront_Back::~RectShelfFront_Back() {

}

bool RectShelfFront_Back::IsLineCutBody(const Vector3D &dir,
																			const Vector3D& pos,
																			Vector3D &p) {
	bool has_point = LineCutBoundingBox(dir, pos, bbmin_back_, bbmax_back_, p);
	type_ = FLOOR_BACK;
	if (has_point) {
		Vector3D temp;
		if(LineCutBoundingBox(dir, pos, bbmin_font_, bbmax_font_, temp)) {
			Vector3D u = p - temp;
			if(u.scalar(dir) < 0) {
				p = temp;
			  type_ = FLOOR_FONT;
			}
		} 
	} else {
			has_point = LineCutBoundingBox(dir, pos, bbmin_font_, bbmax_font_, p);
			type_ = FLOOR_FONT;
	}
	return has_point;
}

void RectShelfFront_Back::DrawShelf() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_CULL_FACE);
	double h_solo = height_/10.0;
	if(d_shelf_ == FONT_BACK) {
		ShelfStructure(width_ ,length_, height_,stocks_back_.size(), height_floor_, h_solo, BACK);
		if(type_ == FLOOR_BACK) { 
			DrawShelfFloor(width_, length_, 1 , h_solo, count_floor_, stocks_back_);
		} else {
			DrawShelfFloor(width_, length_, 1 , h_solo, -1, stocks_back_);
		}
		DrawCommodity(stocks_back_,  h_solo);
		glPushMatrix();
		glTranslatef(width_ + 4, 0, 0 );
		ShelfStructure(width_ ,length_, height_,stocks_back_.size(), height_floor_, h_solo, FONT, false);
		if(type_ == FLOOR_FONT) { 
			DrawShelfFloor(width_, length_, 1 , h_solo, count_floor_, stocks_font_);
		} else {
			DrawShelfFloor(width_, length_, 1 , h_solo, -1, stocks_font_);
		}
		DrawCommodity(stocks_font_,  h_solo);
		glPopMatrix();
	} else {
		ShelfStructure(width_ ,length_, height_,stocks_back_.size(), height_floor_, h_solo, LEFT);
		if(type_ == FLOOR_BACK) { 
			DrawShelfFloor(width_, length_, 1 , h_solo, count_floor_, stocks_back_);
		} else {
			DrawShelfFloor(width_, length_, 1 , h_solo, -1, stocks_back_);
		}
		DrawCommodity(stocks_back_, h_solo);
		glPushMatrix();
		glTranslatef(0, length_ + 4, 0 );
		ShelfStructure(width_ ,length_, height_,stocks_back_.size(), height_floor_, h_solo, RIGHT, false);
		if(type_ == FLOOR_FONT) { 
			DrawShelfFloor(width_, length_, 1 , h_solo, count_floor_, stocks_font_);
		} else {
			DrawShelfFloor(width_, length_, 1 , h_solo, -1, stocks_font_);
		}
		DrawCommodity(stocks_font_, h_solo);
		glPopMatrix();
	}

}
void RectShelfFront_Back::SetCadToShelf(std::pair<Floor , std::vector<Triangle3D*>> &body) {
	if (floor_count_ != -1) {
	  if (type_ == FLOOR_FONT) {
			stocks_font_.at(floor_count_).first.s_r.i = (int)width_/body.first.s_b.x;
			stocks_font_.at(floor_count_).first.s_r.j = (int)length_/body.first.s_b.y;
			stocks_font_.at(floor_count_).first.s_b = body.first.s_b;
			stocks_font_.at(floor_count_).second = body.second;
			//stocks_.assign
		} else if (type_ == FLOOR_BACK) {
			stocks_back_.at(floor_count_).first.s_r.i = (int)width_/body.first.s_b.x;
			stocks_back_.at(floor_count_).first.s_r.j = (int)length_/body.first.s_b.y;
			stocks_back_.at(floor_count_).first.s_b = body.first.s_b;
			stocks_back_.at(floor_count_).second = body.second;
		}
	}
}
void RectShelfFront_Back::GetOriginBody(Vector3D &p_origin) {
	p_origin = bbmin_back_;

}
void RectShelfFront_Back::SetOriginBody(Vector3D &p_move) {
	Vector3D temp(width_, length_, height_);
	bbmin_back_ = bbmin_back_ + p_move;
	bbmax_back_ = bbmin_back_ + temp;
	Vector3D temp1;
	if (shelf_direction_ == FONT_BACK) {
		temp1.Set(width_, 0 , 0);
	  bbmin_font_ = bbmin_back_ + temp1;
	  bbmax_font_ = bbmin_font_ + temp;
	} else {
		temp1.Set(0, length_ , 0);
	  bbmin_font_ = bbmin_back_ + temp1;
	  bbmax_font_ = bbmin_font_ + temp;
	}
}
void RectShelfFront_Back::PointMouseOnFloor(Vector3D &dir, Vector3D &pos) {
	if (type_ == FLOOR_FONT) {
		floor_count_ = FindPointMouseOnFloor(dir, pos, bbmin_font_, bbmax_font_, stocks_font_);
	} else {
	  floor_count_ = FindPointMouseOnFloor(dir, pos, bbmin_back_, bbmax_back_, stocks_back_);
	}
}
void RectShelfFront_Back::ReSetSelectFloor() {
	floor_count_ = - 1;
}