#include "sst7789_device.h"
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ     CONFIG_EXAMPLE_LCD_PIXEL_CLOCK_HZ

#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL  1
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL
#define EXAMPLE_PIN_NUM_DATA0          CONFIG_EXAMPLE_PIN_NUM_DATA0
#define EXAMPLE_PIN_NUM_DATA1          CONFIG_EXAMPLE_PIN_NUM_DATA1
#define EXAMPLE_PIN_NUM_DATA2          CONFIG_EXAMPLE_PIN_NUM_DATA2
#define EXAMPLE_PIN_NUM_DATA3          CONFIG_EXAMPLE_PIN_NUM_DATA3
#define EXAMPLE_PIN_NUM_DATA4          CONFIG_EXAMPLE_PIN_NUM_DATA4
#define EXAMPLE_PIN_NUM_DATA5          CONFIG_EXAMPLE_PIN_NUM_DATA5
#define EXAMPLE_PIN_NUM_DATA6          CONFIG_EXAMPLE_PIN_NUM_DATA6
#define EXAMPLE_PIN_NUM_DATA7          CONFIG_EXAMPLE_PIN_NUM_DATA7
#if CONFIG_EXAMPLE_LCD_I80_BUS_WIDTH > 8
#define EXAMPLE_PIN_NUM_DATA8          CONFIG_EXAMPLE_PIN_NUM_DATA8
#define EXAMPLE_PIN_NUM_DATA9          CONFIG_EXAMPLE_PIN_NUM_DATA9
#define EXAMPLE_PIN_NUM_DATA10         CONFIG_EXAMPLE_PIN_NUM_DATA10
#define EXAMPLE_PIN_NUM_DATA11         CONFIG_EXAMPLE_PIN_NUM_DATA11
#define EXAMPLE_PIN_NUM_DATA12         CONFIG_EXAMPLE_PIN_NUM_DATA12
#define EXAMPLE_PIN_NUM_DATA13         CONFIG_EXAMPLE_PIN_NUM_DATA13
#define EXAMPLE_PIN_NUM_DATA14         CONFIG_EXAMPLE_PIN_NUM_DATA14
#define EXAMPLE_PIN_NUM_DATA15         CONFIG_EXAMPLE_PIN_NUM_DATA15
#endif
#define EXAMPLE_PIN_NUM_PCLK           CONFIG_EXAMPLE_PIN_NUM_PCLK
#define EXAMPLE_PIN_NUM_CS             CONFIG_EXAMPLE_PIN_NUM_CS
#define EXAMPLE_PIN_NUM_DC             CONFIG_EXAMPLE_PIN_NUM_DC
#define EXAMPLE_PIN_NUM_RST            CONFIG_EXAMPLE_PIN_NUM_RST
#define EXAMPLE_PIN_NUM_BK_LIGHT       CONFIG_EXAMPLE_PIN_NUM_BK_LIGHT

// The pixel number in horizontal and vertical
#define EXAMPLE_LCD_H_RES              240
#define EXAMPLE_LCD_V_RES              320
// Bit number used to represent command and parameter
#if CONFIG_EXAMPLE_LCD_I80_CONTROLLER_ST7789
#define EXAMPLE_LCD_CMD_BITS           8
#define EXAMPLE_LCD_PARAM_BITS         8
#elif CONFIG_EXAMPLE_LCD_I80_CONTROLLER_NT35510
#define EXAMPLE_LCD_CMD_BITS           16
#define EXAMPLE_LCD_PARAM_BITS         16
#elif CONFIG_EXAMPLE_LCD_I80_CONTROLLER_ILI9341
#define EXAMPLE_LCD_CMD_BITS           8
#define EXAMPLE_LCD_PARAM_BITS         8
#endif


#define EXAMPLE_DMA_BURST_SIZE         64 // 16, 32, 64. Higher burst size can improve the performance when the DMA buffer comes from PSRAM
#define PIN_POWER_ON 15  // LCD电源控制引脚
#define PIN_NUM_RD      9   // RD引脚
#define LCD_TE_PIN      21   // TE引脚

