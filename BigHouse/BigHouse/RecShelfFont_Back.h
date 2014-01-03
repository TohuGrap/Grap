
/*****************************************************************************
* Class Name		: RectShelfFront_Back
* Purpose				: Make Double Shelf
* Language			: Visual C++ 2010 and OpenGL 
* OS						: Windows 7
* CopyRight(C)	: ToHoGrap
******************************************************************************/

#ifndef RECT_SHELF_FRONT_BACK_H_
#define RECT_SHELF_FRONT_BACK_H_

#include "Shelf.h"
#include "RecShelf.h"


class RectShelfFront_Back :public Shelf, RectShelf {
public:
	enum DirectionShelf {
		FONT_BACK = 0,
		LEFT_RIGHT = 1
	};

	enum TypeFloor {
		FLOOR_FONT = 0,
		FLOOR_BACK = 1
	};

public:
	RectShelfFront_Back(float width, float length, float height, UINT floor_count);
  ~RectShelfFront_Back();

	virtual bool ObjectIsSelectedByLeftMouse(const Vector3D &dir,
														 const Vector3D& pos,
														 Vector3D &p);
	virtual void DrawShelf();
	virtual void SetCadToShelf(std::pair<Floor , std::vector<Triangle3D*>> &body);
	virtual void GetShelfPosition(Vector3D &p_origin);
	virtual void SetShelfPosition(Vector3D &p_move);
	virtual void PointMouseOnFloor(Vector3D &dir, Vector3D &pos);
	virtual void ReSetSelectFloor();
	virtual void RotateShelf();
	virtual void SetHeightFloor(int selected_count, double height_first, double height_second);
	virtual void GetHeightFloor(Vector3D &dir, Vector3D &pos, int &selected_count, double &height_first, double &height_second);

private:
  int length_;
	int height_;
	int width_;
	int floor_height_;
	//int floor_count_;
	int selected_floor_;
	TypeFloor direct_shelf_;
	DirectionShelf shelf_direction_;
	Vector3D bbmin_font_;
	Vector3D bbmax_font_;
	Vector3D bbmin_back_;
	Vector3D bbmax_back_;
	std::vector<std::pair<Floor, std::vector<Triangle3D*>>> stocks_font_;
	std::vector<std::pair<Floor, std::vector<Triangle3D*>>> stocks_back_;
};

#endif // RECT_SHELF_FRONT_BACK_H_