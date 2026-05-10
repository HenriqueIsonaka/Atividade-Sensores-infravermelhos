#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <pwm_z42.h>

#define TPM_MODULE          1000
#define PORTA_A             DT_NODELABEL(gpioa)

// Definição dos LEDs via DeviceTree
static const struct gpio_dt_spec led_blue = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

#define SENSOR_A_PIN        12      //PTA12
#define SENSOR_B_PIN        13      //PTA13

typedef enum{ Desligado, Azul, Verde, Ambos } carrinho_t;     // Máquina de Estados

int main(void){

    const struct device *gpioa_dev = DEVICE_DT_GET(PORTA_A);                    
    if(!device_is_ready(gpioa_dev)) return 0;
    

    gpio_pin_configure(gpioa_dev, SENSOR_A_PIN, GPIO_INPUT | GPIO_PULL_UP);     //3. Sensores (Entradas com Pull-up)
    gpio_pin_configure(gpioa_dev, SENSOR_B_PIN, GPIO_INPUT | GPIO_PULL_UP);

    if(!gpio_is_ready_dt(&led_blue) || !gpio_is_ready_dt(&led_green)) return 0;  //4. LEDs
    gpio_pin_configure_dt(&led_blue, GPIO_OUTPUT_INACTIVE);                 
    gpio_pin_configure_dt(&led_green, GPIO_OUTPUT_INACTIVE);

    carrinho_t estado_atual = Desligado;           // VARIÁVEIS DE CONTROLE
    bool Sensor_A, Sensor_B;                    // VARIÁVEIS DE CONTROLE

    while(1){       
        Sensor_A = (gpio_pin_get(gpioa_dev, SENSOR_A_PIN) == 0);    //1. LEITURA DOS SENSORES (Inverter Lógica)
        Sensor_B = (gpio_pin_get(gpioa_dev, SENSOR_B_PIN) == 0);    // o Sensor retorna 0 quando detecta algo, isso inverte essa lógica, agora retornando 1.

        if(Sensor_A && Sensor_B)            estado_atual = Ambos;   //2. DEFINIÇÃO DO PRÓXIMO ESTADO
        else if(Sensor_A && !Sensor_B)      estado_atual = Azul;
        else if(!Sensor_A && Sensor_B)      estado_atual = Verde;
        else                                estado_atual = Desligado; 

        switch(estado_atual){                                       //3. AÇÕES DA MÁQUINA DE ESTADOS
            case Azul:
                gpio_pin_set_dt(&led_blue, 1);      // LED azul
                gpio_pin_set_dt(&led_green, 0);
                break;
            case Verde:
                gpio_pin_set_dt(&led_blue, 0);
                gpio_pin_set_dt(&led_green, 1);         // LED Verde 
                break;
            case Ambos:
                gpio_pin_set_dt(&led_blue, 1);      // LED Azul claro
                gpio_pin_set_dt(&led_green, 1);        
                break;
            case Desligado:
            default:
                gpio_pin_set_dt(&led_blue, 0);      // LED Apagado;
                gpio_pin_set_dt(&led_green, 0);
                break;
        }
            k_msleep(10);   // Aguarda 10ms (100Hz)
    }
    return 0;
}
//PINAGEM FÍSICA
//                              Microcontrolar              Ponte H
//MOTOR A - PWM                 PTB2                        ENA
//MOTOR B - PWM                 PTB3                        ENB
//MOTOR A - DIREÇÃO 1           PTA1                        IN1
//MOTOR A - DIREÇÃO 2           PTA2                        IN2
//MOTOR B - DIREÇÃO 1           PTA4                        IN3
//MOTOR B - DIREÇÃO 2           PTA5                        IN4
//SENSOR A (CONTROLA MOTOR A)   PTA12
//SENSOR B (CONTROLA MOTOR B)   PTA13
