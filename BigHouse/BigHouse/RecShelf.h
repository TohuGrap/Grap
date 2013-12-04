#pragma once
#include"Shelf.h"
#include "Struct.h"

enum TypeRecShelf {
	FONT = 0,
	BACK,
	LEFT,
	RIGHT
};

class RecShelf: public Shelf {
public :
	RecShelf(int width, int length, int height, int count_floor , TypeRecShelf type);
	RecShelf();
	~RecShelf();
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
	virtual void ReSetSelectFloor() {count_floor_ = -1;}
	
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
	double height_floor_;
	int count_floor_;
	TypeRecShelf type_;
	std::vector<std::pair<Floor, std::vector<Triangle3D*>>> stocks_;
	Vector3D bbmin_;
	Vector3D bbmax_;

	////////
};