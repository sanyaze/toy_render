#include "math_tools.h"
#include "log_debug.h"
#include "bmp_creater.h"
#include "render.h"
#include <fstream>

int main()
{
    int width = 500;
    int height = 500;
    BmpCreater bmp(width, height);

    // 定义视锥体
    double fov = 90.0f;
    double aspect = double(width) / double(height);
    double near = -1.0f;
    double far = -1000.0f;
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
        { 0, 1, 2, 3 }, // 前
        { 7, 6, 5, 4 }, // 后
        { 0, 4, 5, 1 }, // 下
        { 1, 5, 6, 2 }, // 右
        { 2, 6, 7, 3 }, // 上
        { 3, 7, 4, 0 }  // 左
    };

    // 定义立方体各个平面的颜色
    uint8_t color[6][3] = {
        { 255, 0, 0 },   // 前
        { 0, 255, 0 },   // 后
        { 0, 0, 255 },   // 下
        { 255, 255, 0 }, // 右
        { 255, 0, 255 }, // 上
        { 0, 255, 255 }  // 左
    };

    // 变换
    std::vector<Vector4d> vecCube = std::vector<Vector4d>(std::begin(cube), std::end(cube));
    vecCube = ShiftbyDir(vecCube, Vector3d(0., 0., 1.), 50);
    vecCube = RotatebyAxis(vecCube, Vector3d(1., 1., 1.), 45.);
    vecCube = ShiftbyDir(vecCube, Vector3d(0., 0., 1.), -50);

    // 绘制立方体
    // 顶点投影
    std::vector<Vector3d> vProj = VertexProj(vecCube, proj, width, height);
    LOG_INFO("start render\n");

    // 面元着色
    std::vector<std::vector<double>> zBuffer(width, std::vector<double>(height, -INFINITY));
    // 像素矩阵每行每列多两个点，方便后处理流程对边界的处理
    std::vector<std::vector<double>> pixel_r(width + 2, std::vector<double>(height + 2, 0));
    std::vector<std::vector<double>> pixel_g(width + 2, std::vector<double>(height + 2, 0));
    std::vector<std::vector<double>> pixel_b(width + 2, std::vector<double>(height + 2, 0));
    for (int i = 0; i < height; i++) {
       for (int j = 0; j < width; j++) {
           Vector2d v = Vector2d(double(j), double(i));
           for (unsigned int k = 0; k < 6; k++) {
               Vector3d t[2][3] = { { vProj[face[k][0]], vProj[face[k][1]], vProj[face[k][2]] },
                                    { vProj[face[k][0]], vProj[face[k][2]], vProj[face[k][3]] } };
               
               for (int l = 0; l < 2; l++) {
                   // 计算三角形所在平面方程
                   Vector3d fNormal = Vector3d::Cross(t[l][1] - t[l][0], t[l][2] - t[l][0]);
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
                           LOG_INFO("color[%d, %d, %d]\n", color[k][0], color[k][1], color[k][2]);
                           //bmp.SetPixel(j, i, color[k][0], color[k][1], color[k][2]);
                           // 两侧的点为记录像素的边界，因此索引从1开始
                           pixel_r[i+1][j+1] = color[k][0];
                           pixel_g[i+1][j+1] = color[k][1];
                           pixel_b[i+1][j+1] = color[k][2];
                       }
                   }
               }
           }
           printf("%u, %u\n", i, j);
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
        }
    }

    // 绘制顶点
    //for (auto &v : vProj) {
    //    bmp.SetPixel((int)v.data[0][0], (int)v.data[1][0], 0, 0, 0);
    //}
    
    bmp.Save("test.bmp");
    printf("success!\n");
    return 0;
}