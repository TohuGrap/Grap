#include "stdafx.h"
#include "RecShelfFont_Back.h"
RectShelfFront_Back::RectShelfFront_Back(float width,
	                                   float length,
																		 float height, 
																		 float dis_drag,
																		 UINT floor_count) :
	height_(height),
	length_(length),
	width_(width),
	dis_drag_(dis_drag),
	shelf_direction_(DirectionShelf::FONT_BACK) {
	int h = int((height - height/15.0) / dis_drag);
	h = int(h/floor_count);
	float d = h*dis_drag;
	//std::pair<Floor, std::vector<Triangle3D*>> stock;
	Floor floor;
	for(int i = 0; i < floor_count; i++) {
		floor.height_floor = d;
		all_floor_font_.push_back(floor);
		all_floor_back_.push_back(floor);
	}

	bbmin_back_.Set(0, 0, 0);
  selected_floor_ = -1;
	type_ = FLOOR_BACK;
}

RectShelfFront_Back::~RectShelfFront_Back() {

}

bool RectShelfFront_Back::ObjectIsSelectedByLeftMouse(const Vector3D &dir,
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
	if(shelf_direction_ != FONT_BACK) {
		glRotatef(90, 0, 0, 1);
	}
		glTranslatef(0, -length_/2.0, 0);
		int selected_floor;
		if(type_ == FLOOR_FONT) {
			selected_floor = selected_floor_;
		} else {
			selected_floor = -1;
		}
		ShelfStructure(width_ ,
			             length_, 
									 height_,
									 h_solo, 
									 selected_floor,
									 all_floor_font_/*all_floor_font_*/);
		//DrawCommodity(all_floor_font_,  h_solo);
		DrawCommodityFromDownToUp(all_floor_font_, List_commodity_font_, length_, width_, h_solo, 4*dis_drag_, 10);
		if(GetKeyState(VK_SHIFT) & 0x8000) {
			DrawAllSizeOZDR(513, h_solo, all_floor_font_);
		}
		// draw ke thu 2
		glPushMatrix();
		glRotatef(180, 0, 0, 1);
		glTranslatef(0, - length_, 0 );
		if(type_ == FLOOR_BACK) {
			selected_floor = selected_floor_;
		} else {
			selected_floor = -1;
		}
		ShelfStructure(width_ ,
			             length_,
									 height_,
									 h_solo,
									 selected_floor,
									 all_floor_back_,
									 false);
		DrawCommodityFromDownToUp(all_floor_back_, List_commodity_back_, length_, width_, h_solo, 4*dis_drag_, 10);
		if(GetKeyState(VK_SHIFT) & 0x8000) {
		  DrawAllSizeOZDR(513, h_solo, all_floor_back_);
		}
		glPopMatrix();
}
void RectShelfFront_Back::SetCadToShelf(std::pair<Floor , std::vector<Triangle3D*>> &body) {
	//if (selected_floor_ != -1) {
	//  if (type_ == FLOOR_FONT) {
	//		if(selected_floor_< all_floor_font_.size()- 1) {
	//			if(all_floor_font_.at(selected_floor_ + 1).first.height_floor < body.first.floor_size.z_size) {
	//				AfxMessageBox(L"Chiều cao kệ không đủ");
	//				return;
	//			}
	//		}
	//		all_floor_font_.at(selected_floor_).first.cad_pos.x_pos = (int)width_/body.first.floor_size.x_size;
	//		all_floor_font_.at(selected_floor_).first.cad_pos.y_pos = (int)length_/body.first.floor_size.y_size;
	//		all_floor_font_.at(selected_floor_).first.floor_size = body.first.floor_size;
	//		all_floor_font_.at(selected_floor_).second = body.second;
	//		//stocks_.assign
	//	} else if (type_ == FLOOR_BACK) {
	//		if(selected_floor_< all_floor_back_.size()- 1) {
	//			if(all_floor_back_.at(selected_floor_ + 1).first.height_floor < body.first.floor_size.z_size) {
	//				AfxMessageBox(L"Chiều cao kệ không đủ");
	//				return;
	//			}
	//		}
	//		all_floor_back_.at(selected_floor_).first.cad_pos.x_pos = (int)width_/body.first.floor_size.x_size;
	//		all_floor_back_.at(selected_floor_).first.cad_pos.y_pos = (int)length_/body.first.floor_size.y_size;
	//		all_floor_back_.at(selected_floor_).first.floor_size = body.first.floor_size;
	//		all_floor_back_.at(selected_floor_).second = body.second;
	//	}
	//}
}

