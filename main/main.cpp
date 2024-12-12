#include "math_tools.h"
#include "log_debug.h"
#include "bmp_creater.h"
#include "render.h"
#include <fstream>
#include <iostream>

int main(int argc, char* argv)
{
    // 定义渲染平面大小
    int width = 600;
    int height = 600;
    // 定义视锥体参数
    double fov = 90.0f;
    double near = -1.0f;
    double far = -1000.0f;
    
    // 读取配置参数
    if (argc > 1) {
        width = atoi(&argv[1]);
    }
    if (argc > 2) {
        height = atoi(&argv[2]);
    }
    if (argc > 3) {
        fov = atof(&argv[3]);
    }
    if (argc > 4) {
        near = atof(&argv[4]);
    }
    if (argc > 5) {
        far = atof(&argv[5]);
    }

    double aspect = double(width) / double(height);
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
    vecCube = RotatebyAxis(vecCube, Vector3d(0., 0., 1.), 30.);
    //vecCube = RotatebyAxis(vecCube, Vector3d(0., 1., 0.), -30.);
    vecCube = ShiftbyDir(vecCube, Vector3d(0., 0., 1.), -100);

    // 绘制立方体
    std::vector<std::vector<int>> vecFace;
    std::vector<std::vector<int>> vecTexture;
    for (int i = 0; i < 6; i++) {
        std::vector<int> faceTmp = std::vector<int>(std::begin(face[i]), std::end(face[i]));
        std::vector<int> colorTmp = std::vector<int>(std::begin(color[i]), std::end(color[i]));
        vecFace.push_back(faceTmp);
        vecTexture.push_back(colorTmp);
    }

    BmpCreater bmp(width, height);
    Render(vecCube, vecFace, vecTexture, proj, width, height, bmp);
    bmp.Save("test.bmp");
    printf("success!\n");

    return 0;
}