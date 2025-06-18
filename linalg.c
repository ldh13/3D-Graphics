#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "linalg.h"

// ### FUNCTION DEFINITIONS ### //

// ## VECTOR STRUCTS ## //
// # CREATE AND DESTROY FUNCTIONS # //
Vec2 *create_vec2(float x, float y) {
	Vec2 *v2 = (Vec2*)malloc(sizeof(Vec2));
	if (!v2) {
		return NULL;
	}
	
	v2->x = x;
	v2->y = y;
	return v2;
}
Vec3 *create_vec3(float x, float y, float z) {
	Vec3 *v3 = (Vec3*)malloc(sizeof(Vec3));
	if (!v3) {
		return NULL;
	}
	
	v3->x = x;
	v3->y = y;
	v3->z = z;
	return v3;
}
Vec4 *create_vec4(float x, float y, float z, float w) {
	Vec4 *v4 = (Vec4*)malloc(sizeof(Vec4));
	if (!v4) {
		return NULL;
	}
	
	v4->x = x;
	v4->y = y;
	v4->z = z;
	v4->w = w;
	return v4;
}

void destroy_vec2(Vec2 **v2) {
	if (!v2) {
		// Handle NULL pointer
		return;
	}
	
	free(*v2);
	*v2 = NULL;
}
void destroy_vec3(Vec3 **v3) {
	if (!v3) {
		// Handle NULL pointer
		return;
	}
	
	free(*v3);
	*v3 = NULL;
}
void destroy_vec4(Vec4 **v4) {
	if (!v4) {
		// Handle NULL pointer
		return;
	}
	
	free(*v4);
	*v4 = NULL;
}

// # BASIC ARITHMETIC OPERATIONS # //
// 2D Vector
Vec2 vec2_add(Vec2 v1, Vec2 v2) {
	return (Vec2){ v1.x + v2.x, v1.y + v2.y };
}

Vec2 vec2_sub(Vec2 v1, Vec2 v2) {
	return (Vec2){ v1.x - v2.x, v1.y - v2.y};
}

Vec2 vec2_scale(Vec2 v, float scalar) {
	return (Vec2){ v.x * scalar, v.y * scalar };
}

