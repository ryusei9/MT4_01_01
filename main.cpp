#include <Novice.h>
#include <Vector3.h>
#include <Matrix4x4.h>
#include <cstdint>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <imgui.h>
#include <numbers>
#include <algorithm>
const char kWindowTitle[] = "LE2B_13_サトウ_リュウセイ_MT4_01_01";

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3 translate);

// x軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);

// y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

// z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(const Vector3& v1, const Vector3& v2);

Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
// 行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);
// 内積
float Dot(const Vector3& v1, const Vector3& v2);
// 長さ
float Length(const Vector3& v);
// 正規化
Vector3 Normalize(const Vector3 v);
// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);

Matrix4x4 MakeRotateAngleAxis(Vector3 axis, float angle);
// 4x4行列の数値表示
static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 axis = Normalize({1.0f,1.0f,1.0f});

	float angle = 0.44f;

	Matrix4x4 rotateMatrix = MakeRotateAngleAxis(axis, angle);
	
	
	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

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
		MatrixScreenPrintf(0, 0, rotateMatrix);
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
// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 resultMultiply = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			resultMultiply.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] + m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
		}
	}
	return resultMultiply;
}
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 resultInverse = {};
	float A = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]
		- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]
		+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];
	resultInverse.m[0][0] = (
		m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) / A;
	resultInverse.m[0][1] = (
		-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) / A;
	resultInverse.m[0][2] = (
		m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) / A;
	resultInverse.m[0][3] = (
		-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) / A;
	resultInverse.m[1][0] = (
		-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2]
		+ m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) / A;
	resultInverse.m[1][1] = (
		m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) / A;
	resultInverse.m[1][2] = (
		-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2]
		+ m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) / A;
	resultInverse.m[1][3] = (
		m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) / A;
	resultInverse.m[2][0] = (
		m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1]
		- m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) / A;
	resultInverse.m[2][1] = (
		-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1]
		+ m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) / A;
	resultInverse.m[2][2] = (
		m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1]
		- m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) / A;
	resultInverse.m[2][3] = (
		-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1]
		+ m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) / A;
	resultInverse.m[3][0] = (
		-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1]
		+ m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) / A;
	resultInverse.m[3][1] = (
		m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1]
		- m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) / A;
	resultInverse.m[3][2] = (
		-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1]
		+ m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) / A;
	resultInverse.m[3][3] = (
		m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1]
		- m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) / A;
	return resultInverse;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 resultTransform = {};
	resultTransform.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	resultTransform.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	resultTransform.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];

	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	resultTransform.x /= w;
	resultTransform.y /= w;
	resultTransform.z /= w;

	return resultTransform;
}

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3 translate) {
	Matrix4x4 resultAffineMatrix = {};
	Matrix4x4 resultRotateXYZMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));
	resultAffineMatrix.m[0][0] = scale.x * resultRotateXYZMatrix.m[0][0];
	resultAffineMatrix.m[0][1] = scale.x * resultRotateXYZMatrix.m[0][1];
	resultAffineMatrix.m[0][2] = scale.x * resultRotateXYZMatrix.m[0][2];
	resultAffineMatrix.m[1][0] = scale.y * resultRotateXYZMatrix.m[1][0];
	resultAffineMatrix.m[1][1] = scale.y * resultRotateXYZMatrix.m[1][1];
	resultAffineMatrix.m[1][2] = scale.y * resultRotateXYZMatrix.m[1][2];
	resultAffineMatrix.m[2][0] = scale.z * resultRotateXYZMatrix.m[2][0];
	resultAffineMatrix.m[2][1] = scale.z * resultRotateXYZMatrix.m[2][1];
	resultAffineMatrix.m[2][2] = scale.z * resultRotateXYZMatrix.m[2][2];
	resultAffineMatrix.m[3][0] = translate.x;
	resultAffineMatrix.m[3][1] = translate.y;
	resultAffineMatrix.m[3][2] = translate.z;
	resultAffineMatrix.m[3][3] = 1;
	return resultAffineMatrix;
}

// x軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 resultRotateXMatrix = {};
	resultRotateXMatrix.m[0][0] = 1;
	resultRotateXMatrix.m[1][1] = std::cos(radian);
	resultRotateXMatrix.m[1][2] = std::sin(radian);
	resultRotateXMatrix.m[2][1] = -std::sin(radian);
	resultRotateXMatrix.m[2][2] = std::cos(radian);
	resultRotateXMatrix.m[3][3] = 1;
	return resultRotateXMatrix;
}

// y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 resultRotateYMatrix = {};
	resultRotateYMatrix.m[0][0] = std::cos(radian);
	resultRotateYMatrix.m[0][2] = -std::sin(radian);
	resultRotateYMatrix.m[1][1] = 1;
	resultRotateYMatrix.m[2][0] = std::sin(radian);
	resultRotateYMatrix.m[2][2] = std::cos(radian);
	resultRotateYMatrix.m[3][3] = 1;
	return resultRotateYMatrix;
}

