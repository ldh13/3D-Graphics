#include <SDL2/SDL.h>
#include <stdbool.h>
#include "linalg.h"

// ### STRUCTS  AND ENUMS ### //
// ## STRUCTS ## //
typedef struct {
	Vec3 eye;
	Vec3 center;
	Vec3 up_direction;
} Camera;

typedef struct {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
} ColorRgb;

typedef struct {
	int x;
	int y;
} IVec2;

typedef struct {
	IVec2 pos;
	ColorRgb color;
} Pixel;

typedef struct {
	IVec2 *points;
	int count;
} Object;

typedef struct {
	Pixel *pixels;
	int count;
} ViewObject;

typedef struct {
	IVec2 *points;
	int count;
} Line;

typedef struct {
	Vec3 vertices[4];
	int edges[6][2];
} Tetrahedron;

typedef struct {
	Vec3 vertices[8];
	int edges[12][2];
} Cube;

// ## ENUMS ## //
typedef enum {
	TETRAHEDRON,
	HEXAHEDRON,
	OCTAHEDRON,
	DODECAHEDRON,
	ICOSAHEDRON,
} PlatonicSolid;


// ### FUNCTION DECLARATIONS ### //

// ## STRUCT FUNCTIONS ## //
void destroy_line(Line **line);
void destroy_lines(Line* lines, int count);

Tetrahedron create_tetrahedron(Vec3 center, float side_length);
Cube create_cube(Vec3 center, float side_length);

// ## MATRIX TRANSFORMATIONS ## //
Matrix4 gen_view_matrix(Camera *cam);
Matrix4 gen_perspective_projection_matrix();

Vec3 perspective_divide(Vec4 v);
Vec3 viewport_transform(Vec3 v);

Vec3 world_to_viewport(Camera *cam, Vec4 v);


// ## DRAWING  ALGORITHMS ## //
Line bresenham_line(IVec2 p0, IVec2 p1);

// ## DRAWING FUNCTIONS ## //
bool draw_object(Uint32 *buffer, int pitch, ViewObject* object);
bool draw_line(Uint32 *buffer, Camera *cam, Vec3 from, Vec3 to, ColorRgb Color, int pitch);
bool draw_tetrahedron(Uint32 *buffer, Camera *cam, Tetrahedron th, ColorRgb color, int pitch);
bool draw_cube(Uint32 *buffer, Camera *cam, Cube cube, ColorRgb color, int pitch);

// ## DRAWING UTILS ## //
Object *points_to_object(IVec2 *points, int count);
Pixel ivec2_to_pixel(IVec2 v, ColorRgb color);
ViewObject *object_to_view_object(Object *object, ColorRgb color);

// ## GEOMETRIC FUNCTIONS ## //
Line get_line_points(Vec3 from, Vec3 to, Camera *cam);

IVec2 *get_tetrahedron_points(Tetrahedron th, Camera *cam, int *point_count);

Line *get_cube_edges(Cube cube, Camera *cam);
IVec2 *get_cube_points(Cube cube, Camera *cam, int *point_count);