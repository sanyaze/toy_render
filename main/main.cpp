#include "math_tools.h"
#include "log_debug.h"
#include "bmp_creater.h"
#include "render.h"
#include <fstream>

int main()
{
    int width = 50;
    int height = 50;
    BmpCreater bmp(width, height);

    // 定义视锥体
    double fov = 90.0f;
    double aspect = double(width) / double(height);
    double near = -1.0f;
    double far = -100.0f;
    Matrix4d proj = Projection(fov, aspect, near, far);

    // 定义一个简单的立方体，中心坐标为(0,0,-50), 边长为30
    Vector4d cube[8] = {
        Vector4d(-15., -15., -35., 1.),
        Vector4d(15., -15., -35., 1.),
        Vector4d(15., 15., -35., 1.),
        Vector4d(-15., 15., -35., 1.),
        Vector4d(-15., -15., -65., 1.),
        Vector4d(15., -15., -65., 1.),
        Vector4d(15., 15., -65., 1.),
        Vector4d(-15., 15., -65., 1.),
    };

    // 定义面
    int face[6][4] = {
        { 0, 1, 2, 3 },
        { 4, 5, 6, 7 },
        { 0, 4, 5, 1 },
        { 1, 5, 6, 2 },
        { 2, 6, 7, 3 },
        { 3, 7, 4, 0 }
    };

    // 定义立方体各个平面的颜色
    uint8_t color[6][3] = {
        { 255, 0, 0 },   // 上
        { 0, 255, 0 },   // 下
        { 0, 0, 255 },   // 前
        { 255, 255, 0 }, // 后
        { 255, 0, 255 }, // 左
        { 0, 255, 255 }  // 右
    };

    // 变换
    RotatebyAxis(std::vector<Vector4d>(std::begin(cube), std::end(cube)), Vector3d(1., 1., 1.), 30.);

    // 绘制立方体
    // 顶点投影
    std::vector<Vector3d> vProj = VertexProj(std::vector<Vector4d>(std::begin(cube), std::end(cube)), proj, width, height);
    LOG_INFO("start render\n");

    // 面元着色
    std::vector<std::vector<double>> zBuffer(width, std::vector<double>(height, -INFINITY));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Vector2d v = Vector2d(double(j), double(i));
            for (unsigned int k = 0; k < 1; k++) {
                Vector3d t[2][3] = { { vProj[face[k][0]], vProj[face[k][1]], vProj[face[k][2]] },
                                     { vProj[face[k][0]], vProj[face[k][2]], vProj[face[k][3]] } };
                
                for (int l = 0; l < 2; l++) {
                    // 计算三角形所在平面方程
                    Vector3d fNormal = Vector3d::Cross(t[l][1] - t[l][0], t[l][2] - t[l][0]);
                    // 平面与z坐标轴平行，在XoY平面上的投影为一条直线, 不处理
                    if (fNormal.data[2][0] <= 0.001) {
                        continue;
                    }
                    // 计算z轴坐标
                    double vz = Vz(v, t[i], fNormal);
                    if (vz < zBuffer[i][j]) { // 遮挡关系，不渲染
                        continue;
                    }
                    zBuffer[i][j] = vz;
                    if (IsPointInPlane(v, t[l])) {
                        bmp.SetPixel(j, i, color[k][0], color[k][1], color[k][2]);
                    }
                }
            }

            printf("%u, %u\n", i, j);
        }
    }
    
    bmp.Save("test.bmp");
    printf("success!\n");
    return 0;
}