
/*****************************************************************************
* Class Name		: CircleShelf
* Purpose				: Make Circle Shelf
* Language			: Visual C++ 2010 and OpenGL 
* OS						: Windows 7
* CopyRight(C)	: ToHoGrap
******************************************************************************/

#ifndef CIRCLE_SHELF_H_
#define CIRCLE_SHELF_H_

#include"Shelf.h"
#include "Struct.h"
class CircleShelf: public Shelf {
public:
	CircleShelf(double r, double h, double sp, double ep, double flat_angle, UINT floor_count);
  ~CircleShelf();

	virtual bool ObjectIsSelectedByLeftMouse(const Vector3D &dir, const Vector3D& pos, Vector3D &p);
	virtual void DrawShelf();
	virtual void SetCadToShelf(std::pair<Floor , std::vector<Triangle3D*>> &body);
	virtual void GetShelfPosition(Vector3D &p_origin);
	virtual void SetShelfPosition(Vector3D &p_move);
	virtual void PointMouseOnFloor(Vector3D &dir, Vector3D &pos);
	virtual void ReSetSelectFloor();
	virtual void RotateShelf() {};
	virtual void SetHeightFloor(int selected_count, double height_first, double height_second);
	virtual void GetHeightFloor(Vector3D &dir, Vector3D &pos, int &selected_count, double &height_first, double &height_second);


private:
  void CircleShelfFrame(double r, double h, double sp, double ep, double angle);
	void DrawFloor(double r, double h, double sp, double ep, double angle, double height_solo);
	void DrawCommodity(std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks,
		                 double h_solo, 
										 double radius);
	bool IsCadInCircle(Vector3D &o_floor, FloorSize & rect, double radius);
	float radius_;
	float height_;
  float height_floor_;
	UINT  floor_count_;
	float start_angle_;
	float end_angle_;
	float flat_angle_;
	double smooth_angle_;
	Vector3D Origin_;
	Vector3D max_origin_;
	float fcounts;
	std::vector<std::pair<Floor, std::vector<Triangle3D*>>> stocks_;

};

#endif // CIRCLE_SHELF_H_