static const char *TAG = "sst7789";

init_override(sst7789_device_init_impl);
ondraw_override(sst7789_device_ondraw_impl);

// 初始化RD引脚
void rd_pin_init(void) {
    gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << PIN_NUM_RD),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            //.pull_down_en = GPIO_PULLDOWN_DISABLE,
            //.intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    // 设置RD为高电平（读禁止）
    gpio_set_level(PIN_NUM_RD, 1);
}

// 如果需要读取操作
void lcd_read_enable(bool enable) {
    gpio_set_level(PIN_NUM_RD, enable ? 0 : 1);
}

void example_init_i80_bus(esp_lcd_panel_io_handle_t *io_handle)
{
    ESP_LOGI(TAG, "Initialize Intel 8080 bus");
    esp_lcd_i80_bus_handle_t i80_bus = NULL;
    esp_lcd_i80_bus_config_t bus_config = {
            .clk_src = LCD_CLK_SRC_DEFAULT,
            .dc_gpio_num = EXAMPLE_PIN_NUM_DC,
            .wr_gpio_num = EXAMPLE_PIN_NUM_PCLK,
            .data_gpio_nums = {
                    EXAMPLE_PIN_NUM_DATA0,
                    EXAMPLE_PIN_NUM_DATA1,
                    EXAMPLE_PIN_NUM_DATA2,
                    EXAMPLE_PIN_NUM_DATA3,
                    EXAMPLE_PIN_NUM_DATA4,
                    EXAMPLE_PIN_NUM_DATA5,
                    EXAMPLE_PIN_NUM_DATA6,
                    EXAMPLE_PIN_NUM_DATA7,
#if CONFIG_EXAMPLE_LCD_I80_BUS_WIDTH > 8
                    EXAMPLE_PIN_NUM_DATA8,
            EXAMPLE_PIN_NUM_DATA9,
            EXAMPLE_PIN_NUM_DATA10,
            EXAMPLE_PIN_NUM_DATA11,
            EXAMPLE_PIN_NUM_DATA12,
            EXAMPLE_PIN_NUM_DATA13,
            EXAMPLE_PIN_NUM_DATA14,
            EXAMPLE_PIN_NUM_DATA15,
#endif
            },
            .bus_width = CONFIG_EXAMPLE_LCD_I80_BUS_WIDTH,
            .max_transfer_bytes = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES * sizeof(uint16_t),
            .dma_burst_size = EXAMPLE_DMA_BURST_SIZE,
    };
    ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &i80_bus));

    esp_lcd_panel_io_i80_config_t io_config = {
            .cs_gpio_num = EXAMPLE_PIN_NUM_CS,
            .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
            .trans_queue_depth = 10,
            .dc_levels = {
                    .dc_idle_level = 0,
                    .dc_cmd_level = 0,
                    .dc_dummy_level = 0,
                    .dc_data_level = 1,
            },
            .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,
            .lcd_param_bits = EXAMPLE_LCD_PARAM_BITS,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, io_handle));
}

void example_init_lcd_panel(esp_lcd_panel_io_handle_t io_handle, esp_lcd_panel_handle_t *panel)
{
    ESP_LOGI(TAG, "Install LCD driver of st7789");
    esp_lcd_panel_dev_config_t panel_config = {
            .reset_gpio_num = EXAMPLE_PIN_NUM_RST,
            .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
            .data_endian = LCD_RGB_DATA_ENDIAN_LITTLE,
            .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, panel));
    esp_lcd_panel_reset(*panel);
    esp_lcd_panel_init(*panel);
    // Set inversion, x/y coordinate order, x/y mirror according to your LCD module spec
    // the gap is LCD panel specific, even panels with the same driver IC, can have different gap value
    esp_lcd_panel_invert_color(*panel, true);
    esp_lcd_panel_set_gap(*panel, 0, 0);
}


