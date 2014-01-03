#include "stdafx.h"
#include "RecShelfFont_Back.h"
RectShelfFront_Back::RectShelfFront_Back(float width,
	                                   float length,
																		 float height, 
																		 UINT floor_count) :
	height_(height),
	length_(length),
	width_(width),
	shelf_direction_(DirectionShelf::FONT_BACK) {
	floor_height_ = (int)height/(floor_count);
	std::pair<Floor, std::vector<Triangle3D*>> stock;
	for(int i = 0; i < floor_count; i++) {
		stock.first.height_floor = floor_height_;
		stocks_font_.push_back(stock);
		stocks_back_.push_back(stock);
	}

	bbmin_back_.Set(0, 0, 0);
  selected_floor_ = -1;
	direct_shelf_ = FLOOR_BACK;
}

RectShelfFront_Back::~RectShelfFront_Back() {

}

bool RectShelfFront_Back::ObjectIsSelectedByLeftMouse(const Vector3D &dir,
																			const Vector3D& pos,
																			Vector3D &p) {
	bool has_point = LineCutBoundingBox(dir, pos, bbmin_back_, bbmax_back_, p);
	direct_shelf_ = FLOOR_BACK;
	if (has_point) {
		Vector3D temp;
		if(LineCutBoundingBox(dir, pos, bbmin_font_, bbmax_font_, temp)) {
			Vector3D u = p - temp;
			if(u.scalar(dir) < 0) {
				p = temp;
			  direct_shelf_ = FLOOR_FONT;
			}
		} 
	} else {
			has_point = LineCutBoundingBox(dir, pos, bbmin_font_, bbmax_font_, p);
			direct_shelf_ = FLOOR_FONT;
	}
	return has_point;
}

void RectShelfFront_Back::DrawShelf() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_CULL_FACE);
	double h_solo = height_/10.0;
	if(shelf_direction_ != FONT_BACK) {
		glRotatef(90, 0, 0, 1);
	}
		glTranslatef(0, -length_/2.0, 0);
		int selected_floor;
		if(direct_shelf_ == FLOOR_FONT) {
			selected_floor = selected_floor_;
		} else {
			selected_floor = -1;
		}
		ShelfStructure(width_ ,
			             length_, 
									 height_,
									 floor_height_,
									 h_solo, 
									 selected_floor,
									 stocks_font_);
		DrawCommodity(stocks_font_,  h_solo);
		DrawAllSizeOZDR(513, h_solo, stocks_font_);
		// draw ke thu 2
		glPushMatrix();
		glRotatef(180, 0, 0, 1);
		glTranslatef(0, - length_, 0 );
		if(direct_shelf_ == FLOOR_BACK) {
			selected_floor = selected_floor_;
		} else {
			selected_floor = -1;
		}
		ShelfStructure(width_ ,
			             length_,
									 height_,
									 floor_height_,
									 h_solo,
									 selected_floor,
									 stocks_back_,
									 false);
		DrawCommodity(stocks_back_,  h_solo);
		DrawAllSizeOZDR(513, h_solo, stocks_back_);
		glPopMatrix();
}
void RectShelfFront_Back::SetCadToShelf(std::pair<Floor , std::vector<Triangle3D*>> &body) {
	if (selected_floor_ != -1) {
	  if (direct_shelf_ == FONT) {
			stocks_font_.at(selected_floor_).first.cad_pos.x_pos = (int)width_/body.first.floor_size.x_size;
			stocks_font_.at(selected_floor_).first.cad_pos.y_pos = (int)length_/body.first.floor_size.y_size;
			stocks_font_.at(selected_floor_).first.floor_size = body.first.floor_size;
			stocks_font_.at(selected_floor_).second = body.second;
			//stocks_.assign
		} else if (direct_shelf_ == BACK) {
			stocks_back_.at(selected_floor_).first.cad_pos.x_pos = (int)width_/body.first.floor_size.x_size;
			stocks_back_.at(selected_floor_).first.cad_pos.y_pos = (int)length_/body.first.floor_size.y_size;
			stocks_back_.at(selected_floor_).first.floor_size = body.first.floor_size;
			stocks_back_.at(selected_floor_).second = body.second;
		}
	}
}
void RectShelfFront_Back::GetShelfPosition(Vector3D &p_origin) {
	p_origin = bbmin_back_+ bbmax_font_;
	p_origin = p_origin*0.5;
	p_origin.v[2] = 0;

}
void RectShelfFront_Back::SetShelfPosition(Vector3D &p_move) {
	Vector3D temp(width_, length_, height_);
	bbmin_back_ = bbmin_back_ + p_move;
	Vector3D temp1;
	if (shelf_direction_ == FONT_BACK) {
		temp1.Set(width_, 0 , 0);
		temp.Set(width_, length_, height_);
		bbmax_back_ = bbmin_back_ + temp;
	  bbmin_font_ = bbmin_back_ + temp1;
	  bbmax_font_ = bbmin_font_ + temp;
	} else {
		temp1.Set(0,  width_, 0);
		temp.Set(length_, width_, height_);
		bbmax_back_ = bbmin_back_ + temp;
	  bbmin_font_ = bbmin_back_ + temp1;
	  bbmax_font_ = bbmin_font_ + temp;
	}
}
void RectShelfFront_Back::PointMouseOnFloor(Vector3D &dir, Vector3D &pos) {
	if (direct_shelf_ == FLOOR_FONT) {
		selected_floor_ = FindPointMouseOnFloor(dir, pos, bbmin_font_, bbmax_font_, height_ /12.0, stocks_font_);
	} else {
	  selected_floor_ = FindPointMouseOnFloor(dir, pos, bbmin_back_, bbmax_back_, height_ /12.0, stocks_back_);
	}
}
void RectShelfFront_Back::ReSetSelectFloor() {
	selected_floor_ = - 1;
}

