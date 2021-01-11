#include <thread>
#include <unity.h>

#include <BlackBox_I2C.hpp>

static void i2cMasterInit() {
    i2c_config_t conf_master = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 21,
        .scl_io_num = 22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {.clk_speed = 400000,}
    };
    I2C::Ports::init(I2C_NUM_0, conf_master);
}

static void comm(){
    std::uint8_t out[2];
    I2C::Device dev(0x2B, 0);
    dev.read(0x7F, out, 2);
    printf("data: %x\n",out[0]);
    printf("data: %x\n",out[1]);
    std::uint16_t data = (out[0] << 8)|out[1];
    TEST_ASSERT_EQUAL(0x3055, data);
}

extern "C" {
void app_main() {
    UNITY_BEGIN();
    i2cMasterInit();
    RUN_TEST(comm);
    UNITY_END();
}
}