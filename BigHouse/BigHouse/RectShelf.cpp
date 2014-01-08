
#include "stdafx.h"
#include"RecShelf.h"
#include "Struct.h"

RectShelf::RectShelf(float width,
	                  float length,
									  float height,
										float dis_drag,
									  UINT floor_count):
	height_(height),
	length_(length),
	width_(width),
	dis_drag_(dis_drag),
  type_(TypeRecShelf::FONT) {
  if (floor_count <= 0) {
	  MessageBox(NULL, _T("Số tầng của kệ là 0 \nĐiều này không hợp lệ \nLấy giá trị mặc đinh là 5"), _T("Thông báo"), MB_OK|MB_ICONWARNING);
		floor_count = 5;
	}
	//floor_height_ = (int)(height- height/20.0)/(floor_count);
 //// int h = (int)(height- height/20.0)/(floor_count);
	////h = h/3;
	////h = h*3;
	int h = int((height - height/20.0) / dis_drag);
	h = int(h/floor_count);
	float d = h*dis_drag;
	std::pair<Floor, std::vector<Triangle3D*>> stock;
	for(int i = 0; i < floor_count; i ++) {
		stock.first.height_floor = d;
		stocks_.push_back(stock);
	}
	bbmin_.Set(0, 0, 0);
	bbmax_.Set(width_, length_, height_);
	selected_floor_ = - 1;
	//is_circle_shelf_ = false;
}

RectShelf::RectShelf() {

}

RectShelf::~RectShelf() {

}
void RectShelf::DrawFaceShelf(Rect &rec) {
	glBegin(GL_POLYGON);
  	glNormal3fv(rec.n.v);
	  glVertex3fv(rec.p[0].v);
		glVertex3fv(rec.p[1].v);
		glVertex3fv(rec.p[2].v);
		glVertex3fv(rec.p[3].v);
	glEnd();
}

void RectShelf::DrawCube(double width, double length, double height) {
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

void RectShelf::ShelfStructure(double width,
															double length,
															double height,
															double height_solo,
															int selected_floor,
															std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks,
															bool draw_snare) {
  int size_col = (int)(width/20.0);
	if(size_col < 3) {
		size_col = 3;
	}
	glPushMatrix();
		glColor3f(1, 0, 0);
		glPushMatrix();
			glTranslatef(- 1, - size_col, 0);
			DrawCube(width + 2*size_col, length + 2*size_col, height_solo); // de
			DrawCube(size_col, size_col, height);
			glTranslated(0, length + size_col, 0); // dir oy
			DrawCube(size_col, size_col, height);
			glTranslatef(0, -length , height- 2*size_col);
			DrawCube(size_col, length, 2*size_col);
		glPopMatrix();

		if(draw_snare) {
			DrawSnare(size_col/10.0,height, 0, 360, 20, length);	
			glPushMatrix();
			glTranslatef(0, length, 0);
			glRotatef(90, 1, 0, 0);
			DrawSnare(size_col/10.0,length, 0, 360, 20, height -1);	
			glPopMatrix();
		}

		DrawTwoHandeFloor(width, length, width/8.0, height_solo, size_col/2.0, stocks);
		DrawShelfFloor(width, length, 1, height_solo, selected_floor, stocks);
	glPopMatrix();
}

void RectShelf::DrawShelfFloor(int width,
	                            int length, 
															int heigth,
															double height_solo,
															int count,
															std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks) {
	if(stocks.empty()) {
		return;
	}
	glPushMatrix();
	glTranslated(0, 0, height_solo);
	if(count == 0) {
		glColor3f(0, 0, 1);
		DrawCube(width, length, heigth);
	}

	//DrawSizeOZ(stocks.at(0).first.height_floor, 513);
	for(int i = 1; i < stocks.size(); i ++) {
		glTranslated(0, 0, stocks.at(i).first.height_floor);
		if(count == i /*&& stocks.at(i).second.empty()*/) {
			glColor3f(0, 0, 1);
		} else {
		  glColor3f(1, 1, 0);
		}
	  DrawCube(width, length, heigth);
		//if(i < stocks.size() - 1) {
	 // 	DrawSizeOZ(stocks.at(i).first.height_floor, 513);
		//} else if(i == stocks.size() - 1) {
		//	DrawSizeOZ(stocks.at(i).first.height_floor, 513, true);
		//}
	}
	glPopMatrix();
}


void RectShelf::DrawShelf() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	double h_solo = height_/12.0;
  glDisable(GL_CULL_FACE);
	glPushMatrix();
	if(type_ == BACK) {
		glRotated(180, 0, 0, 1);
	} else if(type_ == LEFT) {
		glRotated(- 90, 0, 0, 1);
	} else if(type_ == RIGHT) {
		glRotated(90, 0, 0, 1);
	}
	glTranslatef(- width_/2.0, - length_/2.0, 0);
	ShelfStructure(width_ ,
		             length_, 
								 height_ , 
								 h_solo, 
								 selected_floor_,
								 stocks_);
 	DrawCommodity(stocks_, h_solo);
	if(GetKeyState(VK_SHIFT) & 0x8000) {
  	DrawAllSizeOZ(513, h_solo,0, stocks_);
	}
	glPopMatrix();
}

