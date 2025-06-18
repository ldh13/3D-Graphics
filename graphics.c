#include <math.h>
#include <stdbool.h>
#include "graphics.h"

// ### CONSTANT DEFINITIONS ### //
const float UNIT_EQ_TRIANGLE_CIRCUMCENTER = 0.57735f;
const float UNIT_TETRAHEDRON_CIRCUMRADIUS = 0.6124f;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const float FIELD_OF_VIEW = 0.785f;
const float NEAR =  0.1f;
const float FAR = 1000.0f;

#define ASPECT_RATIO ( (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT )

// ### FUNCTION DEFINITIONS ### //

// ## INLINE FUNCTIONS ## //
static inline float min3(float x, float y, float z) {
	return fminf(fminf(x, y), z);
}
static inline float max3(float x, float y, float z) {
	return fmaxf(fmaxf(x, y), z);
}

// ## STRUCT FUNCTIONS ## //
// # CREATE AND DESTROY FUNCTIONS # //
void destroy_line(Line **line) {
	if ((!line) || (!(*line))) {
		return;
	}
	
	free((*line)->points);
	free((*line));
	*line = NULL;
}

void destroy_lines(Line *lines, int count) {
	if (!lines) {
		return;
	}
	
	for (int i = 0; i < count; i++) {
		free(lines[i].points);
	}
	free(lines);
}

void destroy_object(Object **object) {
	if ((!object) || (!(*object))) {
		return;
	}
	
	// Calling free on a NULL pointer is a no-op
	free((*object)->points);
	free(*object);
	*object = NULL;
}

Tetrahedron create_tetrahedron(Vec3 center, float side_length) {
	float circumcenter = UNIT_EQ_TRIANGLE_CIRCUMCENTER * side_length;
	float circumradius = UNIT_TETRAHEDRON_CIRCUMRADIUS * side_length;
	// Tetrahedron centered at the given position
	Tetrahedron th = {
		.vertices = {
			(Vec3){ center.x + side_length / 2.0f, center.y - circumradius, center.z + sqrtf(3.0f) * side_length / 6.0f }, (Vec3){ center.x, center.y - circumradius, center.z - circumcenter },
			(Vec3){ center.x - side_length / 2.0f, center.y - circumradius, center.z + sqrtf(3.0f) * side_length / 6.0f }, (Vec3){ center.x, center.y + circumradius, center.z }
		},
		.edges = {
			{ 0, 1 }, { 1, 2 }, { 2, 0 },
			{ 3, 0 }, { 3, 1 }, { 3, 2 }
		},
	};
	
	return th;
}

