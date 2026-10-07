#include <Novice.h>
#include <numbers>
#include <imgui.h>

#include "Vector2.hpp"
#include "matrix4x4.hpp"
#include <algorithm>

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
	
	int r1 = 12;
	int r2 = 20;

	float deltaTime = 1.0f / 60.0f;
	
	Vector2 circlePosition = { 640.0f, 360.0f };
	float speed = 10.f;

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
		
		int x, y;
		Novice::GetMousePosition(&x, &y);

		circlePosition.x += (speed * deltaTime) * (x - circlePosition.x) ;
		circlePosition.y += (speed * deltaTime) * (y - circlePosition.y);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		ImGui::Begin("interpolation controller");
		ImGui::Text("Target: Mouse Position (Red Circle)");
		ImGui::Separator();
		ImGui::SliderFloat("Speed", &speed, 0.1f, 10.0f, "%.3f");
		ImGui::Text("Mouse Position: (%d, %d)", x, y);
		ImGui::Text("circle Position: (%.2f, %.2f)", circlePosition.x, circlePosition.y);
		ImGui::Text("Distance: %.2f", std::sqrt((x - circlePosition.x) * (x - circlePosition.x) + (y - circlePosition.y) * (y - circlePosition.y)));
		ImGui::Separator();

		ImGui::End();

		Novice::DrawLine(static_cast<int>(circlePosition.x), static_cast<int>(circlePosition.y), x, y, 0xFFFFFFFF);
		Novice::DrawEllipse(static_cast<int>(circlePosition.x), static_cast<int>(circlePosition.y), r2, r2, 0.0f, 0xFFFF00FF, kFillModeSolid);
		Novice::DrawEllipse(x, y, r1, r1, 0.0f, RED, kFillModeSolid);
		

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
