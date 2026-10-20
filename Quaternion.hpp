#pragma once
#include <cmath>

struct Quaternion {
	float x;
	float y;
	float z;
	float w;

	Quaternion() : x(0), y(0), z(0), w(1) {}
	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}



};

inline Quaternion Multiply(const Quaternion& q1, const Quaternion& q2) {
	Quaternion result;
	result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
	return result;
}

inline Quaternion IndentityQuaternion() {
	return Quaternion( 0, 0, 0,1);
}

inline Quaternion Normalize(const Quaternion& q) {
	float length = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	if (length == 0) {
		return Quaternion(0, 0, 0, 1); // Avoid division by zero
	}
	return Quaternion(q.x / length, q.y / length, q.z / length, q.w / length);
}

inline Quaternion Conjugate(const Quaternion& q) {
	return Quaternion(-q.x, -q.y, -q.z, q.w);
}

inline Quaternion operator/(const Quaternion& q, float scalar) {
	return Quaternion(q.x / scalar, q.y / scalar, q.z / scalar, q.w / scalar);
}

inline Quaternion Inverse(const Quaternion& q) {
	float lengthSquared = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
	if (lengthSquared == 0) {
		return Quaternion(0, 0, 0, 1); // Avoid division by zero
	}
	return Conjugate(q) / lengthSquared;
}

inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs) {
	Quaternion q;
	q = Multiply(lhs, rhs);
	return q;
}

inline Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs) {
	return Quaternion(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

inline Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs) {
	return Quaternion(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

inline Quaternion operator*(const Quaternion& q, float scalar) {
	return Quaternion(q.x * scalar, q.y * scalar, q.z * scalar, q.w * scalar);
}

inline Quaternion operator*(float scalar, const Quaternion& q) {
	return Quaternion(q.x * scalar, q.y * scalar, q.z * scalar, q.w * scalar);
}

inline float Norm(const Quaternion& q) {
	return std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}
