#ifndef STRUCT_H_
#define STRUCT_H_
struct RoomSize {
  float longs;
	float width;
	float height;
	float depth;
};


struct ShelfInfo {
  // Get info from fomrview to view
  float longs;
  float height;
  float width;
  int numf;
	float shelf_radius;
	float shelf_start_angle_;
  float shelf_end_angle_;
	float shelf_flat_angle_;
	float dis_drag;
	UINT shelf_type;
};

	struct Rect {
		Vector3D n;
		Vector3D p[4];
	};

#endif // STRUCT_H_