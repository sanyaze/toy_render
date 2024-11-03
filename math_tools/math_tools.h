#pragma once
#include <cmath>
#include "log_debug.h"

// 矩阵按照2，3，4三个维度创建类型
class Matrix2f
{
public:
    float data[2][2];
    unsigned int row = 2;
    unsigned int col = 2;
    Matrix2f() : data{ {  0.0f, 0.0f },
                       {  0.0f, 0.0f } } {}
    Matrix2f(float a[2][2]) : data{ { a[0][0], a[0][1] },
                                    { a[1][0], a[1][1] } } {}
    
    float Det()
    {
        return data[0][0] * data[1][1] - data[0][1] * data[1][0];
    }
};

class Matrix3f
{
public:
    float data[3][3];
    unsigned int row = 3;
    unsigned int col = 3;
    Matrix3f() : data{ {  0.0f, 0.0f, 0.0f },
                       {  0.0f, 0.0f, 0.0f },
                       {  0.0f, 0.0f, 0.0f } } {}
    Matrix3f(float a[3][3]) : data{ { a[0][0], a[0][1], a[0][2] },
                                    { a[1][0], a[1][1], a[1][2] },
                                    { a[2][0], a[2][1], a[2][2] } } {}
    
    Matrix2f SubMat(int r, int c) {
        Matrix2f subMat;

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

    float Det()
    {
        float r = 0;
        for (unsigned int i = 0; i < row; i++) {
            Matrix2f subMat = SubMat(i, 0);
            r += data[i][0] * subMat.Det() * (i % 2 == 0 ? 1 : -1);
        }
        return r;
    }
};

class Matrix4f
{
public:
    float data[4][4];
    unsigned int row = 4;
    unsigned int col = 4;
    Matrix4f() : data{ {  0.0f, 0.0f, 0.0f, 0.0f },
                       {  0.0f, 0.0f, 0.0f, 0.0f },
                       {  0.0f, 0.0f, 0.0f, 0.0f },
                       {  0.0f, 0.0f, 0.0f, 0.0f } } {}
    Matrix4f(float a[4][4]) : data{ { a[0][0], a[0][1], a[0][2], a[0][3] },
                                    { a[1][0], a[1][1], a[1][2], a[1][3] },
                                    { a[2][0], a[2][1], a[2][2], a[2][3] },
                                    { a[3][0], a[3][1], a[3][2], a[3][3] } } {}
    
    Matrix3f SubMat(int r, int c)
    {
        Matrix3f subMat;

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

    float Det()
    {
        float r = 0;
        for (unsigned int i = 0; i < row; i++) {
            Matrix3f subMat = SubMat(i, 0);
            r += data[i][0] * subMat.Det() * (i % 2 == 0 ? 1 : -1);
        }
        return r;
    }
};

// 向量也按照2，3，4三个维度创建类型
class Vector2f
{
public:
    float data[2][1];
    unsigned int row = 2;
    unsigned int col = 1;
    Vector2f() : data{ 0.0f, 0.0f } {}
    Vector2f(float a[2]) : data{ a[0], a[1] } {}
    Vector2f(float x, float y) : data{ x, y } {}

    float Mod()
    {
        return sqrt(data[0][0] * data[0][0] + data[1][0] * data[1][0]);
    }

    Vector2f Normal()
    {
        if (Mod() == 0.0f) {
            return Vector2f(0.0f, 0.0f);
        } else {
            return Vector2f(data[0][0] / Mod(), data[1][0] / Mod());
        }
    }
};

class Vector3f
{
public:
    float data[3][1];
    unsigned int row = 3;
    unsigned int col = 1;
    Vector3f() : data{ 0.0f, 0.0f, 0.0f } {}
    Vector3f(float a[3]) : data{ a[0], a[1], a[2] } {}
    Vector3f(float x, float y, float z) : data{ x, y, z } {}

    // 向量外积，只在三维成立
    Vector3f Cross(const Vector3f v1, const Vector3f v2) {
        Vector3f r = Vector3f(v1.data[1][0] * v2.data[2][0] - v1.data[2][0] * v2.data[1][0],
                              v1.data[2][0] * v2.data[0][0] - v1.data[0][0] * v2.data[2][0],
                              v1.data[0][0] * v2.data[1][0] - v1.data[1][0] * v2.data[0][0] );
        return r;
    }

