#include "bsp_battery.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

#define FULL_BATTERY_MV        (4160)
#define EMPTY_BATTERY_MV        (2500)

#define BATTERY_ADC_SIZE 9

// 排序函数（冒泡排序实现，可根据需要替换为其他排序算法）
static void bubble_sort(uint16_t *data, uint16_t size)
{
    for (uint8_t i = 0; i < size - 1; i++)
    {
        for (uint8_t j = 0; j < size - i - 1; j++)
        {
            if (data[j] > data[j + 1])
            {
                uint16_t temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

static uint16_t average_filter(uint16_t *samples)
{
    uint16_t out;
    bubble_sort(samples, BATTERY_ADC_SIZE);
    for (int i = 1; i < BATTERY_ADC_SIZE - 1; i++)
    {
        out += samples[i] / (BATTERY_ADC_SIZE - 2);
    }
    return out;
}

uint16_t bsp_battery_read_raw(void)
{
    uint16_t samples[BATTERY_ADC_SIZE];
    adc_select_input(BSP_BAT_ADC_PIN - 26);
    for (int i = 0; i < BATTERY_ADC_SIZE; i++)
    {
        samples[i] = adc_read();
    }
    return average_filter(samples); // 使用中位值滤波
}

void bsp_battery_read(float *voltage, uint16_t *adc_raw)
{
    uint16_t result = bsp_battery_read_raw();
    if (adc_raw)
    {
        *adc_raw = result;
    }
    if (voltage)
    {
        *voltage = result * (3.3 / (1 << 12)) * 3.0;
    }
}

long map(long x, long in_min, long in_max, long out_min, long out_max){
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t bsp_battery_percent(uint16_t battery_power_mv){
    return map(battery_power_mv,EMPTY_BATTERY_MV,FULL_BATTERY_MV,0,100);
}

bool bsp_is_power_on(){
    return gpio_get(BSP_BAT_EN_PIN);
}

void bsp_battery_enabled(bool enabled)
{
    gpio_put(BSP_BAT_EN_PIN, enabled);
}


void bsp_battery_init(uint16_t key_wait_ms)
{
    adc_init();
    adc_gpio_init(BSP_BAT_ADC_PIN);
    gpio_init(BSP_BAT_EN_PIN);
    gpio_set_dir(BSP_BAT_EN_PIN, GPIO_OUT);
    gpio_init(BSP_BAT_KEY_PIN);
    gpio_set_dir(BSP_BAT_KEY_PIN, GPIO_IN);

    bsp_battery_enabled(false);
    sleep_ms(key_wait_ms);
    bsp_battery_enabled(true);
}


bool bsp_battery_get_key_level(void)
{
    return gpio_get(BSP_BAT_KEY_PIN);
}

