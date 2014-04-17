#ifndef STRUCT_H_
#define STRUCT_H_
#include "Vector3D.h"
struct RoomSize {
  float longs;
	float width;
	float height;
	float depth;
};

enum TypeCommodity {
	COMMODITY = 0,
	CO_CONTAINER = 1
};

	enum TypeArrange {
		SEQUENTIAL = 0,
		FLOOR
	};
	enum ShelfType {
		SIMPLE_SHELF = 0,
		DOUBLE_SHELF = 1,
		CIRCLE_SHELF = 2,
		CONTAINER = 3
	};

struct ShelfInfo {
  // Get info from fomrview to view
  float longs;
  float height;
  float width;

  int numf;
	float shelf_radius;
	float dis_drag;
	float dis_wall;
	CString name_project;
	ShelfType shelf_type;
	TypeArrange type_arrange;
};

	struct Rect {
		Vector3D n;
		Vector3D p[4];
	};

	struct CadInfo {
	CString name_production;
	float lenght;
	float width;
	float height;
	float weight;
	int floor;
	int count;
	TypeCommodity type_commodity;
	};

#endif // STRUCT_H_