    float Mod()
    {
        return sqrt(data[0][0] * data[0][0] + data[1][0] * data[1][0] + data[2][0] * data[2][0]);
    }

    Vector3f Normal()
    {
        if (Mod() == 0.0f) {
            return Vector3f(0.0f, 0.0f, 0.0f);
        } else {
            return Vector3f(data[0][0] / Mod(), data[1][0] / Mod(), data[2][0] / Mod());
        }
    }
};

class Vector4f
{
public:
    float data[4][1];
    unsigned int row = 4;
    unsigned int col = 1;
    Vector4f() : data{ 0.0f, 0.0f, 0.0f, 0.0f } {}
    Vector4f(float a[4]) : data{ a[0], a[1], a[2], a[3] } {}
    Vector4f(float x, float y, float z, float w) : data{ x, y, z, w } {}

    float Mod()
    {
        return sqrt(data[0][0] * data[0][0] + data[1][0] * data[1][0] + data[2][0] * data[2][0] + data[3][0] * data[3][0]);
    }

    Vector4f Normal()
    {
        if (Mod() == 0.0f) {
            return Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
        } else {
            return Vector4f(data[0][0] / Mod(), data[1][0] / Mod(), data[2][0] / Mod(), data[3][0] / Mod());
        }
    }
};

// 特殊矩阵生成
// 单位矩阵
Matrix2f Eye2()
{
    Matrix2f r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return r;
}

Matrix3f Eye3()
{
    Matrix3f r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return r;
}

Matrix4f Eye4()
{
    Matrix4f r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return r;
}

// 全1矩阵
Matrix2f Ones2()
{
    Matrix2f r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = 1.0;
        }
    }
    return r;
}

Matrix3f Ones3()
{
    Matrix3f r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = 1.0;
        }
    }
    return r;
}

Matrix4f Ones4()
{
    Matrix4f r;
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
T operator*(const T t, const float k)
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
T operator*(const float k, const T t)
{
    T r;
    for (unsigned int i = 0; i < t.row; i++) {
        for (unsigned int j = 0; j < t.col; j++) {
            r.data[i][j] = k * t.data[i][j];
        }
    }
    return r;
}

template <typename MAT>
float Dot(const MAT m1, const MAT m2)
{
    float r = 0;
    for (unsigned int i = 0; i < m1.row; i++) {
        for (unsigned int j = 0; j < m1.col; j++) {
            r += m1.data[i][j] * m2.data[i][j];
        }
    }
    return r;
}

// 向量转置相乘，得到同维度的矩阵
Matrix2f TransMultiply(const Vector2f v1, const Vector2f v2)
{
    Matrix2f r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = v1.data[i][0] * v2.data[j][0];
        }
    }
   return r;
}

Matrix3f TransMultiply(const Vector3f v1, const Vector3f v2)
{
    Matrix3f r;
    for (unsigned int i = 0; i < r.row; i++) {
        for (unsigned int j = 0; j < r.col; j++) {
            r.data[i][j] = v1.data[i][0] * v2.data[j][0];
        }
    }
   return r;
}

 Matrix4f TransMultiply(const Vector4f v1, const Vector4f v2)
 {
     Matrix4f r;
     for (unsigned int i = 0; i < r.row; i++) {
         for (unsigned int j = 0; j < r.col; j++) {
             r.data[i][j] = v1.data[i][0] * v2.data[j][0];
         }
     }
    return r;
 }

// 矩阵向量相乘
Vector2f operator*(const Matrix2f m, const Vector2f v)
{
    Vector2f r;
    for (unsigned int i = 0; i < m.row; i++) {
        for (unsigned int j = 0; j < m.col; j++) {
            r.data[i][0] += m.data[i][j] * v.data[j][0];
        }
    }
   return r;
}

Vector3f operator*(const Matrix3f m, const Vector3f v)
{
    Vector3f r;
    for (unsigned int i = 0; i < m.row; i++) {
        for (unsigned int j = 0; j < m.col; j++) {
            r.data[i][0] += m.data[i][j] * v.data[j][0];
        }
    }
   return r;
}

 Vector4f operator*(const Matrix4f m, const Vector4f v)
 {
     Vector4f r;
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