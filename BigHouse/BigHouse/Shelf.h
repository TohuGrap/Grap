
/*****************************************************************************
* Class Name		: Shelf
* Purpose				: Class about create Shelf
* Language			: Visual C++ 2010 and OpenGL 
* OS						: Windows 7
* CopyRight(C)	: ToHoGrap
******************************************************************************/

#ifndef SHELF_H_
#define SHELF_H_

#include "Vector3D.h"
#include "Triangle3D.h"
#include "BigHouse.h"
#include "Struct.h"
class Shelf {
public:
  Shelf();
	~Shelf();

	virtual bool IsLineCutBody(const Vector3D &dir, const Vector3D& pos, Vector3D &p) = 0;
	virtual void DrawShelf() = 0;
	virtual void SetCadToShelf(std::pair<Floor , std::vector<Triangle3D*>> &body) = 0;
	virtual void GetOriginBody(Vector3D &p_origin) = 0;
	virtual void SetOriginBody(Vector3D &p_move) = 0;
	virtual void PointMouseOnFloor(Vector3D &dir, Vector3D &pos) = 0;
	virtual void ReSetSelectFloor() = 0;
public:
	bool IsCircleShelf() {return is_circle_shelf_;}
protected:
	bool is_circle_shelf_;
	void DrawFaceShelf(Rect &rect);
	void DrawCylinder(double r, double h, double sp, double ep,
										double angle, bool color_blue = false);
};

#endif // SHELF_H_