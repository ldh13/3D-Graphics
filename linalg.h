#include <stdio.h>

// ### STRUCTS ### //

// ## VECTORS STRUCTS ## //
typedef struct {
	float x;
	float y;
} Vec2;

typedef struct {
	float x;
	float y;
	float z;
} Vec3;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} Vec4;

// ## MATRIX STRUCTS ## //
typedef struct {
	float m[2][2];
} Matrix2;

typedef struct {
	float m[3][3];
} Matrix3;

typedef struct {
	float m[4][4];
} Matrix4;

// ### FUNCTION DECLARATIONS ### //

// ## VECTOR STRUCTS ## //
// # CREATE AND DESTROY FUNCTIONS # //
// POTENTIAL FIX: One function that checks type?
Vec2 *create_vec2(float x, float y);
Vec3 *create_vec3(float x, float y, float z);
Vec4 *create_vec4(float x, float y, float z, float w);

void destroy_vec2(Vec2 **v2);
void destroy_vec3(Vec3 **v3);
void destroy_vec4(Vec4 **v4);

// # BASIC ARITHMETIC OPERATIONS # //
Vec2 vec2_add(Vec2 v1, Vec2 v2);
Vec2 vec2_sub(Vec2 v1, Vec2 v2);
Vec2 vec2_scale(Vec2 v, float scalar);

Vec3 vec3_add(Vec3 v1, Vec3 v2);
Vec3 vec3_sub(Vec3 v1, Vec3 v2);
Vec3 vec3_scale(Vec3 v, float scalar);

Vec4 vec4_add(Vec4 v1, Vec4 v2);
Vec4 vec4_sub(Vec4 v1, Vec4 v2);
Vec4 vec4_scale(Vec4 v, float scalar);

// # VECTOR PROPERTIES AND OPERATIONS # //
float vec2_dot_product(Vec2 v1, Vec2 v2);
float vec2_length_squared(Vec2 v);
float vec2_length(Vec2 v);
Vec2 vec2_normalize(Vec2 v);

float vec3_dot_product(Vec3 v1, Vec3 v2);
float vec3_length_squared(Vec3 v);
float vec3_length(Vec3 v);
Vec3 vec3_normalize(Vec3 v);
Vec3 vec3_cross_product(Vec3 v1, Vec3 v2);
Vec4 vec3_homogenous(Vec3 v, float w);

float vec4_dot_product(Vec4 v1, Vec4 v2);
float vec4_length_squared(Vec4 v);
float vec4_length(Vec4 v);
Vec4 vec4_normalize(Vec4 v);

// # ADVANCED VECTOR OPERATIONS # //
// 2D Vector
float vec2_distance_squared(Vec2 v1, Vec2 v2);
float vec2_distance(Vec2 v1, Vec2 v2);

Vec2 vec2_lerp(Vec2 v1, Vec2 v2, float t);
Vec2 vec2_project(Vec2 v1, Vec2 v2);

// 3D Vector
float vec3_distance_squared(Vec3 v1, Vec3 v2);
float vec3_distance(Vec3 v1, Vec3 v2);

Vec3 vec3_lerp(Vec3 v1, Vec3 v2, float t);
Vec3 vec3_project(Vec3 v1, Vec3 v2);

// 4D Vector
float vec4_distance_squared(Vec4 v1, Vec4 v2);
float vec4_distance(Vec4 v1, Vec4 v2);

Vec4 vec4_lerp(Vec4 v1, Vec4 v2, float t);
Vec4 vec4_project(Vec4 v1, Vec4 v2);

// # VECTOR UTILS FUNCTIONS # //
// 2D Vector
void vec2_print(Vec2 *v);
// 3D Vector
void vec3_print(Vec3 *v);
// 4D Vector
void vec4_print(Vec4 *v);

// ## MATRIX STRUCTS ## //
// // # CREATE AND DESTROY FUNCTIONS # //
// Matrix2 *create_Matrix2(float x, float y);
// Matrix3 *create_Matrix3(float x, float y, float z);
// Matrix4 *create_Matrix4(float w, float x, float y, float z);

// void destroy_Matrix2(float x, float y);
// void destroy_Matrix3(float x, float y, float z);
// void destroy_Matrix4(float w, float x, float y, float z

// // // # MATRIX AS ROWS/COLS # //
// Vec2 **mat2_cols(Matrix2 m);
// Vec2 **mat_as_rows(Matrix2 m);

// # BASIC ARITHMETIC OPERATIONS # //
// Matrix order 2
Matrix2 mat2_add(Matrix2 a, Matrix2 b);
Matrix2 mat2_sub(Matrix2 a, Matrix2 b);
Matrix2 mat2_mul(Matrix2 a, Matrix2 b);

// Matrix order 3
Matrix3 mat3_add(Matrix3 a, Matrix3 b);
Matrix3 mat3_sub(Matrix3 a, Matrix3 b);
Matrix3 mat3_mul(Matrix3 a, Matrix3 b);

// Matrix order 4
Matrix4 mat4_add(Matrix4 a, Matrix4 b);
Matrix4 mat4_sub(Matrix4 a, Matrix4 b);
Matrix4 mat4_mul(Matrix4 a, Matrix4 b);

// # ROTATION, SCALING AND TRANSLATION # //
// POTENTIAL FIX: Should these matrix vector operations be here?...
Vec4 mat4_vec4_mul(Matrix4 m, Vec4 v);

// Rotation
Matrix4 rotation_xaxis(float theta);
Matrix4 rotation_yaxis(float theta);
Matrix4 rotation_zaxis(float theta);

// Scaling
Matrix4 scaling_matrix(float scalar);

// Translation
Matrix4 translate_vec(Vec3 t);

// # MATRIX UTILS FUNCTIONS #//
// ...

// Matrix order 3
void print_mat3(Matrix3 m);

// Matrix order 4
void print_mat4(Matrix4 m);

// ...