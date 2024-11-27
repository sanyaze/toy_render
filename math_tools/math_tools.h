#pragma once
#include <cmath>
#include "log_debug.h"

// 矩阵按照2，3，4三个维度创建类型
class Matrix2d
{
public:
    double data[2][2];
    unsigned int row = 2;
    unsigned int col = 2;
    Matrix2d() : data{ {  0.0f, 0.0f },
                       {  0.0f, 0.0f } } {}
    Matrix2d(double a[2][2]) : data{ { a[0][0], a[0][1] },
                                    { a[1][0], a[1][1] } } {}
    
    double Det()
    {
        return data[0][0] * data[1][1] - data[0][1] * data[1][0];
    }
};

class Matrix3d
{
public:
    double data[3][3];
    unsigned int row = 3;
    unsigned int col = 3;
    Matrix3d() : data{ {  0.0f, 0.0f, 0.0f },
                       {  0.0f, 0.0f, 0.0f },
                       {  0.0f, 0.0f, 0.0f } } {}
    Matrix3d(double a[3][3]) : data{ { a[0][0], a[0][1], a[0][2] },
                                    { a[1][0], a[1][1], a[1][2] },
                                    { a[2][0], a[2][1], a[2][2] } } {}
    
    Matrix2d SubMat(int r, int c) {
        Matrix2d subMat;

        int rCnt = 0;
        int cCnt = 0;
        for (unsigned int i = 0; i < row; i++) {
            if (i == r) {
                continue;
            }
            for (unsigned int j = 0; j < col; j++) {
                if (j == c) {
                    continue;
                }
                subMat.data[rCnt][cCnt] = data[i][j];
                cCnt++;
            }
            rCnt++;
        }

        return subMat;
    }

    double Det()
    {
        double r = 0;
        for (unsigned int i = 0; i < row; i++) {
            Matrix2d subMat = SubMat(i, 0);
            r += data[i][0] * subMat.Det() * (i % 2 == 0 ? 1 : -1);
        }
        return r;
    }
};

class Matrix4d
{
public:
    double data[4][4];
    unsigned int row = 4;
    unsigned int col = 4;
    Matrix4d() : data{ {  0.0f, 0.0f, 0.0f, 0.0f },
                       {  0.0f, 0.0f, 0.0f, 0.0f },
                       {  0.0f, 0.0f, 0.0f, 0.0f },
                       {  0.0f, 0.0f, 0.0f, 0.0f } } {}
    Matrix4d(double a[4][4]) : data{ { a[0][0], a[0][1], a[0][2], a[0][3] },
                                    { a[1][0], a[1][1], a[1][2], a[1][3] },
                                    { a[2][0], a[2][1], a[2][2], a[2][3] },
                                    { a[3][0], a[3][1], a[3][2], a[3][3] } } {}
    
    Matrix3d SubMat(int r, int c)
    {
        Matrix3d subMat;

        int rCnt = 0;
        int cCnt = 0;
        for (unsigned int i = 0; i < row; i++) {
            if (i == r) {
                continue;
            }
            for (unsigned int j = 0; j < col; j++) {
                if (j == c) {
                    continue;
                }
                subMat.data[rCnt][cCnt] = data[i][j];
                cCnt++;
            }
            rCnt++;
        }

        return subMat;
    }

    double Det()
    {
        double r = 0;
        for (unsigned int i = 0; i < row; i++) {
            Matrix3d subMat = SubMat(i, 0);
            r += data[i][0] * subMat.Det() * (i % 2 == 0 ? 1 : -1);
        }
        return r;
    }
};

// 向量也按照2，3，4三个维度创建类型
class Vector2d
{
public:
    double data[2][1];
    unsigned int row = 2;
    unsigned int col = 1;
    Vector2d() : data{ 0.0f, 0.0f } {}
    Vector2d(double a[2]) : data{ a[0], a[1] } {}
    Vector2d(double x, double y) : data{ x, y } {}

    double Mod()
    {
        return sqrt(data[0][0] * data[0][0] + data[1][0] * data[1][0]);
    }

    Vector2d Normal()
    {
        if (Mod() == 0.0f) {
            return Vector2d(0.0f, 0.0f);
        } else {
            return Vector2d(data[0][0] / Mod(), data[1][0] / Mod());
        }
    }
};

class Vector3d
{
public:
    double data[3][1];
    unsigned int row = 3;
    unsigned int col = 1;
    Vector3d() : data{ 0.0f, 0.0f, 0.0f } {}
    Vector3d(double a[3]) : data{ a[0], a[1], a[2] } {}
    Vector3d(double x, double y, double z) : data{ x, y, z } {}

    // 向量外积，只在三维成立
    static Vector3d Cross(const Vector3d v1, const Vector3d v2) {
        Vector3d r = Vector3d(v1.data[1][0] * v2.data[2][0] - v1.data[2][0] * v2.data[1][0],
                              v1.data[2][0] * v2.data[0][0] - v1.data[0][0] * v2.data[2][0],
                              v1.data[0][0] * v2.data[1][0] - v1.data[1][0] * v2.data[0][0] );
        return r;
    }

    double Mod()
    {
        return sqrt(data[0][0] * data[0][0] + data[1][0] * data[1][0] + data[2][0] * data[2][0]);
    }

    Vector3d Normal()
    {
        if (Mod() == 0.0f) {
            return Vector3d(0.0f, 0.0f, 0.0f);
        } else {
            return Vector3d(data[0][0] / Mod(), data[1][0] / Mod(), data[2][0] / Mod());
        }
    }
};

