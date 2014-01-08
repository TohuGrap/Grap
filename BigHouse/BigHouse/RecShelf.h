
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
	RectShelf(float width, float length, float height, float dis_drag, UINT floor_count);
	RectShelf();
	~RectShelf();
	void DrawFaceShelf(Rect &rec);
	void DrawCube(double width, double length, double height);

	void ShelfStructure(double width,
		                  double length,
											double height,
											double height_solo,
											int selected_floor,
											std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks,
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
												 std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks);
	
	//void DrawPoint();
	void DrawCommodity(std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks, double h_solo);
	//*******&&**********
	bool LineCutBoundingBox(const Vector3D &dir,
		                      const Vector3D &pos,
													Vector3D &bbmin,
													Vector3D &bbmax,
													Vector3D &p_on_bb);
	virtual bool ObjectIsSelectedByLeftMouse(const Vector3D &dir, const Vector3D& pos, Vector3D &p);
	virtual void DrawShelf();
	virtual void SetCadToShelf(std::pair<Floor , std::vector<Triangle3D*>> &body);
	virtual void GetShelfPosition(Vector3D &p_origin);
	virtual void SetShelfPosition(Vector3D &p_move);
	virtual void PointMouseOnFloor(Vector3D &dir, Vector3D &pos);
	virtual void ReSetSelectFloor() {selected_floor_ = -1;}
	virtual void RotateShelf();
	virtual void SetHeightFloor(int selected_count, double height_first, double height_second);
	virtual void GetHeightFloor(Vector3D &dir,
		                          Vector3D &pos,
															int &selected_count, 
															double &height_first, 
															double &height_second,
															float &dis_drag);
	
protected:
	int FindPointMouseOnFloor(Vector3D &dir,
		                         Vector3D &pos, 
														 Vector3D &bbmin,
														 Vector3D &bbmax,
														 double height_base,
														 std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks);
	void DrawSnare(double r,
		             double h,
								 double sp,
								 double ep,
								 double angle,
								 double lenght);
	void DrawAllSizeOZDR(DWORD TextList3D,
										double height_base,
								  	std::vector<std::pair<Floor, std::vector<Triangle3D*>>> &stocks) ;

private:
  int length_;
	int height_;
	int width_;
	int selected_floor_;
	float dis_drag_;

	TypeRecShelf type_;
	std::vector<std::pair<Floor, std::vector<Triangle3D*>>> stocks_;
	Vector3D bbmin_;
	Vector3D bbmax_;
	Vector3D point_center_;
};

#endif // RECT_SHELF_H_
