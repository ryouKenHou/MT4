#pragma once
#include <cmath>
#include "matrix4x4.hpp"

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

inline Quaternion MakeRotateAsisAngleQuaternion(const Vector3& axis, float angle) {
	float halfAngle = angle * 0.5f;
	float sinHalfAngle = std::sin(halfAngle);
	float cosHalfAngle = std::cos(halfAngle);
	return Quaternion(
		axis.x * sinHalfAngle,
		axis.y * sinHalfAngle,
		axis.z * sinHalfAngle,
		cosHalfAngle
	);
}

inline Vector3 RotateVector(const Vector3& v, const Quaternion& q) {
	Quaternion p(v.x, v.y, v.z, 0);
	Quaternion qConjugate = Conjugate(q);
	Quaternion rotatedP = q * p * qConjugate;
	return Vector3(rotatedP.x, rotatedP.y, rotatedP.z);
}

Matrix4x4 MakeRotateMatrix(const Quaternion& q) {
	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float ww = q.w * q.w;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;
	return Matrix4x4(
		ww + xx - yy - zz, 2.0f * (xy + wz), 2.0f * (xz - wy), 0.0f,
		2.0f * (xy - wz), ww - xx + yy - zz, 2.0f * (yz + wx), 0.0f,
		2.0f * (xz + wy), 2.0f * (yz - wx), ww - xx - yy + zz, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

}