// 定义VSYNC事件回调函数
static void vsync_timer_callback(void* arg) {
    Sst7789_device* device = (Sst7789_device*)arg;
    // 在此处设置VSYNC信号标志或通知任务
    // 例如：xTaskNotifyFromISR(your_task_handle, 0, eNoAction, NULL);
    // 或者简单地置位一个全局变量：g_vsync_flag = true;
    // 发送 VSync 信号
    xSemaphoreGive(device->vsync_semaphore);
}



void init_softvsync(Sst7789_device* self) {
    // 创建信号量
    self->vsync_semaphore = xSemaphoreCreateBinary();
    // 创建并启动VSYNC定时器
    esp_timer_handle_t vsync_timer = NULL;
    esp_timer_create_args_t timer_args = {
            .callback = &vsync_timer_callback,  // 回调函数
            .arg = self,                        // 传递给回调的参数
            .dispatch_method = ESP_TIMER_TASK,  // 回调在定时器任务中执行（非中断上下文，更安全）
            .name = "virtual_vsync"             // 定时器名称（调试用）
    };
    // 创建定时器
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &vsync_timer));
// 启动周期为16666微秒（即16.666ms）的周期定时器
    ESP_ERROR_CHECK(esp_timer_start_periodic(vsync_timer, 20000));//20000us
}
// VSync中断处理
static void IRAM_ATTR vsync_isr_handler(void* arg) {
    Sst7789_device* device = (Sst7789_device*)arg;
    xSemaphoreGive(device->vsync_semaphore);
}
// 初始化VSync中断
void init_vsync_interrupt(Sst7789_device* self) {
    self->vsync_semaphore = xSemaphoreCreateBinary();
    // 配置TE引脚为输入
    gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << LCD_TE_PIN),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_POSEDGE  // 上升沿触发（可配置）
    };
    gpio_config(&io_conf);

    // 创建信号量
    //vsync_semaphore = xSemaphoreCreateBinary();
    //buffer_mutex = xSemaphoreCreateMutex();
    // 安装ISR服务
    gpio_install_isr_service(0);
    // 添加中断处理程序
    gpio_isr_handler_add(LCD_TE_PIN, vsync_isr_handler, self);
    // 使能中断
    gpio_intr_enable(LCD_TE_PIN);
}

// 析构函数声明
static void sst7789_device_destroy(Sst7789_device* self);

// TODO: 初始化数据成员
static const Sst7789_deviceFun sst7789_device_fun = {
    .destroy = sst7789_device_destroy,
};
// 构造函数实现
Sst7789_device* sst7789_device_create() {
    Sst7789_device* obj = (Sst7789_device*)malloc(sizeof(Sst7789_device));
    if (obj) {
        memset(obj, 0, sizeof(Sst7789_device));
        sst7789_device_init(obj);
    }
    return obj;
}

void sst7789_device_init(Sst7789_device* self) {
    if (GET_SST7789_DEVICE_VTABLE(self) == NULL) {
        GET_SST7789_DEVICE_VTABLE(self) = (Sst7789_deviceVTable *) malloc(sizeof(Sst7789_deviceVTable));
        memset(GET_SST7789_DEVICE_VTABLE(self), 0, sizeof(Sst7789_deviceVTable));
    }
    // 初始化基类部分
    lcd_device_init(&self->base);
    self->fun = &(sst7789_device_fun);
    // TODO: 初始化派生类特有成员

	def_init(self) = sst7789_device_init_impl;
	def_ondraw(self) = sst7789_device_ondraw_impl;
}

void sst7789_device_deinit(Sst7789_device* self) {
    if (GET_SST7789_DEVICE_VTABLE(self) != NULL) {
        free(GET_SST7789_DEVICE_VTABLE(self));
        GET_SST7789_DEVICE_VTABLE(self) = NULL;
    }
    lcd_device_deinit(GET_LCD_DEVICE(self));
    // TODO: 数据成员申请资源释放
}
// 析构函数实现
static void sst7789_device_destroy(Sst7789_device* self) {
    if (self != NULL) {
        sst7789_device_deinit(self);
        free(self);
    }
}

