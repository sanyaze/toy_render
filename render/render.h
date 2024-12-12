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
    if ((detAb * detBc) >= 0 && (detBc * detCa) >= 0) {
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
std::vector<Vector4d> ShiftbyDir(std::vector<Vector4d> v, Vector3d dir, double distance)
{
    Matrix4d shift = Eye4();
    Vector3d dist = dir.Normal() * distance;
    shift.data[0][3] = dist.data[0][0];
    shift.data[1][3] = dist.data[1][0];
    shift.data[2][3] = dist.data[2][0];

    std::vector<Vector4d> vShift(v.size(), Vector4d());
    for (int i = 0; i < v.size(); i++) {
        vShift[i] = shift * v[i];
    }
    return vShift;
}

// 旋转变换
std::vector<Vector4d> RotatebyAxis(std::vector<Vector4d> v, Vector3d axis, double angle) // angle为角度值，axis为通过原点的直线的方向向量
{
    double radAngle = fmod(angle, 360.) * PI / 180;
    double cosa = cos(radAngle);
    double sina = sin(radAngle);
    double k = 1 - cosa;

    Vector3d vAxis = axis.Normal();
    double ax = vAxis.data[0][0];
    double ay = vAxis.data[1][0];
    double az = vAxis.data[2][0];

    /* 罗德里格斯矩阵, R(a, axis) = cos(a)I + (1 - cos(a))(axis * trans(axis)) + sin(a)[0, -az,  ay,
     *                                                                                az,  0,  -ax,
     *                                                                                -ay, ax,   0] */
    Matrix4d rot = Eye4();
    rot.data[0][0] = cosa + ax * ax * k;
    rot.data[0][1] = ax * ay * k - az * sina;
    rot.data[0][2] = ax * az * k + ay * sina;
    rot.data[1][0] = ax * ay * k + az * sina;
    rot.data[1][1] = cosa + ay * ay * k;
    rot.data[1][2] = ay * az * k - ax * sina;
    rot.data[2][0] = ax * az * k - ay * sina;
    rot.data[2][1] = ay * az * k + ax * sina;
    rot.data[2][2] = cosa + az * az * k;

    std::vector<Vector4d> vRot(v.size(), Vector4d());
    for (int i = 0; i < v.size(); i++) {
        vRot[i] = rot * v[i];
    }
    return vRot;
}

// 渲染
void Render(std::vector<Vector4d> vecCube, std::vector<std::vector<int>> face, std::vector<std::vector<int>> texture,
            Matrix4d proj, int width, int height, BmpCreater &bmp)
{
    // 顶点投影
    std::vector<Vector3d> vProj = VertexProj(vecCube, proj, width, height);
    LOG_INFO("start render\n");

    // 面元着色
    std::vector<std::vector<double>> zBuffer(width, std::vector<double>(height, -INFINITY));
    // 像素矩阵每行每列多两个点，方便后处理流程对边界的处理
    std::vector<std::vector<int>> pixel_r(width + 2, std::vector<int>(height + 2, 0));
    std::vector<std::vector<int>> pixel_g(width + 2, std::vector<int>(height + 2, 0));
    std::vector<std::vector<int>> pixel_b(width + 2, std::vector<int>(height + 2, 0));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Vector2d v = Vector2d(double(j), double(i));
            for (unsigned int k = 0; k < 6; k++) {
                Vector3d t[2][3] = { { vProj[face[k][0]], vProj[face[k][1]], vProj[face[k][2]] },
                                     { vProj[face[k][0]], vProj[face[k][2]], vProj[face[k][3]] } };
                // 平面分为的两个三角面法向量一样，计算一次即可
                Vector3d fNormal = Vector3d::Cross(t[0][1] - t[0][0], t[0][2] - t[0][0]);
                for (int l = 0; l < 2; l++) {
                    // 平面与z坐标轴平行，在XoY平面上的投影为一条直线, 不处理
                    // 平面背对投影平面，即法向量朝向z轴反方向，不处理
                    if (fNormal.data[2][0] < 0.00001) {
                        continue;
                    }
                    if (IsPointInPlane(v, t[l])) {
                        // 计算z轴坐标
                        double vz = Vz(v, t[i], fNormal.Normal());
                        if (vz > zBuffer[i][j]) {
                            zBuffer[i][j] = vz;
                            // 两侧的点为记录像素的边界，因此索引从1开始
                            pixel_r[i+1][j+1] = texture[k][0];
                            pixel_g[i+1][j+1] = texture[k][1];
                            pixel_b[i+1][j+1] = texture[k][2];
                        }
                    }
                }
            }
            int progress = (i * width + j) * 100 / (width * height);
        }
    }

    // 后处理，抗锯齿
    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= width; j++) {
            // 每9个像素合成一个像素点
            int r = 0;
            int g = 0;
            int b = 0;
            for (int pi = -1; pi <= 1; pi++) {
                for (int pj = -1; pj <= 1; pj++) {
                    r += pixel_r[i + pi][j + pj];
                    g += pixel_g[i + pi][j + pj];
                    b += pixel_b[i + pi][j + pj];
                }
            }
            bmp.SetPixel(j - 1, i - 1, r / 9, g / 9, b / 9);
            int progress = (i * width + j) * 100 / (width * height);
        }
    }
}

// 缩放变换

// 透视除法

// 