// z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 resultRotateZMatrix = {};
	resultRotateZMatrix.m[0][0] = std::cos(radian);
	resultRotateZMatrix.m[0][1] = std::sin(radian);
	resultRotateZMatrix.m[1][0] = -std::sin(radian);
	resultRotateZMatrix.m[1][1] = std::cos(radian);
	resultRotateZMatrix.m[2][2] = 1;
	resultRotateZMatrix.m[3][3] = 1;
	return resultRotateZMatrix;
}

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 resultPerspectiveFovMatrix = {};
	resultPerspectiveFovMatrix.m[0][0] = (1 / aspectRatio) * (1 / std::tan(fovY / 2));
	resultPerspectiveFovMatrix.m[1][1] = 1 / std::tan(fovY / 2);
	resultPerspectiveFovMatrix.m[2][2] = farClip / (farClip - nearClip);
	resultPerspectiveFovMatrix.m[2][3] = 1;
	resultPerspectiveFovMatrix.m[3][2] = -nearClip * farClip / (farClip - nearClip);
	return resultPerspectiveFovMatrix;
}

// ビューポート変換
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 resultViewPortMatrix = {};
	resultViewPortMatrix.m[0][0] = width / 2.0f;
	resultViewPortMatrix.m[1][1] = -height / 2.0f;
	resultViewPortMatrix.m[2][2] = maxDepth - minDepth;
	resultViewPortMatrix.m[3][0] = left + (width / 2);
	resultViewPortMatrix.m[3][1] = top + (height / 2);
	resultViewPortMatrix.m[3][2] = minDepth;
	resultViewPortMatrix.m[3][3] = 1;
	return resultViewPortMatrix;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 resultCross = {};
	resultCross.x = v1.y * v2.z - v1.z * v2.y;
	resultCross.y = v1.z * v2.x - v1.x * v2.z;
	resultCross.z = v1.x * v2.y - v1.y * v2.x;
	return resultCross;
}

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 AddResult = {};
	AddResult.x = v1.x + v2.x;
	AddResult.y = v1.y + v2.y;
	AddResult.z = v1.z + v2.z;
	return AddResult;
}
// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 SubtractResult = {};
	SubtractResult.x = v1.x - v2.x;
	SubtractResult.y = v1.y - v2.y;
	SubtractResult.z = v1.z - v2.z;
	return SubtractResult;
}

Vector3 Multiply(const Vector3& v1, const Vector3& v2) {
	return { v1.x * v2.x,v1.y * v2.y,v1.z * v2.z };
}

// 行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 resultAdd = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			resultAdd.m[j][i] = m1.m[j][i] + m2.m[j][i];
		}
	}
	return resultAdd;
}
// 行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 resultSubtract = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			resultSubtract.m[j][i] = m1.m[j][i] - m2.m[j][i];
		}
	}
	return resultSubtract;
}

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 MultiplyResult = {};
	MultiplyResult.x = scalar * v.x;
	MultiplyResult.y = scalar * v.y;
	MultiplyResult.z = scalar * v.z;
	return MultiplyResult;
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {
	float DotResult = {};
	DotResult = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return DotResult;
}
// 長さ
float Length(const Vector3& v) {
	float LengthResult = {};
	LengthResult = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return LengthResult;
}

// 正規化
Vector3 Normalize(const Vector3 v) {
	Vector3 NormalizeResult = {};
	float LengthResult = {};
	LengthResult = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	NormalizeResult.x = v.x / LengthResult;
	NormalizeResult.y = v.y / LengthResult;
	NormalizeResult.z = v.z / LengthResult;
	return NormalizeResult;
}

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	return Multiply(Dot(v1, v2) / powf(Length(v2), 2), v2);
}

Matrix4x4 MakeRotateAngleAxis(Vector3 axis, float angle)
{
	Matrix4x4 rotateResult = {};
	rotateResult.m[0][0] = (axis.x * axis.x) * (1 - std::cos(angle)) + std::cos(angle);
	rotateResult.m[0][1] = axis.x * axis.y * (1 - std::cos(angle)) + axis.z * std::sin(angle);
	rotateResult.m[0][2] = axis.x * axis.z * (1 - std::cos(angle)) - axis.y * std::sin(angle);
	rotateResult.m[1][0] = axis.x * axis.y * (1 - std::cos(angle)) - axis.z * std::sin(angle);
	rotateResult.m[1][1] = (axis.y * axis.y) * (1 - std::cos(angle)) + std::cos(angle);
	rotateResult.m[1][2] = axis.y * axis.z * (1 - std::cos(angle)) + axis.x * std::sin(angle);
	rotateResult.m[2][0] = axis.x * axis.z * (1 - std::cos(angle)) + axis.y * std::sin(angle);
	rotateResult.m[2][1] = axis.y * axis.z * (1 - std::cos(angle)) - axis.x * std::sin(angle);
	rotateResult.m[2][2] = (axis.z * axis.z) * (1 - std::cos(angle)) + std::cos(angle);
	rotateResult.m[3][3] = 1;
	return rotateResult;
}
// 4x4行列の数値表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix) {
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "%6.03f", matrix.m[row][column]);
		}
	}
}