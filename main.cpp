#pragma warning(disable:4996)
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdio>

//using namespace std;

struct SHPHeader {
    uint32_t fileCode;
    uint32_t fileLen;
    uint32_t SHPType;

    uint64_t Xmin;
    uint64_t Ymin;
    uint64_t Xmax;
    uint64_t Ymax;
    uint64_t Zmin;
    uint64_t Zmax;
    uint64_t Mmin;
    uint64_t Mmax;
};

bool readShapefile(const char* fileName) {
    FILE* fp = fopen(fileName, "rb"); // ���̳ʸ� ���� ����
    if (fp == NULL) {
        std::cout << "Failed to open file\n";
        return false;
    }

    fseek(fp, 0L, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);

    // Double�� Little�� ����
    //  ���� ������� �ϴ� �� Big!
    //  Double�� ������ �� ���� ���Ḹ �ϸ� �ǹǷ�, �ּҴ����� int�� ����

    // Shapefile�� �� ���Ͽ� �ϳ��� SHP Ÿ�Ը� ����
    // ��, ��� �м� �������� Ÿ�� Ư�� ����

    // data �ʱ�ȭ
    // 1byte������ ��� data�� ����
    // ������ ���� �м�
    //      ��ü ���� ���
    //          Shapefile Ÿ�� Ȯ��
    //          ���� �߻� �� false
    //              (���� ���): ����� ������ ���� ����
    //      ���ڵ� ���(Int Big �� ��)
    //      ���ڵ� ����(Ing/Double Little)

    // ���� ũ��: 1778986



    uint8_t* data = new uint8_t[fileSize];
    memset(data, 0, fileSize);
    fread(data, sizeof(uint8_t), fileSize, fp);


    uint64_t unit = 0;
    bool isBigEndian = true;
    bool isInteger = true;


    for (size_t i = 0; i < 100;) {
        size_t currentIdx = i;
        if (currentIdx == 28) {
            isBigEndian = false;
        }
        else if (currentIdx == 36) {
            isInteger = false;
        }

        if (isBigEndian) {
            unit = (data[i] << 24) | (data[i + 1] << 16) | (data[i + 2] << 8) | data[i + 3];
            i += 4;
        }
        else {
            // default: Integer Little
            unit = data[i] << 0 | (data[i + 1] << 8) | (data[i + 2] << 16) | (data[i + 3] << 24);
            i += 4;
            if (!isInteger) { // Double Little
                unit = unit | data[i] << 32 | (data[i + 1] << 40) | (data[i + 2] << 48) | (data[i + 3] << 56);
                i += 4;
            }
        }

        if (currentIdx == 0) {
            printf("%d\n", (uint32_t)unit);
        }
        else if (currentIdx == 24) {
            printf("%d\n", (uint32_t)unit);
        }
        else if (currentIdx == 28) {
            printf("%d\n", (uint32_t)unit);
        }
        else if (currentIdx == 32) {
            printf("%d\n", (uint32_t)unit);
        }
        else if (currentIdx == 36) {
            printf("%f\n", (double)unit);
        }
    }

    fclose(fp);
    delete[] data;

    return true;
}

int main() {
    readShapefile("sample.shp");
    return 0;
}
