#include <stdio.h>
#include "pico/stdlib.h"

#define led_red 13

void initGPIO(){
    gpio_init(led_red);
    gpio_set_dir(led_red, GPIO_OUT);
}


int main()
{
    stdio_init_all();
    initGPIO();

    while (true) {
        gpio_put(led_red, 1);
        sleep_ms(100);
        gpio_put(led_red, 0);
        sleep_ms(100);
    }
}
