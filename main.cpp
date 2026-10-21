#include <Novice.h>
#include <numbers>
#include <imgui.h>

#include "Vector2.hpp"
#include "matrix4x4.hpp"
#include <algorithm>
#include "Quaternion.hpp"

const char kWindowTitle[] = "LC1B_35_リョウ_ケン_ホウ";

struct Spherical {
	float radius;
	float theta;
	float phi;
};


Vector3 ToCartesian(const Spherical& spherical) {
	float rho = spherical.radius * cos(spherical.theta);
	float x = rho * cos(spherical.phi);
	float y = spherical.radius * sin(spherical.theta);
	float z = rho * sin(spherical.phi);
	return Vector3(x, y, z);
}

Spherical ToSpherical(const Vector3& p) {
	float r = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
	if(r == 0.0f) {
		return Spherical{ 0.0f, 0.0f, 0.0f }; // Avoid division by zero
	}
	float sinTheta = std::clamp(p.y / r, -1.0f, 1.0f);
	float phi = 0.0f;
	if (p.x != 0.0f || p.z != 0.0f) {
		phi = std::atan2(p.z, p.x);
	}
	return Spherical{ r, std::asin(sinTheta), phi };

}

struct Vector2 {
	float x;
	float y;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	
	Quaternion rotation = MakeRotateAsisAngleQuaternion(Vector3::Normalized({ 1.0f, 0.40f, -0.20f}), 0.450f);
	Vector3 pointY = { 2.1f, -0.9f, 1.3f };
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = Transform(pointY, rotateMatrix);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		Novice::ScreenPrintf(0, 0, "rotation quaternion: (%.3f, %.3f, %.3f, %.3f)", rotation.x, rotation.y, rotation.z, rotation.w);
		Novice::ScreenPrintf(0, 20, "rotateMatrix:");
		Novice::ScreenPrintf(0, 40, "(%.3f, %.3f, %.3f, %.3f)", rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3]);
		Novice::ScreenPrintf(0, 60, "(%.3f, %.3f, %.3f, %.3f)", rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3]);
		Novice::ScreenPrintf(0, 80, "(%.3f, %.3f, %.3f, %.3f)", rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3]);
		Novice::ScreenPrintf(0, 100, "(%.3f, %.3f, %.3f, %.3f)", rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);
		Novice::ScreenPrintf(0, 120, "rotateByQuaternion: (%.3f, %.3f, %.3f)", rotateByQuaternion.x, rotateByQuaternion.y, rotateByQuaternion.z);
		Novice::ScreenPrintf(0, 140, "rotateByMatrix: (%.3f, %.3f, %.3f)", rotateByMatrix.x, rotateByMatrix.y, rotateByMatrix.z);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