Cube create_cube(Vec3 center, float side_length) {
	// Cube centered at the origin
	Cube cube = {
		.vertices = {
			(Vec3){ -side_length / 2.0f, -side_length / 2.0f, -side_length / 2.0f }, (Vec3){ side_length / 2.0f, -side_length / 2.0f, -side_length / 2.0f },
			(Vec3){ side_length / 2.0f, -side_length / 2.0f, side_length / 2.0f }, (Vec3){ -side_length / 2.0f, -side_length / 2.0f, side_length / 2.0f },
			(Vec3){ -side_length / 2.0f, side_length / 2.0f, -side_length / 2.0f }, (Vec3){ side_length / 2.0f, side_length / 2.0f, -side_length / 2.0f },
			(Vec3){ side_length / 2.0f, side_length / 2.0f, side_length / 2.0f }, (Vec3){ -side_length / 2.0f, side_length/ 2.0f, side_length / 2.0f }
		},
		.edges = {
			{ 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
			{ 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 },
			{ 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 }
		},
	};
	
	// Translate cube
	for (int i = 0; i < 8; i++) {
		cube.vertices[i] = vec3_add(cube.vertices[i], center);
	}
	
	return cube;
}

// ## MATRIX TRANSFORMATIONS ## //
Matrix4 gen_view_matrix(Camera *cam) {
	// View-coordinates = World-coordinates from the camera's perspective
	// w is the direction from the point the camera looks at, to the camera
	Vec3 w = vec3_normalize(vec3_sub(cam->eye, cam->center));
	// right direction
	Vec3 u = vec3_normalize(vec3_cross_product(cam->up_direction, w));
	// Correct True up vector
	Vec3 v = vec3_cross_product(w, u);
	
	// Change of basis to the camera orthonormal basis u, v, w
	// And add the translation
	Matrix4 view_matrix = { .m = {
			{u.x, v.x, w.x, -vec3_dot_product(cam->eye, u)},
			{u.y, v.y, w.y, -vec3_dot_product(cam->eye, v)},
			{u.z, v.z, w.z, -vec3_dot_product(cam->eye, w)},
			{0.0f, 0.0f, 0.0f, 1.0f}
		},
	};
	
	return view_matrix;
}

Matrix4 gen_perspective_projection_matrix() {
	Matrix4 perspective_projection_matrix = { .m = {
			{1.0f / (ASPECT_RATIO * tanf(FIELD_OF_VIEW / 2.0f)), 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f / tanf(FIELD_OF_VIEW / 2.0f), 0.0f, 0.0f},
			{0.0f, 0.0f, -(FAR + NEAR) / (FAR - NEAR), -(2 * FAR * NEAR) / (FAR - NEAR)},
			{0.0f, 0.0f, -1.0f, 0.0f}
		},
	};
	
	return perspective_projection_matrix;
}

// The perspective information is encoded in w
Vec3 perspective_divide(Vec4 v) {
	// POTENTIAL BUG: Handle divide by 0 or very close to 0 values
	return (Vec3){ v.x / v.w, v.y / v.w, v.z / v.w };
}

Vec3 viewport_transform(Vec3 v) {
	return (Vec3){ (v.x + 1.0f) / 2.0f * SCREEN_WIDTH, (1.0f - v.y) / 2.0f * SCREEN_HEIGHT, v.z };
}

Vec3 world_to_viewport(Camera *cam, Vec4 v) {
	Matrix4 view_matrix = gen_view_matrix(cam);
	Matrix4 projection_matrix = gen_perspective_projection_matrix();
	
	Vec4 projected_vector = mat4_vec4_mul(mat4_mul(projection_matrix, view_matrix), v);
	Vec3 perspective_vector = perspective_divide(projected_vector);
	
	return viewport_transform(perspective_vector);
}

// ## DRAWING ALGORITHMS ## //
Line bresenham_line(IVec2 p0, IVec2 p1) {
	int x0 = p0.x;
	int y0 = p0.y;
	int x1 = p1.x;
	int y1 = p1.y;
	
	// Magnitude of change
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	
	// Directionf of change
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	
	// Decision variable proportional to the difference from the true line
	// and the rasterized line
	int err = dx - dy;
	
	int capacity = dx + dy + 1;
	// POTENTIAL FIX: Every malloc needs a free
	IVec2* points = malloc(capacity * sizeof(Vec2));
	if (!points) {
		return (Line){ NULL, 0 };
	}
	
	int index = 0;
	while (true) {
		points[index++] = (IVec2){ x0, y0};
		if (x0 == x1 && y0 == y1) {
			break;
		}
		
		// Helper to avoid division or floating-point math
		int err2 = 2 * err;
		
		if (err2 > -dy) {
			err -=dy;
			x0 += sx;
		}
		if (err2 < dx) {
			err += dx;
			y0 += sy;
		}
	}

	return (Line){ points, index };
}

Vec3 barycentric_coordinates(Vec2 a, Vec2 b, Vec2 c, Vec2 point) {
	float total_signed_area = signed_area(a, b, c);
	float alpha = signed_area(point, b, c) / total_signed_area;
	float beta = signed_area(a, point, c) / total_signed_area;
	float gamma = signed_area(a, b, point) / total_signed_area;
	
	return (Vec3){ alpha, beta, gamma };
}

bool point_in_triangle(Vec3 b_coordinates) {
	return b_coordinates.x >= 0 && b_coordinates.y >= 0 && b_coordinates.z >= 0;
}

// ## DRAWING FUNCTIONS ## //
bool draw_object(Uint32 *buffer, int pitch, ViewObject *object) {
	if ((!buffer) || (!object)) {
		return false;
	}
	
	Pixel *pixels = object->pixels;
	for (int i = 0; i < object->count; i++) {
		// Pixel at coordinates (x, y)
		// (0, 0) at top left and (639, 479) at bottom right
		buffer[pixels[i].pos.y * (pitch / 4) + pixels[i].pos.x] = (pixels[i].color.a << 24) | (pixels[i].color.r << 16) | (pixels[i].color.g << 8) | pixels[i].color.b;
	}
	return true;
}

// ## DRAWING UTILS ## //
Pixel ivec2_to_pixel(IVec2 v, ColorRgb color) {
	return (Pixel){ v, color};
}

Object *points_to_object(IVec2 *points, int count) {
	if (!points) {
		return NULL;
	}
	
	// POTENTIAL BUG: Allows fow dangling pointers if the original array gets freed outside...
	Object *object = malloc(sizeof(Object));
	if (!object) {
		return NULL;
	}
	
	object->points = points;
	object->count = count;
	return object;
}

// ViewObject does NOT take ownership of the object whose reference it receives
ViewObject *object_to_view_object(Object *object, ColorRgb color) {
	if  ((!object) || (!(object->points))) {
		return NULL;
	}
	
	ViewObject *view_object = malloc(sizeof(ViewObject));
	if (!view_object) {
		return NULL;
	}
	
	view_object->count = object->count;
	IVec2 *points = object->points;
	Pixel *pixels = malloc(view_object->count * sizeof(Pixel));
	if (!pixels) {
		free(view_object);
		return NULL;
	}
	
	for (int i = 0; i < view_object->count; i++) {
		pixels[i] = (Pixel){ points[i], color };
	}
	view_object->pixels = pixels;
	return view_object;
}

// ## GEOMETRIC FUNCTIONS ## //
Line get_line_points(Vec3 from, Vec3 to, Camera *cam) {
	if (!cam) {
		return (Line){ NULL, 0 };
	}
	
	Vec3 viewport_from = world_to_viewport(cam, vec3_homogenous(from, 1.0f));
	Vec3 viewport_to = world_to_viewport(cam, vec3_homogenous(to, 1.0f));
	Line line = bresenham_line((IVec2){ (int)viewport_from.x, (int)viewport_from.y}, (IVec2){ (int)viewport_to.x, (int)viewport_to.y});
	
	return line;
}

IVec2 *get_triangle_points(Triangle t, Camera *cam, int *point_count) {
	if (!point_count) {
		return NULL;
	}
	if (!cam) {
		*point_count = 0;
		return NULL;
	}
	
	// World -> Viewport coordinates
	Vec3 a = world_to_viewport(cam, vec3_homogenous(t.vertices[0], 1.0f));
	Vec3 b = world_to_viewport(cam, vec3_homogenous(t.vertices[1], 1.0f));
	Vec3 c = world_to_viewport(cam, vec3_homogenous(t.vertices[2], 1.0f));
	
	// Max Rectangular Bound
	int min_x = (int)floorf(min3(a.x, b.x, c.x));
	int max_x = (int)ceilf(max3(a.x, b.x, c.x));
	int min_y = (int)floorf(min3(a.y, b.y, c.y));
	int max_y = (int)ceilf(max3(a.y, b.y, c.y));
	
	// POTENTIAL FIX: Every malloc needs a free...
	int capacity = (max_x - min_x + 1) * (max_y - min_y + 1);
	IVec2 *points = malloc(capacity * sizeof(IVec2));
	if (!points) {
		*point_count = 0;
		return NULL;
	}
	
	Vec2 a_pixel = { a.x, a.y };
	Vec2 b_pixel = { b.x, b.y };
	Vec2 c_pixel = { c.x, c.y };
	Vec3 b_coordinates = { 0.0f, 0.0f, 0.0f };
	int index = 0;
	for (int x = min_x; x < max_x; x++) {
		for (int y = min_y; y < max_y; y++) {
			b_coordinates = barycentric_coordinates(
				a_pixel, b_pixel, c_pixel,
				(Vec2){ (float)x, (float)y }
			);
			
			if (point_in_triangle(b_coordinates)) {
				// POTENTIAL FIX: This is in case of barycentric rounding error...
				if (index < capacity) {
					points[index] = (IVec2){ x, y };
					index++;
				}
			}
		}
	}
	
	*point_count = index;
	return points;
}

IVec2 *get_tetrahedron_points(Tetrahedron th, Camera *cam, int *point_count) {
	if (!point_count) {
		return NULL;
	}
	if (!cam) {
		*point_count = 0;
		return NULL;
	}
	
	// POTENTIAL FIX: Every malloc needs a free...
	Line *edges = malloc(6 * sizeof(Line));
	if (!edges) {
		*point_count = 0;
		return NULL;
	}
	
	int total_point_count = 0;
	for (int i = 0; i < 6; i++) {
		edges[i] = get_line_points(th.vertices[th.edges[i][0]], th.vertices[th.edges[i][1]], cam);
		total_point_count += edges[i].count;
	}
	
	IVec2 *points = malloc(total_point_count * sizeof(IVec2));
	if (!points) {
		destroy_lines(edges, 6);
		*point_count = 0;
		return NULL;
	}
	
	int index = 0;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < edges[i].count; j++) {
			points[index] = edges[i].points[j];
			index++;
		}
	}
	
	destroy_lines(edges, 6);
	*point_count = index;
	return points;
}