// 3D Vector
Vec3 vec3_add(Vec3 v1, Vec3 v2) {
	return (Vec3){ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

Vec3 vec3_sub(Vec3 v1, Vec3 v2) {
	return (Vec3){ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

Vec3 vec3_scale(Vec3 v, float scalar) {
	return (Vec3){ v.x * scalar, v.y * scalar, v.z * scalar };
}

// 4D Vector
Vec4 vec4_add(Vec4 v1, Vec4 v2) {
	return (Vec4){ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}

Vec4 vec4_sub(Vec4 v1, Vec4 v2) {
	return (Vec4){ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}

Vec4 vec4_scale(Vec4 v, float scalar) {
	return (Vec4){ v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar };
}

// # VECTOR PROPERTIES AND OPERATIONS # //
// 2D Vector
float vec2_dot_product(Vec2 v1, Vec2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float vec2_length_squared(Vec2 v) {
	return vec2_dot_product(v, v);
}

float vec2_length(Vec2 v) {
	return sqrtf(vec2_length_squared(v));
}

Vec2 vec2_normalize(Vec2 v) {
	float magnitude = vec2_length(v);
	return magnitude == 0.0f ? v : vec2_scale(v, 1.0f / magnitude);
}

float signed_area(Vec2 a, Vec2 b, Vec2 c) {
	Vec2 a_b = vec2_sub(b, a);
	Vec2 a_c = vec2_sub(c, a);
	return a_b.x *a_c.y - a_b.y * a_c.x;
}

// 3D Vector
float vec3_dot_product(Vec3 v1, Vec3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float vec3_length_squared(Vec3 v) {
	return vec3_dot_product(v, v);
}

float vec3_length(Vec3 v) {
	return sqrtf(vec3_length_squared(v));
}

Vec3 vec3_normalize(Vec3 v) {
	float magnitude = vec3_length(v);
	return magnitude == 0.0f ? v : vec3_scale(v, 1.0f / magnitude);
}

Vec3 vec3_cross_product(Vec3 v1, Vec3 v2) {
	return (Vec3){
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x,
	};
}

Vec4 vec3_homogenous(Vec3 v, float w) {
	return (Vec4){ v.x, v.y, v.z, w };
}

// 4D Vector
float vec4_dot_product(Vec4 v1, Vec4 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

float vec4_length_squared(Vec4 v) {
	return vec4_dot_product(v, v);
}

float vec4_length(Vec4 v) {
	return sqrtf(vec4_length_squared(v));
}

Vec4 vec4_normalize(Vec4 v) {
	float magnitude = vec4_length(v);
	return magnitude == 0.0f ? v : vec4_scale(v, 1.0f / magnitude);
}

// # ADVANCED VECTOR OPERATIONS # //
// 2D Vector
float vec2_distance_squared(Vec2 v1, Vec2 v2) {
	return vec2_length_squared(vec2_sub(v1, v2));
}

float vec2_distance(Vec2 v1, Vec2 v2) {
	return sqrtf(vec2_distance_squared(v1, v2));
}

Vec2 vec2_lerp(Vec2 v1, Vec2 v2, float t) {
	return vec2_add(vec2_scale(v1, 1.0f - t), vec2_scale(v2, t));
}

Vec2 vec2_project(Vec2 v1, Vec2 v2) {
	float v2_length_sq = vec2_length_squared(v2);
	return v2_length_sq == 0.0f ? v2 : vec2_scale(v2, vec2_dot_product(v1, v2) / v2_length_sq);
}

// 3D Vector
float vec3_distance_squared(Vec3 v1, Vec3 v2) {
	return vec3_length_squared(vec3_sub(v1, v2));
}

float vec3_distance(Vec3 v1, Vec3 v2) {
	return sqrtf(vec3_distance_squared(v1, v2));
}

Vec3 vec3_lerp(Vec3 v1, Vec3 v2, float t) {
	return vec3_add(vec3_scale(v1, 1.0f - t), vec3_scale(v2, t));
}

Vec3 vec3_project(Vec3 v1, Vec3 v2) {
	float v2_length_sq = vec3_length_squared(v2);
	return v2_length_sq == 0.0f ? v2 : vec3_scale(v2, vec3_dot_product(v1, v2) / v2_length_sq);
}

// 4D Vector
float vec4_distance_squared(Vec4 v1, Vec4 v2) {
	return vec4_length_squared(vec4_sub(v1, v2));
}

float vec4_distance(Vec4 v1, Vec4 v2) {
	return sqrtf(vec4_distance_squared(v1, v2));
}

Vec4 vec4_lerp(Vec4 v1, Vec4 v2, float t) {
	return vec4_add(vec4_scale(v1, 1.0f - t), vec4_scale(v2, t));
}

Vec4 vec4_project(Vec4 v1, Vec4 v2) {
	float v2_length_sq = vec4_length_squared(v2);
	return v2_length_sq == 0.0f ? v2 : vec4_scale(v2, vec4_dot_product(v1, v2) / v2_length_sq);
}

// # VECTOR UTILS FUNCTIONS # //
// 2D Vector
void vec2_print(Vec2 *v) {
	if (v) {
		printf("Vec2: x = %0.2f, y = %0.2f\n", v->x, v->y);
	}
}
// 3D Vector
void vec3_print(Vec3 *v) {
	if (v) {
		printf("Vec3: x = %0.2f, y = %0.2f, z = %0.2f\n", v->x, v->y, v->z);
	}
}

// 4D Vector
void vec4_print(Vec4 *v) {
	if (v) {
		printf("Vec4: x = %0.2f, y = %0.2f, z = %0.2f, w = %0.2f\n", v->x, v->y, v->z, v->w);
	}
}

// ## MATRIX STRUCT ## //
// # CREATE AND DESTROY FUNCTIONS # //
// Matrix2 *create_Matrix2(float x, float y);
// Matrix3 *create_Matrix3(float x, float y, float z);
// Matrix4 *create_Matrix4(float w, float x, float y, float z);

// void destroy_Matrix2(float x, float y);
// void destroy_Matrix3(float x, float y, float z);
// void destroy_Matrix4(float w, float x, float y, float z);

// // // # MATRIX AS ROWS/COLS # //
// Vec2 **mat2_cols(Matrix2 m) {
	// Vec2 **cols = (Vec2**)malloc(2 * sizeof(Vec2*));
	// if (!cols) {
		// return NULL;
	// }
	
	// cols[0] = create_vec2(m[0][0], m[1][0]);
	// cols[1] = create_vec2(m[0][1], m[1][1]);
	// return cols;
// }

// Vec2 **mat2_rows(Matrix2 m) {
	// Vec2 **rows = (Vec2**)malloc(2 * sizeof(Vec2*));
	// if (!rows) {
		// return NULL;
	// }
	
	// rows[0] = create_vec2(m[0][0], m[0][1]);
	// rows[1] = create_vec2(m[1][0], m[1][1]);
	// return rows;
// }

// # BASIC ARITHMETIC OPERATIONS # //
// Matrix order 2
Matrix2 mat2_add(Matrix2 a, Matrix2 b) {
	Matrix2 result;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result.m[i][j] = a.m[i][j] + b.m[i][j];
		}
	}
	
	return result;
}

Matrix2 mat2_sub(Matrix2 a, Matrix2 b) {
	Matrix2 result;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result.m[i][j] = a.m[i][j] - b.m[i][j];
		}
	}
	
	return result;
}

Matrix2 mat2_mul(Matrix2 a, Matrix2 b) {
	Matrix2 result;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j];
		}
	}
	
	return result;
}

// Matrix order 3
Matrix3 mat3_add(Matrix3 a, Matrix3 b) {
	Matrix3 result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.m[i][j] = a.m[i][j] + b.m[i][j];
		}
	}
	
	return result;
}

