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

	static Vector2 Normalized(const Vector2& v) {
		float length = std::sqrt(v.x * v.x + v.y * v.y);
		if (length == 0) {
			return Vector2{ 0, 0 }; // Avoid division by zero
		}
		return Vector2{ v.x / length, v.y / length };
	}
};

float f(float x) {
	return 0.125f * x * x * x + 0.25f * x * x;
}

float fPrime(float x) {
	return 0.375f * x * x + 0.5f * x;
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
	
	float posX = 0.0f;
	Vector2 position = { 0.0f, 0.0f };

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
		
		if(keys[DIK_D] != 0) {
			posX += 0.1f;
		}
		if (keys[DIK_A] != 0) {
			posX -= 0.1f;
		}

		posX = std::clamp(posX, 0.0f, 2.0f);
		position = { posX, f(posX) };
		float slope = fPrime(posX);
		Vector2 forward = Vector2::Normalized({ 1.0f, slope });

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		float segmentCount = 50.0f;
		float step = 2.0f / segmentCount;
		for (float x = step; x <= 2; x += step) {
			float x1 = x - step;
			float y1 = f(x1);
			float x2 = x;
			float y2 = f(x2);
			Novice::DrawLine(worldToScreenX(x1), worldToScreenY(y1), worldToScreenX(x2), worldToScreenY(y2), WHITE);
		}

		Novice::DrawLine(worldToScreenX(-1000), worldToScreenY(0), worldToScreenX(1000), worldToScreenY(0), WHITE);
		Novice::DrawLine(worldToScreenX(2), worldToScreenY(0), worldToScreenX(2), worldToScreenY(f(2)), WHITE);

		Vector2 start = { position.x - forward.x * 0.1f, position.y - forward.y * 0.1f };
		Vector2 end = { position.x + forward.x * 0.1f, position.y + forward.y * 0.1f };

		Novice::DrawEllipse(worldToScreenX(start.x), worldToScreenY(start.y), 8, 8, 0.0f, RED, kFillModeSolid);
		Novice::DrawEllipse(worldToScreenX(end.x), worldToScreenY(end.y), 8, 8, 0.0f, RED, kFillModeSolid);
		Novice::DrawLine(worldToScreenX(start.x), worldToScreenY(start.y), worldToScreenX(end.x), worldToScreenY(end.y), RED);

		ImGui::Begin("jump");
		ImGui::Text("A/D: move");
		ImGui::Text("X: %.3f", posX);
		ImGui::Text("f'(x): %.3f", fPrime(posX));
		ImGui::Text("forward: (%.3f, %.3f)", forward.x, forward.y);
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
