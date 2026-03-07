#include "font.h"
#include <stdio.h>

static void font_parse(Font* self, const uint8_t *glyphData, uint32_t dataSize);

// 析构函数声明
static void font_destroy(Font* self);
static uint8_t* parse_flags(uint8_t* ptr, uint8_t* flags, uint16_t numPoints);
static int16_t* parse_coordinates(uint8_t** ptrPtr, uint8_t* flags,
                                  uint16_t numPoints, int16_t initialValue,
                                  uint8_t sameBit, uint8_t byteBit);
// TODO: 初始化数据成员
static const FontFun font_fun = {
    .destroy = font_destroy,
	.parse = font_parse,
};
// 构造函数实现
Font* font_create() {
    Font* obj = (Font*)malloc(sizeof(Font));
    if (obj) {
        memset(obj, 0, sizeof(Font));
        font_init(obj);
    }
    return obj;
}

void font_init(Font* self) {
    self->fun = &(font_fun);
    // TODO: 初始化数据成员
    self->glyph = malloc(sizeof(GlyphData));
}

void font_deinit(Font* self) {
    // TODO: 数据成员申请资源释放
    if (self->glyph != NULL) {
        free(self->glyph);
        self->glyph = NULL;
    }
}

// 析构函数实现
static void font_destroy(Font* self) {
    if (self != NULL) {
        font_deinit(self);
        free(self);
    }
}

// 解析标志位
static uint8_t* parse_flags(uint8_t* ptr, uint8_t* flags, uint16_t numPoints) {
    for (uint16_t i = 0; i < numPoints;) {
        uint8_t flag = *ptr++;
        flags[i++] = flag;

        // 处理重复标志（第1位为1表示重复）
        if (flag & 0x08) {
            uint8_t repeat = *ptr++;
            for (uint8_t j = 0; j < repeat; j++) {
                if (i < numPoints) {
                    flags[i++] = flag;
                }
            }
        }
    }
    return ptr;
}

// 解析坐标（增量编码）
static int16_t* parse_coordinates(uint8_t** ptrPtr, uint8_t* flags,
                           uint16_t numPoints, int16_t initialValue,
                           uint8_t sameBit, uint8_t byteBit) {
    int16_t* coords = malloc(numPoints * sizeof(int16_t));
    int16_t value = initialValue;
    uint8_t* ptr = *ptrPtr;

    for (uint16_t i = 0; i < numPoints; i++) {
        uint8_t flag = flags[i];
        int16_t delta = 0;

        if (flag & sameBit) {
            // 相同标志位：增量与前一个相同
            if (flag & byteBit) {
                // 增量是1字节
                delta = (int8_t)*ptr++;
            } else {
                // 增量是0
                delta = 0;
            }
        } else {
            // 不同标志位：增量是2字节
            if (flag & byteBit) {
                // 增量是1字节（有符号）
                delta = (int8_t)*ptr++;
            } else {
                // 增量是2字节（有符号）
                delta = (int16_t)((ptr[0] << 8) | ptr[1]);
                ptr += 2;
            }
        }

        value += delta;
        coords[i] = value;
    }

    *ptrPtr = ptr;
    return coords;
}

// 解析字形数据
// parse method
static void font_parse(Font* self, const uint8_t *glyphData, uint32_t dataSize) {
    // TODO: add parse method
    if (!glyphData || dataSize < 10) {
        return;
    }
    const uint8_t* ptr = glyphData;
    // 读取字形头
    self->glyph->numberOfContours = (ptr[0] << 8) | ptr[1];
    self->glyph->xMin = (ptr[2] << 8) | ptr[3];
    self->glyph->yMin = (ptr[4] << 8) | ptr[5];
    self->glyph->xMax = (ptr[6] << 8) | ptr[7];
    self->glyph->yMax = (ptr[8] << 8) | ptr[9];
    ptr += 10;

    if (self->glyph->numberOfContours > 0) {
        // 简单字形
        uint16_t numContours = self->glyph->numberOfContours;

        // 读取轮廓终点
        self->glyph->endPtsOfContours = malloc(numContours * sizeof(uint16_t));
        for (int i = 0; i < numContours; i++) {
            self->glyph->endPtsOfContours[i] = (ptr[0] << 8) | ptr[1];
            ptr += 2;
        }

        // 总点数 = 最后一个轮廓终点 + 1
        uint16_t numPoints = self->glyph->endPtsOfContours[numContours - 1] + 1;

        // 读取指令长度
        self->glyph->instructionLength = (ptr[0] << 8) | ptr[1];
        ptr += 2;

        // 读取指令（跳过，通常不需要）
        self->glyph->instructions = malloc(self->glyph->instructionLength);
        for (int i = 0; i < self->glyph->instructionLength; i++) {
            self->glyph->instructions[i] = *ptr++;
        }

        // 解析标志
        self->glyph->flags = malloc(numPoints);
        uint8_t* mutablePtr = (uint8_t*)ptr;
        mutablePtr = parse_flags(mutablePtr, self->glyph->flags, numPoints);
        ptr = mutablePtr;

        // 解析X坐标
        mutablePtr = (uint8_t*)ptr;
        self->glyph->xCoordinates = parse_coordinates(&mutablePtr, self->glyph->flags,
                                                numPoints, 0, 0x10, 0x02);
        ptr = mutablePtr;

        // 解析Y坐标
        self->glyph->yCoordinates = parse_coordinates(&mutablePtr, self->glyph->flags,
                                                numPoints, 0, 0x20, 0x04);
        ptr = mutablePtr;

    } else if (self->glyph->numberOfContours < 0) {
        // 复合字形
        self->glyph->componentCount = 0;
        self-> glyph->components = NULL;

        // 解析组件
        uint16_t flags;
        uint8_t* mutablePtr = (uint8_t*)ptr;

        do {
            flags = (mutablePtr[0] << 8) | mutablePtr[1];
            mutablePtr += 2;

            // 重新分配内存
            self->glyph->components = realloc(self->glyph->components,
                                        (self->glyph->componentCount + 1) *
                                        sizeof(self->glyph->components[0]));

            self->glyph->components[self->glyph->componentCount].flags = flags;
            self->glyph->components[self->glyph->componentCount].glyphIndex =
                    (mutablePtr[0] << 8) | mutablePtr[1];
            mutablePtr += 2;

            // 解析参数（根据标志位）
            // 这里简化处理，实际需要根据标志位解析
            self->glyph->componentCount++;

        } while (flags & 0x0020);  // 还有更多组件
    }
}

