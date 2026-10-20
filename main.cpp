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
	
	Quaternion q1 = Quaternion(2.f, 3.0f, 4.0f, 1.0f);
	Quaternion q2 = Quaternion(1.f, 3.0f, 5.0f, 2.0f);
	Quaternion indentity = IndentityQuaternion();
	Quaternion conj = Conjugate(q1);
	Quaternion inv = Inverse(q1);
	Quaternion normal = Normalize(q1);
	Quaternion mul1 = Multiply(q1, q2);
	Quaternion mul2 = q2 * q1;
	float norm = Norm(q1);

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
		
		Novice::ScreenPrintf(0, 0,   "q1       : %.3f, %.3f, %.3f, %.3f", q1.w, q1.x, q1.y, q1.z);
		Novice::ScreenPrintf(0, 20,  "q2       : %.3f, %.3f, %.3f, %.3f", q2.w, q2.x, q2.y, q2.z);
		Novice::ScreenPrintf(0, 40,  "indentity: %.3f, %.3f, %.3f, %.3f", indentity.w, indentity.x, indentity.y, indentity.z);
		Novice::ScreenPrintf(0, 60,  "conj     : %.3f, %.3f, %.3f, %.3f", conj.w, conj.x, conj.y, conj.z);
		Novice::ScreenPrintf(0, 80,  "inv      : %.3f, %.3f, %.3f, %.3f", inv.w, inv.x, inv.y, inv.z);
		Novice::ScreenPrintf(0, 100, "normal   : %.3f, %.3f, %.3f, %.3f", normal.w, normal.x, normal.y, normal.z);
		Novice::ScreenPrintf(0, 120, "mul1     : %.3f, %.3f, %.3f, %.3f", mul1.w, mul1.x, mul1.y, mul1.z);
		Novice::ScreenPrintf(0, 140, "mul2     : %.3f, %.3f, %.3f, %.3f", mul2.w, mul2.x, mul2.y, mul2.z);
		Novice::ScreenPrintf(0, 160, "norm     : %.3f", norm);


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
