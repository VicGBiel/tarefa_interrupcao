#include <stdio.h>   // Funções de entrada e saída padrão (ex.: printf, usado para mensagens no console)
#include <stdlib.h>  // Funções padrão (ex.: rand, usado para gerar números aleatórios)
#include "pico/stdlib.h"  // Funções padrão do Raspberry Pi Pico (GPIO, temporização, inicialização)
#include "hardware/pio.h"  // Controle do PIO (Programmable Input/Output, usado para os LEDs WS2812)
#include "hardware/dma.h"  // Controle do DMA (Direct Memory Access, usado para atualizar LEDs)
#include "pico/bootrom.h"  // Funções relacionadas ao bootloader (ex.: reset_usb_boot para reinício no modo bootloader)
#include "ws2812.pio.h"  // Programa PIO e inicialização para controlar os LEDs WS2812

// Definições de pinos e configurações do hardware
#define btn_a 5 // Definição do botão A
#define btn_b 6 // Definição do botão B
#define led_pin_red 13 // Definição do LED vermelho
#define IS_RGBW false // Define se os LEDs são RGBW ou apenas RGB
#define NUM_PIXELS 25 // Número de LEDs na matriz
#define WS2812_PIN 7 // Pino onde os LEDs WS2812 estão conectados

// Variáveis globais para armazenar a cor do LED
uint8_t led_r = 10; // Intensidade do vermelho
uint8_t led_g = 8;  // Intensidade do verde
uint8_t led_b = 0;  // Intensidade do azul

// Buffer para armazenar o estado de cada LED
bool led_buffer[NUM_PIXELS]= {}; // Inicializa o buffer de LEDs com zeros

volatile int estado = 0; // Variável global para armazenar o estado atual da animação
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

// Função para representar a cor em formato RGB
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Envia um pixel para o barramento WS2812
static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Atualiza todos os LEDs com base no buffer
void atualizaFita(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = urgb_u32(r, g, b); // Define a cor
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (led_buffer[i]) {
            put_pixel(color); // Liga o LED conforme o buffer
        } else {
            put_pixel(0);  // Desliga os LEDs
        }
    }
}

// Declaração antecipada das funções
void atualizaEstado(int estado);
void initGPIO();
static void gpio_irq_handler(uint gpio, uint32_t events);

