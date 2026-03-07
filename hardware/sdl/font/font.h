#ifndef FONT_H
#define FONT_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define GET_FONT(obj) ((Font *)obj)
// 类声明
typedef struct _Font Font;
typedef struct _FontFun FontFun;
// 类成员函数结构
struct _FontFun {
    void (*destroy)(Font* self);
	void (*parse)(Font* self, const uint8_t *glyphData, uint32_t dataSize);

};

// 字形数据结构
typedef struct {
    int16_t numberOfContours;  // 轮廓数（>0简单，<0复合）
    int16_t xMin;              // 边界框
    int16_t yMin;
    int16_t xMax;
    int16_t yMax;

    // 简单字形特有
    uint16_t* endPtsOfContours;  // 每个轮廓的终点索引
    uint16_t instructionLength;   // 指令长度
    uint8_t* instructions;        // 指令数据
    uint8_t* flags;              // 点标志
    int16_t* xCoordinates;        // X坐标
    int16_t* yCoordinates;        // Y坐标

    // 复合字形特有
    uint16_t componentCount;     // 组件数
    struct {
        uint16_t flags;
        uint16_t glyphIndex;
        int16_t arg1, arg2;      // 位置参数
        float scaleX, scaleY;    // 缩放
        float skewX, skewY;      // 倾斜
    }* components;
} GlyphData;

// 类结构
struct _Font {
    const FontFun* fun;
    // TODO: 添加数据成员
    GlyphData* glyph;
};

// 构造函数声明
Font* font_create();
void font_init(Font* self);

// 析构函数声明
void font_deinit(Font* self);

#endif // FONT_H