void RectShelfFront_Back::SetCommodity(std::vector<Commodity*> list_commodity) {
	if (selected_floor_ != -1) {
	  if (type_ == FLOOR_FONT) {
			List_commodity_font_ = list_commodity;
		} else if (type_ == FLOOR_BACK) {
			List_commodity_back_ = list_commodity;
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
	if (type_ == FLOOR_FONT) {
		selected_floor_ = FindPointMouseOnFloor(dir, pos, bbmin_font_, bbmax_font_, height_ /12.0,all_floor_font_ /*all_floor_font_*/);
	} else {
		selected_floor_ = FindPointMouseOnFloor(dir, pos, bbmin_back_, bbmax_back_, height_ /12.0, all_floor_back_/*all_floor_back_*/);
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
	if(height_first < 4*dis_drag_) {
		return;
	}
	if(selected_count >= 0) {
		if(selected_count < all_floor_font_.size() && type_ == FLOOR_FONT) {
			if(selected_count < all_floor_font_.size() - 1) {
				if(height_second < 4*dis_drag_)
					return;
			}
			if( selected_count == all_floor_font_.size() - 1) {
				double d = height_/12.0;
				for(int i = 1; i < all_floor_font_.size() - 1; i ++) {
					d += all_floor_font_.at(i).height_floor;
				}
				if(d + height_first > height_) {
					int h = (height_ - d)/dis_drag_;
					height_first = h*dis_drag_;
				}
				height_second = height_ - d - height_first;
			}
			assert(height_first > 0);
			if(selected_count < all_floor_font_.size() && selected_count > 0) {
				if(height_first  < all_floor_font_.at(selected_count -1).floor_size.z_size + 2) {
					return;
				}
				if(selected_count < all_floor_font_.size() - 1) {
					if(height_second < all_floor_font_.at(selected_count).floor_size.z_size + 2) {
						return;
					}
					all_floor_font_.at(selected_count + 1).height_floor = height_second;
				} else {
					all_floor_font_.at(0).height_floor = height_second;
				}
				all_floor_font_.at(selected_count).height_floor = height_first;
			}
		} else if(selected_count < all_floor_back_.size()) {
			if(selected_count < all_floor_back_.size() - 1) {
				if(height_second < 4*dis_drag_)
					return;
			}
			if( selected_count == all_floor_back_.size() - 1) {
				double d = height_/12.0;
				for(int i = 1; i < all_floor_back_.size() - 1; i ++) {
					d += all_floor_back_.at(i).height_floor;
				}
				if(d + height_first > height_) {
					int h = (height_ - d)/dis_drag_;
					height_first = h*dis_drag_;
				}
				height_second = height_ - d - height_first;

			}

			assert(height_first > 0);
			if(selected_count < all_floor_back_.size() && selected_count > 0) {
				if(height_first  < all_floor_back_.at(selected_count -1).floor_size.z_size + 2) {
					return;
				}
				if(selected_count < all_floor_back_.size() - 1) {
					if(height_second < all_floor_back_.at(selected_count).floor_size.z_size + 2) {
						return;
					}
					all_floor_back_.at(selected_count + 1).height_floor = height_second;
				} else {
					all_floor_back_.at(0).height_floor = height_second;
				}
				all_floor_back_.at(selected_count).height_floor = height_first;
			}
		}
	}
}

void RectShelfFront_Back::GetHeightFloor(Vector3D &dir,
	                                       Vector3D &pos,
																				 int &selected_count,
																				 double &height_first,
																				 double &height_second,
																				 float &dis_drag) {
	if (type_ == FLOOR_FONT) {
		selected_count = FindPointMouseOnFloor(dir,
			                                      pos, 
																						bbmin_font_,
																						bbmax_font_, 
																						height_ /12.0, 
																						all_floor_font_/*all_floor_font_*/);
		 if(selected_count != - 1) {
			height_first = all_floor_font_.at(selected_count).height_floor;
			if(selected_count< all_floor_font_.size() - 1) {
				height_second = all_floor_font_.at(selected_count + 1).height_floor;
			}
		} 
	} else {
	  selected_count = FindPointMouseOnFloor(dir,
			                                      pos,
																						bbmin_back_,
																						bbmax_back_, 
																						height_ /12.0,
																						all_floor_back_/*all_floor_back_*/);
		 if(selected_count != - 1) {
			height_first = all_floor_back_.at(selected_count).height_floor;
			if(selected_count< all_floor_back_.size() - 1) {
				height_second = all_floor_back_.at(selected_count + 1).height_floor;
			}
		} 
	}
	dis_drag = dis_drag_;
}