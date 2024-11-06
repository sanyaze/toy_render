#pragma once
#include "log_debug.h"
#include <cstdint>
#include <vector>
#include <fstream>
#include <string>

#pragma pack(push, 1)
struct BmpFileHeader
{
	uint16_t bfType = 0x4D42;
    uint32_t bfSize;
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits = 54;
};

struct BmpInfoHeader
{
	uint32_t biSize = 40;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes = 1;
    uint16_t biBitCount = 32; // 默认rgba 32位颜色
    uint32_t biCompression = 0;
    uint32_t biSizeImage = 0;
    int32_t  biXPelsPerMeter = 2835;
    int32_t  biYPelsPerMeter = 2835;
    uint32_t biColorUsed = 0;
    uint32_t biColorImportant = 0;
};
#pragma pack(pop)

class BmpCreater
{
public:
	BmpFileHeader fileHeader;
    BmpInfoHeader infoHeader;
    std::vector<uint32_t> data;
    
    BmpCreater(int width = 100, int height = 100)
    {
        infoHeader.biWidth = width;
        infoHeader.biHeight = height;
        data = std::vector<uint32_t>(width * height, 0xffffffff);
        fileHeader.bfSize = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader) + data.size() * sizeof(uint32_t);
    }

    void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0)
    {
        if (x < 0 || x >= infoHeader.biWidth || y < 0 || y >= infoHeader.biHeight) {
            return;
        }
        int index = y * infoHeader.biWidth + x;
        // 组装rgba为一个32位整数
        uint32_t rgba = (a << 24) | (r << 16) | (g << 8) | b;
        data[index] = rgba;
    }

    void Save(const std::string path)
    {
        std::ofstream out(path.c_str(), std::ios_base::binary);
        out.write((char*)&fileHeader, sizeof(fileHeader));
        out.write((char*)&infoHeader, sizeof(infoHeader));
        out.write((char*)data.data(), data.size() * sizeof(uint32_t));
        out.close();
    }
};