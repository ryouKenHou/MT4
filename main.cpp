#include <Novice.h>
#include <numbers>
#include <imgui.h>

#include "Vector2.hpp"
#include "matrix4x4.hpp"
#include <algorithm>
#include "Quaternion.hpp"

#include <string>

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

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, std::string s) {
	Novice::ScreenPrintf(x, y+0, s.c_str());
	Novice::ScreenPrintf(x, y+40, "(%.3f, %.3f, %.3f, %.3f)", matrix.m[0][0], matrix.m[0][1], matrix.m[0][2], matrix.m[0][3]);
	Novice::ScreenPrintf(x, y+60, "(%.3f, %.3f, %.3f, %.3f)", matrix.m[1][0], matrix.m[1][1], matrix.m[1][2], matrix.m[1][3]);
	Novice::ScreenPrintf(x, y+80, "(%.3f, %.3f, %.3f, %.3f)", matrix.m[2][0], matrix.m[2][1], matrix.m[2][2], matrix.m[2][3]);
	Novice::ScreenPrintf(x, y+100, "(%.3f, %.3f, %.3f, %.3f)", matrix.m[3][0], matrix.m[3][1], matrix.m[3][2], matrix.m[3][3]);
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
	
	Quaternion rotation0 = MakeRotateAsisAngleQuaternion(Vector3(0.710f, 0.710f, 0.0f), 0.3f);
	Quaternion rotation1 = MakeRotateAsisAngleQuaternion(Vector3(0.710f, 0.0f, 0.710f), 3.141592f);

	Quaternion interpolated0 = Slerp(rotation0, rotation1, 0.0f);
	Quaternion interpolated1 = Slerp(rotation0, rotation1, 0.3f);
	Quaternion interpolated2 = Slerp(rotation0, rotation1, 0.5f);
	Quaternion interpolated3 = Slerp(rotation0, rotation1, 0.7f);
	Quaternion interpolated4 = Slerp(rotation0, rotation1, 1.0f);
	

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
		
		Novice::ScreenPrintf(0, 0, "interpolated0(0.0f): %.2f, %.2f, %.2f, %.2f ", interpolated0.x, interpolated0.y, interpolated0.z, interpolated0.w);
		Novice::ScreenPrintf(0, 20, "interpolated1(0.3f): %.2f, %.2f, %.2f, %.2f ", interpolated1.x, interpolated1.y, interpolated1.z, interpolated1.w);
		Novice::ScreenPrintf(0, 40, "interpolated2(0.5f): %.2f, %.2f, %.2f, %.2f ", interpolated2.x, interpolated2.y, interpolated2.z, interpolated2.w);
		Novice::ScreenPrintf(0, 60, "interpolated3(0.7f): %.2f, %.2f, %.2f, %.2f ", interpolated3.x, interpolated3.y, interpolated3.z, interpolated3.w);
		Novice::ScreenPrintf(0, 80, "interpolated4(1.0f): %.2f, %.2f, %.2f, %.2f ", interpolated4.x, interpolated4.y, interpolated4.z, interpolated4.w);

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
