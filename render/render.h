#pragma once
#include "log_debug.h"
#include "math_tools.h"

// 计算投影矩阵
Matrix4f Projection(float fov, float aspect, float near, float far) // 视场角 宽高比 近平面 远平面
{
    float tanHalfFov = tan(fov / 2);
    float range = far - near;

    Matrix4f proj = Eye4();
    proj.data[0][0] = 1 / (aspect * tanHalfFov);
    proj.data[1][1] = 1 / tanHalfFov;
    proj.data[2][2] = -(far + near) / range;
    proj.data[2][3] = -1;
    
    return proj;
}


// 投影变换
Vector3f VertexProj(Vector4f v, Matrix4f& proj)
{
    Vector4f vProj = proj * v;
    if (vProj.data[3][0] != 0) {
        vProj = vProj * (1 / vProj.data[3][0]);
    }
    return Vector3f(vProj.data[0][0], vProj.data[1][0], vProj.data[2][0]);
}

// 仿射变换

// 平移变换

// 旋转变换

// 缩放变换

// 透视除法

// 
