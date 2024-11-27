#pragma once
#include "log_debug.h"
#include "math_tools.h"
#include <vector>

constexpr double PI = 3.1416;

// 计算投影矩阵
Matrix4d Projection(double fov, double aspect, double near, double far) // 视场角 宽高比 近平面 远平面
{
    double fovRad = fov / 180.0 * PI;
    double tanHalfFov = tan(fovRad / 2);

    Matrix4d proj = Eye4();
    proj.data[0][0] = -1.0 / (aspect * tanHalfFov);
    proj.data[1][1] = -1.0 / tanHalfFov;
    proj.data[2][2] = -1.0 * (far + near) / (near - far);
    proj.data[2][3] = 2.0 * near * far / (near - far);
    proj.data[3][2] = 1.0;
    proj.data[3][3] = 0.0;
    
    return proj;
}

// 计算投影平面上任一点反推到模型上的z轴坐标
double Vz(const Vector2d v, const Vector3d t[3], const Vector3d fNormal)
{
    // 找出距离v最远的顶点
    int idx = 0;
    double dv = -1;
    for (int i = 0; i < 3; i++) {
        Vector2d vTmp = v - Vector2d(t[i].data[0][0], t[i].data[0][1]);
        double dvTmp = vTmp.Mod();
        if (dvTmp > dv) {
            dv = dvTmp;
            idx = i;
       }
    }
    // 插值计算v点z坐标
    double vtMax = Dot(Vector2d(fNormal.data[0][0], fNormal.data[1][0]), v - Vector2d(t[idx].data[0][0], t[idx].data[0][1]));
    double vz = t[idx].data[2][0] - vtMax / fNormal.data[2][0];

    return vz;
}

// 判断点是否在三角形内
// 以点v到三角形三个顶点a、b、c形成向量，z轴坐标为0，计算三个向量两两之间的叉积，判断是否同向
// z轴为0时，叉积为平行于z轴的向量，因此只需要判断向量z轴分量是否同号即可
// 为了简化计算，将向量退化为2维，并以矩阵行列式的方式计算z轴分量
bool IsPointInPlane(Vector2d v, Vector3d t[3]) // v为点，t为三角形的顶点
{
    // 构造点v与三个顶点形成的向量的叉积矩阵
    double avb[2][2] = {{t[0].data[0][0] - v.data[0][0], t[0].data[1][0] - v.data[1][0]},
                       {t[1].data[0][0] - v.data[0][0], t[1].data[1][0] - v.data[1][0]}};
    double bvc[2][2] = {{t[1].data[0][0] - v.data[0][0], t[1].data[1][0] - v.data[1][0]},
                       {t[2].data[0][0] - v.data[0][0], t[2].data[1][0] - v.data[1][0]}};
    double cva[2][2] = {{t[2].data[0][0] - v.data[0][0], t[2].data[1][0] - v.data[1][0]},
                       {t[0].data[0][0] - v.data[0][0], t[0].data[1][0] - v.data[1][0]}};

    Matrix2d ab = Matrix2d(avb);
    Matrix2d bc = Matrix2d(bvc);
    Matrix2d ca = Matrix2d(cva);
    
    // 计算叉积的z轴分量
    double detAb = ab.Det();
    double detBc = bc.Det();
    double detCa = ca.Det();

    // 判断三个叉积是否同向，即z轴分量是否同号
    if ((detAb * detBc) > 0 && (detBc * detCa) > 0) {
        return true; // 同向说明点在三角形内
    } else {
        return false;// 不同向说明点在三角形外
    }
}

// 投影变换
std::vector<Vector3d> VertexProj(std::vector<Vector4d> v, Matrix4d& proj, int width, int height)
{
    std::vector<Vector3d> vProj(v.size(), Vector3d());
    for (unsigned int i = 0; i < v.size(); i++) {
        Vector4d vTmp = proj * v[i];
        vTmp = vTmp.Normal();
        if (vTmp.data[3][0] != 0) {
            vTmp = vTmp * (1 / vTmp.data[3][0]);
        }
        vProj[i] = Vector3d((vTmp.data[0][0] + 0.5) * double(height),
                            (vTmp.data[1][0] + 0.5) * double(width),
                             vTmp.data[2][0]);
    }
    
    return vProj;
}

// 平移变换
void ShiftbyDir(Vector4d &v, Vector3d dir)
{
    v = v + Vector4d(dir.data[0][0], dir.data[1][0], dir.data[2][0], 0);
}

// 旋转变换
std::vector<Vector4d> RotatebyAxis(std::vector<Vector4d> v, Vector3d axis, double angle) // angle为角度值，axis为通过原点的直线的方向向量
{
    double radAngle = angle * PI / 180;
    double cosa = cos(angle);
    double sina = sin(angle);
    double k = 1 - cosa;

    Vector3d vAxis = axis.Normal();
    double ax = vAxis.data[0][0];
    double ay = vAxis.data[1][0];
    double az = vAxis.data[2][0];
    Matrix4d rot = Eye4();
    rot.data[0][0] = ax*ax*k + cosa;
    rot.data[0][1] = ax*ay*k - az * sina;
    rot.data[0][2] = ax*az*k + ay * sina;
    rot.data[0][3] = 0;
    rot.data[1][0] = ax*ay*k + az * sina;
    rot.data[1][1] = ay*ay*k + cosa;
    rot.data[1][2] = ay*az*k - ax * sina;
    rot.data[1][3] = 0;
    rot.data[2][0] = ax*az*k - ay * sina;
    rot.data[2][1] = ay*az*k + ax * sina;
    rot.data[2][2] = az*az*k + cosa;
    rot.data[2][3] = 0;

    std::vector<Vector4d> vRot(v.size(), Vector4d());
    for (int i = 0; i < v.size(); i++) {
        vRot[i] = rot * v[i];
    }
    return vRot;
}

// 缩放变换

// 透视除法

// 
