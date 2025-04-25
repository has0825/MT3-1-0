#include <Novice.h>
#include <cmath>
#include <cstdio>

const char kWindowTitle[] = "LC1B_12_シミズグチ_ハル";
const int kRowHeight = 20;

struct Vector3 {
    float x, y, z;
};

struct Matrix4x4 {
    float m[4][4];
};

// --- 行列生成関数群 ---
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
    Matrix4x4 result = {};
    float yScale = 1.0f / tanf(fovY / 2.0f);
    float xScale = yScale / aspectRatio;

    result.m[0][0] = xScale;
    result.m[1][1] = yScale;
    result.m[2][2] = farClip / (farClip - nearClip);
    result.m[2][3] = 1.0f;
    result.m[3][2] = -nearClip * farClip / (farClip - nearClip);
    return result;
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
    Matrix4x4 result = {};
    result.m[0][0] = 2.0f / (right - left);
    result.m[1][1] = 2.0f / (top - bottom);
    result.m[2][2] = 1.0f / (farClip - nearClip);
    result.m[3][0] = (left + right) / (left - right);
    result.m[3][1] = (top + bottom) / (bottom - top);
    result.m[3][2] = nearClip / (nearClip - farClip);
    result.m[3][3] = 1.0f;
    return result;
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
    Matrix4x4 result = {};
    result.m[0][0] = width / 2.0f;
    result.m[1][1] = -height / 2.0f;
    result.m[2][2] = maxDepth - minDepth;
    result.m[3][0] = left + width / 2.0f;
    result.m[3][1] = top + height / 2.0f;
    result.m[3][2] = minDepth;
    result.m[3][3] = 1.0f;
    return result;
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
    Novice::ScreenPrintf(x, y, "%s:", label);
    for (int i = 0; i < 4; i++) {
        Novice::ScreenPrintf(x, y + (i + 1) * kRowHeight,
            "%.2f %.2f %.2f %.2f",
            matrix.m[i][0], matrix.m[i][1], matrix.m[i][2], matrix.m[i][3]);
    }
}

// --- メイン関数 ---
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Novice::Initialize(kWindowTitle, 1280, 720);
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    // 行列の生成
    Matrix4x4  orthographic = MakeOrthographicMatrix(-160.f, 160.f, 200.0f, 300.0f, 0.0f, 1000.0f);
    Matrix4x4  perspective = MakePerspectiveFovMatrix(0.63f, 1.33f, 0.1f, 1000.0f);
    Matrix4x4 viewport = MakeViewportMatrix(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f);

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        // 表示
        MatrixScreenPrintf(0, 0, orthographic, "Orthographic");
        MatrixScreenPrintf(0, 100, perspective, "Perspective");
        MatrixScreenPrintf(0, 200, viewport, "Viewport");

        Novice::EndFrame();

        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
    }

    Novice::Finalize();
    return 0;
}