class Vector4d
{
public:
    double data[4][1];
    unsigned int row = 4;
    unsigned int col = 1;
    Vector4d() : data{ 0.0f, 0.0f, 0.0f, 0.0f } {}
    Vector4d(double a[4]) : data{ a[0], a[1], a[2], a[3] } {}
    Vector4d(double x, double y, double z, double w) : data{ x, y, z, w } {}

    double Mod()
    {
        return sqrt(data[0][0] * data[0][0] + data[1][0] * data[1][0] + data[2][0] * data[2][0] + data[3][0] * data[3][0]);
    }

    Vector4d Normal()
    {
        if (Mod() == 0.0f) {
            return Vector4d(0.0f, 0.0f, 0.0f, 0.0f);
        } else {
            return Vector4d(data[0][0] / Mod(), data[1][0] / Mod(), data[2][0] / Mod(), data[3][0] / Mod());
        }
    }
};

// 特殊矩阵生成
// 单位矩阵
Matrix2d Eye2()
{
    Matrix2d r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return r;
}

Matrix3d Eye3()
{
    Matrix3d r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return r;
}

Matrix4d Eye4()
{
    Matrix4d r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return r;
}

// 全1矩阵
Matrix2d Ones2()
{
    Matrix2d r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = 1.0;
        }
    }
    return r;
}

Matrix3d Ones3()
{
    Matrix3d r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = 1.0;
        }
    }
    return r;
}

Matrix4d Ones4()
{
    Matrix4d r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = 1.0;
        }
    }
    return r;
}

// 矩阵向量计算
template <typename T>
T operator+(const T t1, const T t2)
{
    T r;
    for (unsigned int i = 0; i < t1.row; i++) {
        for (unsigned int j = 0; j < t1.col; j++) {
            r.data[i][j] = t1.data[i][j] + t2.data[i][j];
        }
    }
    return r;
}

template <typename T>
T operator-(const T t1, const T t2)
{
    T r;
    for (unsigned int i = 0; i < t1.row; i++) {
        for (unsigned int j = 0; j < t1.col; j++) {
            r.data[i][j] = t1.data[i][j] - t2.data[i][j];
        }
    }
    return r;
}

template <typename T>
T operator*(const T t1, const T t2)
{
    T r;
    if (r.col == 1) { // 向量点乘
        for (unsigned int i = 0; i < t1.row; i++) {
            for (unsigned int j = 0; j < t1.col; j++) {
                r.data[i][j] = t1.data[i][j] * t2.data[i][j];
            }
        }
    } else { // 矩阵叉乘
        for (unsigned int i = 0; i < r.row; i++) {
            for (unsigned int j = 0; j < r.col; j++) {
                for (unsigned int k = 0; k < t1.col; k++) {
                    r.data[i][j] += t1.data[i][k] * t2.data[k][j];
                }
            }
        }
    }

    return r;
}

template <typename T>
T operator*(const T t, const double k)
{
    T r;
    for (unsigned int i = 0; i < t.row; i++) {
        for (unsigned int j = 0; j < t.col; j++) {
            r.data[i][j] = t.data[i][j] * k;
        }
    }
    return r;
}

template <typename T>
T operator*(const double k, const T t)
{
    T r;
    for (unsigned int i = 0; i < t.row; i++) {
        for (unsigned int j = 0; j < t.col; j++) {
            r.data[i][j] = k * t.data[i][j];
        }
    }
    return r;
}

template <typename VEC>
double Dot(const VEC m1, const VEC m2)
{
    double r = 0;
    for (unsigned int i = 0; i < m1.row; i++) {
        r += m1.data[i][0] * m2.data[i][0];
    }
    return r;
}

// 向量转置相乘，得到同维度的矩阵
Matrix2d TransMultiply(const Vector2d v1, const Vector2d v2)
{
    Matrix2d r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = v1.data[i][0] * v2.data[j][0];
        }
    }
   return r;
}

Matrix3d TransMultiply(const Vector3d v1, const Vector3d v2)
{
    Matrix3d r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = v1.data[i][0] * v2.data[j][0];
        }
    }
   return r;
}

 Matrix4d TransMultiply(const Vector4d v1, const Vector4d v2)
 {
     Matrix4d r;
     for (unsigned int i = 0; i < r.row; i++) {
         for (unsigned int j = 0; j < r.col; j++) {
             r.data[i][j] = v1.data[i][0] * v2.data[j][0];
         }
     }
    return r;
 }

// 矩阵向量相乘
Vector2d operator*(const Matrix2d m, const Vector2d v)
{
    Vector2d r;
    for (unsigned int i = 0; i < m.row; i++) {
        for (unsigned int j = 0; j < m.col; j++) {
            r.data[i][0] += m.data[i][j] * v.data[j][0];
        }
    }
   return r;
}

Vector3d operator*(const Matrix3d m, const Vector3d v)
{
    Vector3d r;
    for (unsigned int i = 0; i < m.row; i++) {
        for (unsigned int j = 0; j < m.col; j++) {
            r.data[i][0] += m.data[i][j] * v.data[j][0];
        }
    }
   return r;
}

 Vector4d operator*(const Matrix4d m, const Vector4d v)
 {
     Vector4d r;
     for (unsigned int i = 0; i < m.row; i++) {
         for (unsigned int j = 0; j < m.col; j++) {
             r.data[i][0] += m.data[i][j] * v.data[j][0];
         }
     }
    return r;
 }

// 打印
template <typename T>
void show(T t)
{
    for (unsigned int i = 0; i < t.row; i++) {
        for (unsigned int j = 0; j < t.col; j++) {
            LOG_DEBUG("%.2f \t", t.data[i][j]);
        }
        LOG_DEBUG("\n");
    }
    LOG_DEBUG("\n");
}