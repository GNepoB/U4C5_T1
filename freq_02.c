#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos pinos do LED RGB
#define LED_VERMELHO 13
#define LED_AMARELO 12
#define LED_VERDE 11

// Intervalos de tempo
#define INTERVALO_SEMAFORO 3000  // 3 segundos
#define INTERVALO_MENSAGEM 1000   // 1 segundo

// Estado inicial
volatile int estado = 0;

// Função de callback do temporizador para mudar o estado do semáforo
bool repeating_timer_callback(struct repeating_timer *t) {
    switch (estado) {
        case 0:
            gpio_put(LED_VERMELHO, 0);
            gpio_put(LED_AMARELO, 1);
            estado = 1;
            break;
        case 1:
            gpio_put(LED_AMARELO, 0);
            gpio_put(LED_VERDE, 1);
            estado = 2;
            break;
        case 2:
            gpio_put(LED_VERDE, 0);
            gpio_put(LED_VERMELHO, 1);
            estado = 0;
            break;
    }
    return true; // Continua a repetição do temporizador
}

int main() {
    stdio_init_all();

    // Inicializa os LEDs como saída
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(LED_AMARELO);
    gpio_set_dir(LED_AMARELO, GPIO_OUT);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);

    // Define o estado inicial: vermelho ligado
    gpio_put(LED_VERMELHO, 1);
    gpio_put(LED_AMARELO, 0);
    gpio_put(LED_VERDE, 0);

    // Configuração do temporizador repetitivo
    struct repeating_timer timer;
    add_repeating_timer_ms(INTERVALO_SEMAFORO, repeating_timer_callback, NULL, &timer);

    // Loop principal para exibir mensagens a cada segundo
    while (true) {
        printf("Sistema funcionando - Estado atual: %d\n", estado);
        sleep_ms(INTERVALO_MENSAGEM);
    }
}