// init method
init_override(sst7789_device_init_impl) {
    // TODO: add init method
    Sst7789_device *sst7789_device = (Sst7789_device *)self;
    //params 
    // 1. 初始化RD引脚
    rd_pin_init();
#if EXAMPLE_PIN_NUM_BK_LIGHT >= 0
    ESP_LOGI(TAG, "Turn off LCD backlight");
    gpio_config_t bk_gpio_config = {
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = 1ULL << EXAMPLE_PIN_NUM_BK_LIGHT
    };
    gpio_config_t lcden_gpio_config = {
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = 1ULL << PIN_POWER_ON
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL);
    ESP_ERROR_CHECK(gpio_config(&lcden_gpio_config));
    gpio_set_level(PIN_POWER_ON, 1);


#endif // EXAMPLE_PIN_NUM_BK_LIGHT >= 0

#if CONFIG_EXAMPLE_LCD_IMAGE_FROM_FILE_SYSTEM
    example_init_filesystem();
#endif // CONFIG_EXAMPLE_LCD_IMAGE_FROM_FILE_SYSTEM

    self->io_handle = NULL;
    example_init_i80_bus(&self->io_handle);

    self->panel_handle = NULL;
    example_init_lcd_panel(self->io_handle, &self->panel_handle);
    // Stub: user can flush pre-defined pattern to the screen before we turn on the screen or backlight
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(self->panel_handle, true));
#if EXAMPLE_PIN_NUM_BK_LIGHT >= 0
    ESP_LOGI(TAG, "Turn on LCD backlight");
    gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL);
#endif // EXAMPLE_PIN_NUM_BK_LIGHT >= 0

    // it's recommended to choose the size of the draw buffer(s) to be at least 1/10 screen sized
    self->framebuf.draw_buffer_sz = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES * sizeof(uint16_t);
    // alloc draw buffers used by LVGL
    uint32_t draw_buf_alloc_caps = 0;
#if CONFIG_EXAMPLE_LCD_I80_COLOR_IN_PSRAM
    draw_buf_alloc_caps |= MALLOC_CAP_SPIRAM;
#endif
    self->framebuf.frontbuf = esp_lcd_i80_alloc_draw_buffer(self->io_handle, self->framebuf.draw_buffer_sz, draw_buf_alloc_caps);
    //self->framebuf.backbuf = esp_lcd_i80_alloc_draw_buffer(self->io_handle, self->framebuf.draw_buffer_sz, draw_buf_alloc_caps);
    assert(self->framebuf.frontbuf);
    //assert(self->framebuf.backbuf);
    self->framebuf.currentbuf = self->framebuf.frontbuf;
    ESP_LOGI(TAG, "currentbuf ok");
    sst7789_device->start_time = esp_timer_get_time();
    sst7789_device->end_time = sst7789_device->start_time;
    sst7789_device->colortest = 0xf800;
    //sst7789_device->vsync_triggered = false;

    //init_softvsync(sst7789_device);
    init_vsync_interrupt(sst7789_device);
    memset(self->framebuf.currentbuf, 0xff, self->framebuf.draw_buffer_sz);
    //memset(self->framebuf.backbuf, 0xff, self->framebuf.draw_buffer_sz);
    esp_lcd_panel_draw_bitmap(self->panel_handle, 0, 0, 240, 320, self->framebuf.currentbuf);
}
// ondraw method
ondraw_override(sst7789_device_ondraw_impl) {
    Sst7789_device *sst7789_device = (Sst7789_device *)self;
    //params 
    if (xSemaphoreTake(sst7789_device->vsync_semaphore, 20) == pdTRUE) {
        esp_lcd_panel_draw_bitmap(self->panel_handle, 0, 0, 240, 320, self->framebuf.currentbuf);
    }
}

