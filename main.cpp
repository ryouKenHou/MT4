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

float f(float x) {
	return 0.125f * x * x * x + 0.25f * x * x;
}

float fPrime(float x) {
	return 0.375f * x * x + 0.5f * x;
}

Vector2 Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t) {
	// Quadratic Bezier formula: B(t) = (1-t)^2 * P0 + 2(1-t)t * P1 + t^2 * P2
	float u = 1.0f - t;
	return u * u * p0 + 2 * u * t * p1 + t * t * p2;
}

Vector2 BezierDerivative(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t) {
	float u = 1.0f - t;
	return 2 * u * (p1 - p0) + 2 * t * (p2 - p1);
}

int worldToScreenX(float worldX) {
	return static_cast<int>(worldX * 240.0f + 440.f);
}

int worldToScreenY(float worldY) {
	return static_cast<int>(-worldY * 240.0f + 600.0f);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	
	float t = 0.0f;
	Vector2 position = { 0.0f, 0.0f };

	Vector2 p0 = { 200.0f, 570.0f };
	Vector2 p1 = { 800.f, 70.f };
	Vector2 p2 = { 1080.f, 570.f };

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

		position = Bezier(p0, p1, p2, t);
		Vector2 forward = BezierDerivative(p0, p1, p2, t);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		float segmentCount = 20.0f;
		float step = 1.0f / segmentCount;
		for (float x = step; x <= 1.0f; x += step) {
			Vector2 p11 = Bezier(p0, p1, p2, x - step);
			Vector2 p22 = Bezier(p0, p1, p2, x);
			Novice::DrawLine((int)p11.x, (int)p11.y, (int)p22.x, (int)p22.y, WHITE);
		}

		Vector2 start = { position.x , position.y  };

		Vector2 normalizedForward = Vector2::Normalized(forward);
		Vector2 end = { position.x + normalizedForward.x * 80.f, position.y + normalizedForward.y * 80.f };

		Novice::DrawEllipse((int)start.x, (int)start.y, 8, 8, 0.0f, RED, kFillModeSolid);
		Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, RED);

		ImGui::Begin("jump");
		ImGui::SliderFloat("t", &t, 0.0f, 1.0f);
		ImGui::SliderFloat("p1", &p1.x, 400.0f, 880.0f);
		ImGui::SliderFloat("p2", &p1.y, 20.0f, 570.0f);
		ImGui::Text("B(t): (%.3f, %.3f)", position.x, position.y);
		ImGui::Text("B'(t): (%.3f, %.3f)", forward.x, forward.y);
		ImGui::End();	

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
