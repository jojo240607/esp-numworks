#include "serialconv.h"
#include <stdio.h>

static uint8_t serialconv_read(Serialconv* self);
static void serialconv_write(Serialconv* self, uint8_t data);

// 析构函数声明
static void serialconv_destroy(Serialconv* self);

// TODO: 初始化数据成员
static const SerialconvFun serialconv_fun = {
    .destroy = serialconv_destroy,
	.read = serialconv_read,
	.write = serialconv_write,
};
// 构造函数实现
Serialconv* serialconv_create() {
    Serialconv* obj = (Serialconv*)malloc(sizeof(Serialconv));
    if (obj) {
        memset(obj, 0, sizeof(Serialconv));
        serialconv_init(obj);
    }
    return obj;
}

void serialconv_init(Serialconv* self) {
    self->fun = &(serialconv_fun);
    // TODO: 初始化数据成员
    //设置各个操作io
    self->data = gpio_create(SERIAL_DATA);
    self->clk = gpio_create(SERIAL_CLK);
    self->read_ctrl = gpio_create(SERIAL_READ);
    self->write_ctrl = gpio_create(SERIAL_WRITE);

    //设置gpio输入输出模式
    self->clk->fun->set_mode(self->clk, pushpull_output);//输出模式
    self->data->fun->set_mode(self->data, pushpull_output);//输出模式
    self->read_ctrl->fun->set_mode(self->read_ctrl, pushpull_output);//输出模式
    self->write_ctrl->fun->set_mode(self->write_ctrl, pushpull_output);//输出模式

    //设置默认电平
    self->clk->fun->set(self->clk, 0);
    self->data->fun->set(self->data, 0);
    self->read_ctrl->fun->set(self->read_ctrl, 0);
    self->write_ctrl->fun->set(self->write_ctrl, 0);
}

void serialconv_deinit(Serialconv* self) {
    // TODO: 数据成员申请资源释放
    if (self->data != NULL) {
        self->data->fun->destroy(self->data);
    }

    if (self->clk != NULL) {
        self->clk->fun->destroy(self->clk);
    }

    if (self->read_ctrl != NULL) {
        self->read_ctrl->fun->destroy(self->read_ctrl);
    }

    if (self->write_ctrl != NULL) {
        self->write_ctrl->fun->destroy(self->write_ctrl);
    }
}

// 析构函数实现
static void serialconv_destroy(Serialconv* self) {
    if (self != NULL) {
        serialconv_deinit(self);
        free(self);
    }
}

// read method
/*
 *  CE 默认接地拉低 → 165输入使能。
    触发SH/LD引脚，锁存当前列电平。
    GPIO_A 切换为输入模式。
    发送时钟，从165读取数据。
 */
static uint8_t serialconv_read(Serialconv* self) {
    // TODO: add read method
    uint8_t read_data = 0;
    //165_SH/LD） 一个从高到低的脉冲（先拉高再拉低再拉高）。在 SH/LD为低电平期间，165会将其并行输入引脚A-H上的电平（即矩阵列线状态）锁存到内部寄存器
    self->read_ctrl->fun->set(self->read_ctrl, 1);//set SH/LD high
    self->read_ctrl->fun->set(self->read_ctrl, 0);//set SH/LD low
    self->read_ctrl->fun->set(self->read_ctrl, 1);//set SH/LD high

    self->data->fun->set_mode(self->data, highimpedance_input);//读165数据之前，必须先将GPIO设置为高阻输入模式
    //SH/LD为高，CE为低（接地），芯片进入“移位模式”。通过 GPIO_B（共享时钟） 发出8个上升沿，每个上升沿都会将165内部寄存器的一位数据从 Q7 引脚移出。

    for (uint8_t bit = 0; bit < 8; bit++) {
        //clk 上升沿读取数据
        self->clk->fun->set(self->clk, 0);//0
        self->clk->fun->set(self->clk, 1);//1
        //need wait ???
        read_data |= (self->data->fun->get(self->data) << bit);//get a bit 先收到的存到高位
    }
    //printf("read data 0x%x\n", read_data);
    return read_data;
}
// write method
/*
 *  OE 直接接地。  拉高 → 165输出高阻态,直接接地。
 *  MR / SRCLR 接 vcc
    GPIO_A 设为输出模式。
    发送数据与时钟给595，最后锁存。
    STCP 一个从低到高的上升沿脉冲锁存
 */
static void serialconv_write(Serialconv* self, uint8_t data) {
    // TODO: add write method
    self->write_ctrl->fun->set(self->write_ctrl, 0);//（595_RCLK/STCP）锁存信号置低
    self->data->fun->set_mode(self->data, pushpull_output);//写595数据之前，确保GPIO已设置为输出模式。
    //上升沿触发。每个上升沿，将SER引脚上的当前数据位（0或1）移入芯片内部的8位移位寄存器。数据在上升沿时被锁存。
    for (uint8_t bit = 0; bit < 8; bit++) {
        self->data->fun->set(self->data, (data >> bit) & 0x01);//set a bit  从低位开始输入
        //need wait ???
        //clk 上升沿设置数据
        self->clk->fun->set(self->clk, 0);//0
        self->clk->fun->set(self->clk, 1);//1
    }
    //（595_RCLK/STCP） 一个从低到高的上升沿脉冲，将移位寄存器的数据锁存到输出引脚Q0-Q7
    self->write_ctrl->fun->set(self->write_ctrl, 1);//上升沿数据锁存
    //printf("write data 0x%x\n", data);
}