Matrix3 mat3_sub(Matrix3 a, Matrix3 b) {
	Matrix3 result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.m[i][j] = a.m[i][j] - b.m[i][j];
		}
	}
	
	return result;
}

Matrix3 mat3_mul(Matrix3 a, Matrix3 b) {
	Matrix3 result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j];
		}
	}
	
	return result;
}

// Matrix order 4
Matrix4 mat4_add(Matrix4 a, Matrix4 b) {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = a.m[i][j] + b.m[i][j];
		}
	}
	
	return result;
}

Matrix4 mat4_sub(Matrix4 a, Matrix4 b) {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = a.m[i][j] - b.m[i][j];
		}
	}
	
	return result;
}

Matrix4 mat4_mul(Matrix4 a, Matrix4 b) {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
		}
	}
	
	return result;
}

// # ROTATION, SCALING AND TRANSLATION # //
// POTENTIAL FIX: Should these matrix vector operations be here?...
Vec4 mat4_vec4_mul(Matrix4 m, Vec4 v) {
	return (Vec4){
		m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w,
		m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w,
		m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w,
		m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w,
	};
}

// Rotation
Matrix4 rotation_xaxis(float theta) {
	Matrix4 rotation_matrix = { .m = {
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, cosf(theta), -sinf(theta), 0.0f},
			{0.0f, sinf(theta), cosf(theta), 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		},
	};
	
	return rotation_matrix;
}

Matrix4 rotation_yaxis(float theta) {
	Matrix4 rotation_matrix = { .m = {
			{cosf(theta), 0.0f, sinf(theta), 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{-sinf(theta), 0.0f, cosf(theta), 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		},
	};
	
	return rotation_matrix;
}

Matrix4 rotation_zaxis(float theta) {
	Matrix4 rotation_matrix = { .m = {
			{cosf(theta), -sinf(theta), 0.0f, 0.0f},
			{sinf(theta), cosf(theta), 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		},
	};
	
	return rotation_matrix;
}

// Scaling
Matrix4 scale_transformation(float scalar) {
	Matrix4 scaling_matrix = { .m = {
			{scalar, 0.0f, 0.0f, 0.0f},
			{0.0f, scalar, 0.0f, 0.0f},
			{0.0f, 0.0f, scalar, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		},
	};
	
	return scaling_matrix;
}

// Translation
Matrix4 translate_vec(Vec3 t) {
	Matrix4 translation_matrix = { .m = {
			{1.0f, 0.0f, 0.0f, t.x},
			{0.0f, 1.0f, 0.0f, t.y},
			{0.0f, 0.0f, 1.0f, t.z},
			{0.0f, 0.0f, 0.0f, 1.0f}
		},
	};
	
	return translation_matrix;
}

// # MATRIX UTILS FUNCTIONS #//
// Matrix order 2
void print_mat2(Matrix2 m) {
	for (int i = 0; i < 2; i++) {
		printf("|");
		for (int j = 0; j < 2; j++) {
			printf(" %0.2f", m.m[i][j]);
		}
		printf(" |\n");
	}
}

// Matrix order 3
void print_mat3(Matrix3 m) {
	for (int i = 0; i < 3; i++) {
		printf("|");
		for (int j = 0; j < 3; j++) {
			printf(" %0.2f", m.m[i][j]);
		}
		printf(" |\n");
	}
}

// Matrix order 4
void print_mat4(Matrix4 m) {
	for (int i = 0; i < 4; i++) {
		printf("|");
		for (int j = 0; j < 4; j++) {
			printf(" %0.2f", m.m[i][j]);
		}
		printf(" |\n");
	}
}