Line *get_cube_edges(Cube cube, Camera *cam) {
	if (!cam) {
		return NULL;
	}
	// POTENTIAL FIX: Every malloc needs a free...
	Line *edges = malloc(12 * sizeof(Line));
	if (!edges) {
		return NULL;
	}
	
	// Convert to viewport coordinates
	Vec3 viewport_vertices[8];
	for (int i = 0; i < 8; i++) {
		viewport_vertices[i] = world_to_viewport(cam, vec3_homogenous(cube.vertices[i], 1.0f));
	}
	
	// Construct the edges of the cube
	for (int i = 0; i < 12; i++) {
		Vec3 from_vector = viewport_vertices[cube.edges[i][0]];
		Vec3 to_vector = viewport_vertices[cube.edges[i][1]];
		// edges[i] is equivalent to *(edges + i)
		edges[i] = bresenham_line((IVec2){ (int)from_vector.x, (int)from_vector.y }, (IVec2){ (int)to_vector.x, (int)to_vector.y });
	}
	
	return edges;
}

IVec2 *get_cube_points(Cube cube, Camera *cam, int *point_count) {
	if (!point_count) {
		return NULL;
	}
	if (!cam) {
		*point_count = 0;
		return NULL;
	}
	// Every malloc
	Line *edges = get_cube_edges(cube, cam);
	if (!edges) {
		*point_count = 0;
		return NULL;
	}
	
	int total_points = 0;
	for (int i = 0; i < 12; i++) {
		total_points += edges[i].count;
	}
	// POTENTIAL FIX: Every malloc needs a free...
	IVec2 *points = malloc(total_points * sizeof(IVec2));
	
	if (!points) {
		// Needs a free
		destroy_lines(edges, 12);
		*point_count = 0;
		return NULL;
	}
	
	int index = 0;
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < edges[i].count; j++) {
			points[index] = edges[i].points[j];
			index++;
		}
	}
	
	destroy_lines(edges, 12);
	*point_count = index;
	return points;
}

