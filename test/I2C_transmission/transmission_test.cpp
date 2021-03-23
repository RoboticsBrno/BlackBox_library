#include <thread>
#include <unity.h>

#include <library/BlackBox_I2C.hpp>

static void communicate() {
    std::uint8_t manufacturerArr[2];
    std::uint8_t deviceIDArr[2];

    I2C::Device dev(0x2A, 0);

    dev.readBytes(0x7E, manufacturerArr, 2);
    std::uint16_t manufacturer = (manufacturerArr[0] << 8) | manufacturerArr[1];

    dev.readBytes(0x7F, deviceIDArr, 2);
    std::uint16_t deviceID = (deviceIDArr[0] << 8) | deviceIDArr[1];

    printf("Device ID: %x\n", deviceID);
    
    switch (deviceID) {
    case 0x3054: // LDC13XX
        printf("Chip is LDC13XX.\n");
        break;

    case 0x3055: // LDC13XX
        printf("Chip is LDC16XX.\n");
        break;

    default:
        TEST_FAIL_MESSAGE("Wrong chip ID");
        break;
    }

    TEST_ASSERT_EQUAL(0x5449, manufacturer);
}

extern "C" {
void app_main() {

    UNITY_BEGIN();

    i2c_config_t conf_master = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = BlackBox::Pins::I2C::SDA,
        .scl_io_num = BlackBox::Pins::I2C::SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {
            .clk_speed = 400000,
        }
    };
    I2C::Ports::init(I2C_NUM_0, conf_master);

    RUN_TEST(communicate);

    UNITY_END();
}
}