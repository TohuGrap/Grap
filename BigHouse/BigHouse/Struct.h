#ifndef STRUCT_H_
#define STRUCT_H_
struct RoomSize {
  float longs;
	float width;
	float height;
	float depth;
};

	enum TypeArrange {
		SEQUENTIAL = 0,
		FLOOR
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
	UINT shelf_type;
	TypeArrange type_arrange;
};

	struct Rect {
		Vector3D n;
		Vector3D p[4];
	};

	struct CadInfo {
	CString name_production;
	CString type_production;
	float lenght;
	float width;
	float height;
	float weight;
	int floor;
	//bool change_proportion;
	int count;
	//
	
	};

#endif // STRUCT_H_