bool draw_line(Uint32 *buffer, Camera *cam, Vec3 from, Vec3 to, ColorRgb color, int pitch) {
	if ((!buffer) || (!cam)) {
		return false;
	}
	
	Line line = get_line_points(to, from, cam);
	if (!line.points) {
		return false;
	}
	Object *object = points_to_object(line.points, line.count);
	if (!object) {
		return false;
	}
	ViewObject *view_object = object_to_view_object(object, color);
	if (!view_object) {
		return false;
	}
	
	return draw_object(buffer, pitch, view_object);
}

bool draw_triangle(Uint32 *buffer, Camera *cam, Triangle t, ColorRgb color, int pitch) {
	if (!(buffer && cam)) {
		return false;
	}
	
	int npoints;
	IVec2 *points = get_triangle_points(t, cam, &npoints);
	Object *object = points_to_object(points, npoints);
	ViewObject *view_object = object_to_view_object(object, color);
	
	if (!(points && object && view_object)) {
		return false;
	}
	
	return draw_object(buffer, pitch, view_object);
}

bool draw_tetrahedron(Uint32 *buffer, Camera *cam, Tetrahedron th, ColorRgb color, int pitch) {
	if (!(buffer && cam)) {
		return false;
	}
	
	int npoints;
	IVec2 *points = get_tetrahedron_points(th, cam, &npoints);
	Object *object = points_to_object(points, npoints);
	ViewObject *view_object = object_to_view_object(object, color);
	
	if (!(points && object && view_object)) {
		return false;
	}
	
	return draw_object(buffer, pitch, view_object);
}

bool draw_cube(Uint32 *buffer, Camera *cam, Cube cube, ColorRgb color, int pitch) {
	if (!(buffer && cam)) {
		return false;
	}
	// Get screen points of the cube
	int npoints;
	IVec2 *points = get_cube_points(cube, cam, &npoints);
	Object *object = points_to_object(points, npoints);
	ViewObject *view_object = object_to_view_object(object, color);
	
	if (!(points && object && view_object)) {
		return false;
	}
	
	return draw_object(buffer, pitch, view_object);
}