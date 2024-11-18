#pragma once
#include "log_debug.h"
#include "math_tools.h"
#include <vector>

constexpr float PI = 3.1416;

// 计算投影矩阵
Matrix4f Projection(float fov, float aspect, float near, float far) // 视场角 宽高比 近平面 远平面
{
    float fovRad = fov / 180.0 * PI;
    float tanHalfFov = tan(fovRad / 2);

    Matrix4f proj = Eye4();
    proj.data[0][0] = -1.0 / (aspect * tanHalfFov);
    proj.data[1][1] = -1.0 / tanHalfFov;
    proj.data[2][2] = -1.0 * (far + near) / (near - far);
    proj.data[2][3] = 2.0 * near * far / (near - far);
    proj.data[3][2] = 1.0;
    proj.data[3][3] = 0.0;
    
    return proj;
}

// 计算投影平面上任一点反推到模型上的z轴坐标
float Vz(const Vector2f v, const Vector3f t[3], const Vector3f fNormal)
{
    // 找出距离v最远的顶点
    int idx = 0;
    float dv = -1;
    for (int i = 0; i < 3; i++) {
        Vector2f vTmp = v - Vector2f(t[i].data[0][0], t[i].data[0][1]);
        float dvTmp = vTmp.Mod();
        if (dvTmp > dv) {
            dv = dvTmp;
            idx = i;
       }
    }
    // 插值计算v点z坐标
    float vtMax = Dot(Vector2f(fNormal.data[0][0], fNormal.data[1][0]), v - Vector2f(t[idx].data[0][0], t[idx].data[0][1]));
    float vz = t[idx].data[2][0] - vtMax / fNormal.data[2][0];

    return vz;
}

// 判断点是否在三角形内
bool IsPointInPlane(Vector3f v3d, Vector3f t[3], Vector3f fNormal) // v为点，t为三角形的顶点
{
    // 计算点v与三个顶点的方向向量
    Vector3f va = t[0] - v3d;
    Vector3f vb = t[1] - v3d;
    Vector3f vc = t[2] - v3d;

    // 计算顶点形成的方向向量与顶点与v形成的方向向量的叉积
    Vector3f vaxvb = Vector3f::Cross(va, vb);
    Vector3f vbxvc = Vector3f::Cross(vb, vc);
    Vector3f vcxva = Vector3f::Cross(vc, va);

    // 判断三个叉积是否同向
    if ((Dot(vaxvb, vbxvc) > 0) && (Dot(vbxvc, vcxva) > 0) && (Dot(vcxva, vaxvb) > 0)) {
        return true; // 同向说明点在三角形内
    } else {
        return false;// 不同向说明点在三角形外
    }
}

// 投影变换
std::vector<Vector3f> VertexProj(std::vector<Vector4f> v, Matrix4f& proj, int width, int height)
{
    std::vector<Vector3f> vProj(v.size(), Vector3f());
    for (unsigned int i = 0; i < v.size(); i++) {
        Vector4f vTmp = proj * v[i];
        vTmp = vTmp.Normal();
        if (vTmp.data[3][0] != 0) {
            vTmp = vTmp * (1 / vTmp.data[3][0]);
        }
        vProj[i] = Vector3f((vTmp.data[0][0] + 0.5) * float(height),
                            (vTmp.data[1][0] + 0.5) * float(width),
                             vTmp.data[2][0]);
    }
    
    return vProj;
}

// 平移变换
void ShiftbyDir(Vector4f &v, Vector3f dir)
{
    v = v + Vector4f(dir.data[0][0], dir.data[1][0], dir.data[2][0], 0);
}

// 旋转变换
void RotatebyAxis(Vector4f &v, Vector4f axis, float angle) // angle为角度值
{
    float radAngle = angle * PI / 180;
    float cosA = cos(angle);
    float sinA = sin(angle);

    Matrix4f I = Eye4();
}

// 缩放变换

// 透视除法

// 
