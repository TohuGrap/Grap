
/*****************************************************************************
* Class Name		: RectShelf
* Purpose				: Make Simple Shelf
* Language			: Visual C++ 2010 and OpenGL 
* OS						: Windows 7
* CopyRight(C)	: ToHoGrap
******************************************************************************/

#ifndef RECT_SHELF_H_
#define RECT_SHELF_H_

#include"Shelf.h"
#include "Struct.h"

class RectShelf: public Shelf {
public:
	enum TypeRecShelf {
		FONT = 0,
		BACK,
		LEFT,
		RIGHT
	};
public :
	RectShelf(float width, float length, float height, UINT floor_count , TypeRecShelf diection);
	RectShelf();
	~RectShelf();
	void DrawFaceShelf(Rect &rec);
	void DrawCube(double width, double length, double height);

	void ShelfStructure(double width,
		                  double length,
											double height,
											int count_floor,
											double height_floor,
											double height_solo,
											TypeRecShelf type,
											bool draw_snare = true);
	void DrawShelfFloor(int width, 
		                  int length,
											int heigth ,
											double height_solo,
											int count,
											std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks);
	void GetBoundingBox(Vector3D &bbmin, Vector3D &bbmax) const;
	void GetBBmin(Vector3D &bbmin) const;
  void SetBoundingBox(Vector3D &bbmin /*Vector3D &bbmax*/);
	bool LineCutSurface(Vector3D &dir,
	                    Vector3D &pos,
											Vector3D &n,
											Vector3D &A,// min body
											Vector3D &B, // max body
											Vector3D &E); // point axis cut on face
	void DrawFloorHande(double width, double height, double length, double t , bool draw_bar);
	void DrawTwoHandeFloor(double width,
		                     double length,
												 double height,
												 double height_solo,
												 double t,
												 int count_floor, 
												 double height_floor);
	
	//void DrawPoint();
	void DrawCommodity(std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks, double h_solo);
	//*******&&**********
	bool LineCutBoundingBox(const Vector3D &dir,
		                      const Vector3D &pos,
													Vector3D &bbmin,
													Vector3D &bbmax,
													Vector3D &p_on_bb);
	virtual bool IsLineCutBody(const Vector3D &dir, const Vector3D& pos, Vector3D &p);
	virtual void DrawShelf();
	virtual void SetCadToShelf(std::pair<Floor , std::vector<Triangle3D*>> &body);
	virtual void GetOriginBody(Vector3D &p_origin);
	virtual void SetOriginBody(Vector3D &p_move);
	virtual void PointMouseOnFloor(Vector3D &dir, Vector3D &pos);
	virtual void ReSetSelectFloor() {floor_count_ = -1;}
	
protected:
	int FindPointMouseOnFloor(Vector3D &dir,
		                         Vector3D &pos, 
														 Vector3D &bbmin,
														 Vector3D &bbmax,
														 std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks);
	void DrawSnare(double r,
		             double h,
								 double sp,
								 double ep,
								 double angle,
								 double lenght);

private:
  int length_;
	int height_;
	int width_;
	double floor_height_;
	int floor_count_;
	TypeRecShelf type_;
	std::vector<std::pair<Floor, std::vector<Triangle3D*>>> stocks_;
	Vector3D bbmin_;
	Vector3D bbmax_;
};

#endif // RECT_SHELF_H_
