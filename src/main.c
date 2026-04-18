
#include <zephyr/kernel.h>        // Funções básicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>        // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>  // API para controle de pinos de entrada/saída (GPIO)
#include <pwm_z42.h>        // Biblioteca personalizada com funções de controle do TPM (Timer/PWM Module)

#define PTA_NODE 
// Define o valor do registrador MOD do TPM para configurar o período do PWM
#define TPM_MODULE 1000  // Define a frequência do PWM fpwm = (TPM_CLK / (TPM_MODULE * PS))
// Valores de duty cycle correspondentes a diferentes larguras de pulso
uint16_t vel_25  = TPM_MODULE/4;  
uint16_t vel_10  = TPM_MODULE/10; 
uint16_t vel_100  = TPM_MODULE/1;

int main(void)
{
    // Inicializa o módulo TPM2 com:
    // - base do TPMx
    // - fonte de clock PLL/FLL (TPM_CLK)
    // - valor do registrador MOD
    // - tipo de clock (TPM_CLK)
    // - prescaler de 1 a 128 (PS)
    // - modo de operação EDGE_PWM
    pwm_tpm_Init(TPM1, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Init(TPM2, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);

    pwm_tpm_Ch_Init(TPM1, 0, TPM_PWM_H, GPIOB, 0);
    pwm_tpm_Ch_Init(TPM2, 1, TPM_PWM_H, GPIOB, 3);// até aqui está ok(pino PTB0 E PTB3)
    
    const struct device *gpioa_apontador = DEVICE_DT_GET(DT_NODELABEL(gpioa));
    
    if (!device_is_ready(gpioa_apontador)) {
        return 0;
    }

    gpio_pin_configure(gpioa_apontador, 1, GPIO_OUTPUT_ACTIVE);// motor a in1(pino PTA1)
    gpio_pin_configure(gpioa_apontador, 2, GPIO_OUTPUT_INACTIVE);//motor a in2(pino PTA2)
    gpio_pin_configure(gpioa_apontador, 12, GPIO_OUTPUT_ACTIVE);//motor b in3(pino PTA12)
    gpio_pin_configure(gpioa_apontador, 4, GPIO_OUTPUT_INACTIVE);// motor b in4(pino PTA 4)


    // Loop infinito
    for(;;) {
        pwm_tpm_CnV(TPM1, 0, vel_100);
        pwm_tpm_CnV(TPM2, 1, vel_100);
        k_msleep(4000);
        pwm_tpm_CnV(TPM1, 0, vel_25);
        k_msleep(4000);
        pwm_tpm_CnV(TPM1, 0, vel_10);
        k_msleep(4000);
    }
    return 0;
}