// Função principal
int main() {
    initGPIO(); // Inicializa os pinos GPIO

    // Configura o controlador de LEDs WS2812
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    atualizaEstado(0);

    // Configuração das interrupções dos botões
    gpio_set_irq_enabled_with_callback(btn_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(btn_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Loop principal piscando um LED vermelho 5 vezes por segundo
    while (true) {
        gpio_put(led_pin_red, 1);
        sleep_ms(100);
        gpio_put(led_pin_red, 0);
        sleep_ms(100);
    }
}

// Inicializa os pinos GPIO
void initGPIO() {
    //inicializção do led vermelho
    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, true);
    //inicialização do botão A
    gpio_init(btn_a);
    gpio_set_dir(btn_a, false);
    gpio_pull_up(btn_a);
    //inicialização do botão B
    gpio_init(btn_b);
    gpio_set_dir(btn_b, false);
    gpio_pull_up(btn_b);
}

// Tratador de interrupção dos botões
static void gpio_irq_handler(uint gpio, uint32_t events) {
    static int estadoAnterior = -1;
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    //verifica qual o botão apertado, o limite numérico e se passou tempo suficiente desde o último evento
    if (gpio == btn_a && estado < 9 && current_time - last_time > 200000) {     //200 milissegundos de debouncing
        last_time = current_time;
        estado++;
        atualizaEstado(estado);
        estadoAnterior = estado;
    }
    if (gpio == btn_b && estado > 0 && current_time - last_time > 200000) {
        last_time = current_time;
        estado--;
        atualizaEstado(estado);
        estadoAnterior = estado;
    }
}


void atualizaEstado(int estado) {

    switch(estado){
        case 0:         
            bool led_buffer_0 [] = { 
                1, 1, 1, 0, 0, 
                0, 0, 1, 0, 1, 
                1, 0, 1, 0, 0, 
                0, 0, 1, 0, 1, 
                1, 1, 1, 0, 0
            };
            memcpy(led_buffer, led_buffer_0, sizeof(led_buffer));
            atualizaFita(led_r, led_g, led_b);
            break;

        case 1:         
            bool led_buffer_1 [NUM_PIXELS] = {
                1, 0, 0, 0, 0, 
                0, 0, 0, 0, 1, 
                1, 0, 0, 0, 0, 
                0, 0, 0, 0, 1, 
                1, 0, 0, 0, 0
            };
            memcpy(led_buffer, led_buffer_1, sizeof(led_buffer));
            atualizaFita(led_r, led_g, led_b);
            break;

        case 2:         
            bool led_buffer_2 [NUM_PIXELS] = {
                1, 1, 1, 0, 0, 
                0, 0, 1, 0, 0, 
                1, 1, 1, 0, 0, 
                0, 0, 0, 0, 1, 
                1, 1, 1, 0, 0
            };
            memcpy(led_buffer, led_buffer_2, sizeof(led_buffer));
            atualizaFita(led_r, led_g, led_b);
            break;

        case 3:         
            bool led_buffer_3 [NUM_PIXELS] = {
                1, 1, 1, 0, 0, 
                0, 0, 0, 0, 1, 
                1, 1, 1, 0, 0, 
                0, 0, 0, 0, 1, 
                1, 1, 1, 0, 0
            };
            memcpy(led_buffer, led_buffer_3, sizeof(led_buffer));
            atualizaFita(led_r, led_g, led_b);
            break;

        case 4:         
            bool led_buffer_4 [NUM_PIXELS] = {
                1, 0, 0, 0, 0, 
                0, 0, 0, 0, 1, 
                1, 1, 1, 0, 0, 
                0, 0, 1, 0, 1, 
                1, 0, 1, 0, 0
            };
            memcpy(led_buffer, led_buffer_4, sizeof(led_buffer));
            atualizaFita(led_r, led_g, led_b);
            break;

        case 5:         
            bool led_buffer_5 [NUM_PIXELS] = {
                1, 1, 1, 0, 0, 
                0, 0, 0, 0, 1, 
                1, 1, 1, 0, 0, 
                0, 0, 1, 0, 0, 
                1, 1, 1, 0, 0
            };
            memcpy(led_buffer, led_buffer_5, sizeof(led_buffer));
            atualizaFita(led_r, led_g, led_b);
            break;

        case 6:         
            bool led_buffer_6 [NUM_PIXELS] = {
                1, 1, 1, 0, 0, 
                0, 0, 1, 0, 1, 
                1, 1, 1, 0, 0, 
                0, 0, 1, 0, 0, 
                1, 1, 1, 0, 0
            };
            memcpy(led_buffer, led_buffer_6, sizeof(led_buffer));
            atualizaFita(led_r, led_g, led_b);
            break;
        
        case 7:         
            bool led_buffer_7 [NUM_PIXELS] = {
                1, 0, 0, 0, 0, 
                0, 0, 0, 0, 1, 
                1, 0, 0, 0, 0, 
                0, 0, 0, 0, 1, 
                1, 1, 1, 0, 0
            };
            memcpy(led_buffer, led_buffer_7, sizeof(led_buffer));
            atualizaFita(led_r, led_g, led_b);
            break;

        case 8:         
            bool led_buffer_8 [NUM_PIXELS] = {
                1, 1, 1, 0, 0, 
                0, 0, 1, 0, 1, 
                1, 1, 1, 0, 0, 
                0, 0, 1, 0, 1, 
                1, 1, 1, 0, 0
            };
            memcpy(led_buffer, led_buffer_8, sizeof(led_buffer));
            atualizaFita(led_r, led_g, led_b);
            break;
        
        case 9:         
            bool led_buffer_9 [NUM_PIXELS] = {
                1, 1, 1, 0, 0, 
                0, 0, 0, 0, 1, 
                1, 1, 1, 0, 0, 
                0, 0, 1, 0, 1, 
                1, 1, 1, 0, 0
            };
            memcpy(led_buffer, led_buffer_9, sizeof(led_buffer));
            atualizaFita(led_r, led_g, led_b);
            break;
    }
}