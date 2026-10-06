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

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Spherical s{ 6.0f, 0.f, -std::numbers::pi_v<float> / 2.f };

	

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
		Vector3 offset = ToCartesian(s);
		Vector3 target = Vector3(0.f, 0.f, 0.f);
		Vector3 eye = target + offset;

		Vector3 woorldUp = Vector3(0.f, 1.f, 0.f);
		Vector3 forward = (target - eye).Normalized();
		Vector3 right = Vector3::Cross(woorldUp, forward).Normalized();
		Vector3 up = Vector3::Cross(forward, right).Normalized();

		Matrix4x4 cameraMatrix{
			right.x	, right.y	, right.z	, 0.f,
			up.x	, up.y		, up.z		, 0.f,
			forward.x, forward.y, forward.z	, 0.f,
			eye.x	, eye.y		, eye.z		, 1.f
		};

		const float limit = std::numbers::pi_v<float> / 2.f - 0.01f;
		s.radius = max(0.1f, s.radius);
		s.theta = std::clamp(s.theta, -limit, limit);
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		ImGui::Begin("Spherical Coordinates");
		ImGui::Text("Target: (0,0,0) / +Y up / Camera +Z forward");
		ImGui::Separator();

		ImGui::InputFloat("Radius", &s.radius, 0.01f, 0.01f, "%.3f");
		ImGui::InputFloat("Theta", &s.theta, 0.01f, 0.01f, "%.3f");
		ImGui::InputFloat("Phi", &s.phi, 0.01f, 0.01f, "%.3f");
		ImGui::Separator();

		ImGui::Text("Spherical: Radius = %.3f, Theta = %.3f, Phi = %.3f", s.radius, s.theta, s.phi);
		ImGui::Text("Cartesian: X = %.3f, Y = %.3f, Z = %.3f", offset.x, offset.y, offset.z);
		ImGui::Separator();

		ImGui::Text("Camera Matrix:");
		ImGui::Text("%8.3f\t\t %8.3f\t\t %8.3f\t\t %8.3f", cameraMatrix.m[0][0], cameraMatrix.m[0][1], cameraMatrix.m[0][2], cameraMatrix.m[0][3]);
		ImGui::Text("%8.3f\t\t %8.3f\t\t %8.3f\t\t %8.3f", cameraMatrix.m[1][0], cameraMatrix.m[1][1], cameraMatrix.m[1][2], cameraMatrix.m[1][3]);
		ImGui::Text("%8.3f\t\t %8.3f\t\t %8.3f\t\t %8.3f", cameraMatrix.m[2][0], cameraMatrix.m[2][1], cameraMatrix.m[2][2], cameraMatrix.m[2][3]);
		ImGui::Text("%8.3f\t\t %8.3f\t\t %8.3f\t\t %8.3f", cameraMatrix.m[3][0], cameraMatrix.m[3][1], cameraMatrix.m[3][2], cameraMatrix.m[3][3]);

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
