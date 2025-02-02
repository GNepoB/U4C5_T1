#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

//Define os pinos dos LED's
#define LED_vermelho 13
#define LED_verde 11
#define LED_amarelo 12
//Define o intervalo de tempo desejado (em ms)
#define intervalo 3000

bool led_on = true;
bool led_off = false;

int main() {
    stdio_init_all();

    // Inicializa os LED's como saída
    gpio_init(LED_vermelho);
    gpio_set_dir(LED_vermelho, GPIO_OUT);
    
    gpio_init(LED_verde);
    gpio_set_dir(LED_verde, GPIO_OUT);
    
    gpio_init(LED_amarelo);
    gpio_set_dir(LED_amarelo, GPIO_OUT);

    // Estado inicial: LED Verde ligado, os outros apagados
    int estado = 0;
    gpio_put(LED_vermelho, led_off);
    gpio_put(LED_amarelo, led_off);
    gpio_put(LED_verde, led_on);

    absolute_time_t proximo_tempo = make_timeout_time_ms(intervalo);

    while (true) {
        if (time_reached(proximo_tempo)) {
            // Atualiza o estado do semáforo (verde -> amarelo -> vermelho -> verde -> ...)
            if (estado == 0) {
                gpio_put(LED_verde, led_off);
                gpio_put(LED_amarelo, led_on);
                estado = 1;
            } else if (estado == 1) {
                gpio_put(LED_amarelo, led_off);
                gpio_put(LED_vermelho, led_on);
                estado = 2;
            } else if (estado == 2) {
                gpio_put(LED_vermelho, led_off);
                gpio_put(LED_verde, led_on);
                estado = 0;
            }

            printf("3 segundos se passaram. Alterando LED acesso");

            // Atualiza o tempo da próxima troca
            proximo_tempo = make_timeout_time_ms(intervalo);
        }

        sleep_ms(1);  // Pequena pausa para eficiência
    }
}
