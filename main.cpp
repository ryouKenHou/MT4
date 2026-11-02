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
	
	Vector3 form0 = Vector3::Normalized(Vector3(1.0f, 0.70f, 0.50f));
	Vector3 to0 = form0 * -1;
	Vector3 from1 = Vector3::Normalized(Vector3(-0.60f, 0.9f, 0.2f));
	Vector3 to1 = Vector3::Normalized(Vector3(0.4f, 0.7f, -0.5f));
	Matrix4x4 rotateMatrix0 = MakeRotateMatrix(DirectionToDirectionQuaternion(
		Vector3(1.f,0.f,0.f),
		Vector3(-1.f,0.f,0.f)));
	Matrix4x4 rotateMatrix1 = MakeRotateMatrix(DirectionToDirectionQuaternion(
		form0,
		to0));
	Matrix4x4 rotateMatrix2 = MakeRotateMatrix(DirectionToDirectionQuaternion(
		from1,
		to1));
	

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
		
		MatrixScreenPrintf(0, 0, rotateMatrix0, "rotateMatrix0");
		MatrixScreenPrintf(0, 200, rotateMatrix1, "rotateMatrix1");
		MatrixScreenPrintf(0, 400, rotateMatrix2, "rotateMatrix2");

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