void RectShelf::GetBoundingBox(Vector3D &bbmin, Vector3D &bbmax) const{
	bbmin = bbmin_;
	bbmax = bbmax_;
}

void RectShelf::SetBoundingBox(Vector3D &bbmin){
	bbmin_ = bbmin;
	Vector3D p(width_, length_, height_);
	bbmax_ = bbmin + p;
}

void RectShelf::GetBBmin(Vector3D &bbmin) const {
	bbmin = bbmin_;
}

int RectShelf::FindPointMouseOnFloor(Vector3D &dir,
	                                   Vector3D &pos,
																		  Vector3D &bbmin,
														          Vector3D &bbmax,
																			double height_base,
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
	double height = height_base;
	bool has_point = false;
	Vector3D point_floor;
	for(int i = 0; i < stocks.size(); i ++) {
		if(i != 0) {
			height += stocks.at(i).first.height_floor;
		}
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
		//if(i != 0) {
		//	height  += stocks.at(i).first.height_floor;
		//}
	}
	return count;
}


void RectShelf::PointMouseOnFloor(Vector3D &dir, Vector3D &pos) {
	selected_floor_ = FindPointMouseOnFloor(dir, pos, bbmin_, bbmax_, height_/12.0,stocks_);
}

void RectShelf::DrawCommodity(std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks, double h_solo) {
	glPushMatrix();
	glColor3f(0, 0, 1);
	glShadeModel(GL_SMOOTH);
	for(int i = 0 ; i < stocks.size(); i ++) {
		if(i ==  0) {
      glTranslated(0, 0, h_solo);
		} else {
		  glTranslated(0, 0, stocks.at(i).first.height_floor); // trans z
		}
	  glPushMatrix();
		//glTranslatef(stocks.at(i).first.o_floor.v[0],
		//	           stocks.at(i).first.o_floor.v[1], 
		//						 stocks.at(i).first.o_floor.v[2]); // trab to origin - floor
		if(!stocks.at(i).second.empty()) {
			for(int j = 0; j < stocks.at(i).first.cad_pos.x_pos; j++) {
				if(j != 0)
				  glTranslated(stocks.at(i).first.floor_size.x_size,0,0); // trans x
				glPushMatrix();
				for(int k = 0 ; k < stocks.at(i).first.cad_pos.y_pos; k++) {
					if(k != 0)
					  glTranslated(0, stocks.at(i).first.floor_size.y_size, 0); //trans y
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

bool RectShelf::LineCutSurface(Vector3D &dir,
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

void RectShelf::SetCadToShelf(std::pair<Floor, std::vector<Triangle3D*>> &body) {
	if(selected_floor_ != -1) {
		if(selected_floor_< stocks_.size()- 1) {
			if(stocks_.at(selected_floor_ + 1).first.height_floor < body.first.floor_size.z_size) {
				AfxMessageBox(L"Chiều cao kệ không đủ");
				return;
			}
	  }
		stocks_.at(selected_floor_).first.cad_pos.x_pos = (int)width_/body.first.floor_size.x_size;
		stocks_.at(selected_floor_).first.cad_pos.y_pos = (int)length_/body.first.floor_size.y_size;
		stocks_.at(selected_floor_).first.floor_size = body.first.floor_size;
		stocks_.at(selected_floor_).second = body.second;
		//stocks_.assign
	}
}

//**********************************&&*************************

bool RectShelf::LineCutBoundingBox(const Vector3D &dir,
	                                const Vector3D &pos,
																	Vector3D &bbmin,
																	Vector3D &bbmax,
																	Vector3D &p_on_bb) {
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

void RectShelf::DrawTwoHandeFloor(double width,
																 double length,
																 double height,
																 double height_solo,
																 double t,
																 std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks){
	glPushMatrix();
	glTranslatef(0, - t, height_solo);
	for(int i = 1; i < stocks.size(); i ++){
		glTranslatef(0, 0, stocks.at(i).first.height_floor);
		DrawFloorHande(width,length, height, t , true);
		glPushMatrix();
		glTranslatef(0, length + t, 0);
		DrawFloorHande(width,length, height, t, false);
		glPopMatrix();
	}
	glPopMatrix();
}

void RectShelf::DrawFloorHande(double width, double length , double height, double t , bool draw_bar) {
	Vector3D A[5];
	A[0].Set(0, 0, 0);
	A[1].Set(width, 0, 0);
  A[2].Set(width + 1, 0, - height/5);
	A[3].Set(width/3, 0,-height/3);
  A[4].Set(0, 0,-height);

	Vector3D B[5];
	Vector3D n(0, t, 0);
	for(int i = 0; i < 5; i ++) {
	  B[i] = A[i] + n;
	}
	glColor3f(0 , 0, 1);
	glBegin(GL_POLYGON);
	glNormal3f(0, -1, 0);
	glVertex3fv(A[0].v);
	glVertex3fv(A[1].v);
	glVertex3fv(A[2].v);
	glVertex3fv(A[3].v);
	glVertex3fv(A[4].v);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glVertex3fv(B[0].v);
	glVertex3fv(B[1].v);
	glVertex3fv(B[2].v);
	glVertex3fv(B[3].v);
	glVertex3fv(B[4].v);
	glEnd();

	Vector3D temp1 = A[0];
	Vector3D temp2 = B[0];
	std::vector<Rect> rect;
	for(int i = 0 ; i < 5; i ++) {
		Vector3D v = temp1 - temp2;//=
		Vector3D u = A[i] - temp1;
		Vector3D nor = v*u;
		nor = nor.Unit();
		glBegin(GL_POLYGON);
		glNormal3fv(nor.v);
		glVertex3fv(temp1.v);
		glVertex3fv(A[i].v);
		glVertex3fv(B[i].v);
		glVertex3fv(temp2.v);
		glEnd();
		temp1 = A[i];
		temp2 = B[i];
	}

	if(draw_bar) {
	  Vector3D v = A[1] - B[1];
		Vector3D u = A[2] - A[1];
		Vector3D nor = v*u;
		nor = nor.Unit();
		Vector3D P1(B[1]);
		Vector3D P2(B[2]);
		P1.v[1] = P1.v[1] + length;
		P2.v[1] = P2.v[1] + length;
		glBegin(GL_POLYGON);
		glNormal3fv(nor.v);
		glVertex3fv(A[1].v);
		glVertex3fv(A[2].v);
		glVertex3fv(P2.v);
		glVertex3fv(P1.v);
		glEnd();
	}

}


bool RectShelf::ObjectIsSelectedByLeftMouse(const Vector3D &dir, const Vector3D& pos, Vector3D &p) {
	return LineCutBoundingBox(dir, pos, bbmin_, bbmax_, p);
}

void RectShelf::GetShelfPosition(Vector3D &p_origin) {
	p_origin = bbmin_ + bbmax_;
	p_origin = p_origin*0.5;
	p_origin.v[2] = 0.0;
}

void RectShelf::SetShelfPosition(Vector3D &p_move) {
	bbmin_ = bbmin_ + p_move;
	if(type_ == FONT || type_ == BACK) {
		Vector3D p(width_, length_, height_);
	} else {
		Vector3D p(length_, width_, height_);
	}
  Vector3D p(width_, length_, height_);
	bbmax_ = bbmin_ + p;

}


void RectShelf::DrawSnare(double r, 
	                       double h, 
												 double sp,
												 double ep,
												 double angle,
												 double lenght) {
	int size = (int)lenght/ 10.0;
	float temp = lenght/size;
	glPushMatrix();
	for(int i = 0; i < size; i++) {
	  glTranslatef(0, temp ,0);
	  DrawCylinder(r, h, sp, ep, angle);
	}
	glPopMatrix();
}

void RectShelf::RotateShelf() {
	Vector3D center = bbmin_ + bbmax_;
	center = center*0.5;
	center.v[2] = 0.0;
	if(type_ == FONT) {
		type_ = LEFT;
	} else if (type_ == LEFT) {
		type_ = BACK;
	} else if(type_ == BACK) {
		type_ = RIGHT;
	} else {
		type_ = FONT;
	}

	if(type_ == BACK || type_ == FONT) {
		Vector3D P(-length_/2.0, - width_/2.0, 0);
		bbmin_ = center + P;
		Vector3D p_m(length_, width_, height_);
		bbmax_ = bbmin_ + p_m;
	} else {
			Vector3D P(- width_/2.0, - length_/2.0, 0);
		bbmin_ = center + P;
		Vector3D p_m(width_, length_, height_);
		bbmax_ = bbmin_ + p_m;
	}
}

void RectShelf::SetHeightFloor(int selected_count, double height_first, double height_second) {
	if(selected_count < stocks_.size() - 1) {
		if(height_second < 4*dis_drag_)
			return;
	}
	if(height_first < 4*dis_drag_) {
		return;
	}
	if( selected_count == stocks_.size() - 1) {
		double d = height_/12.0;
		for(int i = 1; i < stocks_.size() - 1; i ++) {
			d += stocks_.at(i).first.height_floor;
		}
		if(d + height_first > height_) {
			int h = (height_ - d)/dis_drag_;
			height_first = h*dis_drag_;
		}
	}

	assert(height_first > 0);
	if(selected_count < stocks_.size() && selected_count > 0) {
		if(height_first  < stocks_.at(selected_count -1).first.floor_size.z_size + 2) {
			return;
		}
		if(selected_count < stocks_.size() - 1) {
			if(height_second < stocks_.at(selected_count).first.floor_size.z_size + 2) {
				return;
			}
			stocks_.at(selected_count + 1).first.height_floor = height_second;
		}
		stocks_.at(selected_count).first.height_floor = height_first;
	}
}

void RectShelf::GetHeightFloor(Vector3D &dir,
	                             Vector3D &pos,
															 int &selected_count, 
															 double &height_first,
															 double &height_second,
															 float &dis_drag) {
  selected_count = FindPointMouseOnFloor(dir, pos, bbmin_, bbmax_, height_/12.0, stocks_);
	if(selected_count != - 1) {
    height_first = stocks_.at(selected_count).first.height_floor;
		if(selected_count< stocks_.size() - 1) {
			height_second = stocks_.at(selected_count + 1).first.height_floor;
		}
	} 
	dis_drag = dis_drag_;
}

void RectShelf::DrawAllSizeOZDR(DWORD TextList3D,
												      	double height_base,
										            std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks) {
  DrawAllSizeOZ(TextList3D, height_base, 0, stocks);
}