void RectShelfFront_Back::RotateShelf() {
	Vector3D temp;
	GetShelfPosition(temp);
	if(shelf_direction_ == FONT_BACK) {
		shelf_direction_ = LEFT_RIGHT;
		Vector3D p(-length_/2.0, -width_, 0);
		bbmin_back_ = p + temp;
	} else {
		shelf_direction_ = FONT_BACK;
		Vector3D p(-width_, -length_/2.0, 0);
		bbmin_back_ = p + temp;
	}
	Vector3D u(0, 0, 0);
	SetShelfPosition(u);
}

void RectShelfFront_Back::SetHeightFloor(int selected_count, double height_first, double height_second) {
	if(height_first < 16 || height_second < 16) {
		return;
	}
	if(selected_count < stocks_font_.size() && selected_count >= 0) {
		if(type_ == FLOOR_FONT) {
			stocks_font_.at(selected_count).first.height_floor = height_first;
			if(selected_count < stocks_font_.size() - 1) {
				stocks_font_.at(selected_count + 1).first.height_floor = height_second;
			}
		} else {
			stocks_back_.at(selected_count).first.height_floor = height_first;
			if(selected_count < stocks_back_.size() - 1) {
				stocks_back_.at(selected_count + 1).first.height_floor = height_second;
			}
		}
	}
}

void RectShelfFront_Back::GetHeightFloor(Vector3D &dir,
	                                       Vector3D &pos,
																				 int &selected_count,
																				 double &height_first,
																				 double &height_second) {
	if (type_ == FLOOR_FONT) {
		selected_count = FindPointMouseOnFloor(dir,
			                                      pos, 
																						bbmin_font_,
																						bbmax_font_, 
																						height_ /12.0, 
																						stocks_font_);
		 if(selected_count != - 1) {
			height_first = stocks_font_.at(selected_count).first.height_floor;
			if(selected_count< stocks_font_.size() - 1) {
				height_second = stocks_font_.at(selected_count + 1).first.height_floor;
			}
		} 
	} else {
	  selected_count = FindPointMouseOnFloor(dir,
			                                      pos,
																						bbmin_back_,
																						bbmax_back_, 
																						height_ /12.0,
																						stocks_back_);
		 if(selected_count != - 1) {
			height_first = stocks_back_.at(selected_count).first.height_floor;
			if(selected_count< stocks_back_.size() - 1) {
				height_second = stocks_back_.at(selected_count + 1).first.height_floor;
			}
		} 
	}
}