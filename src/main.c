#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>


#define Tempo 1000
#define VM gpio_pin_get_dt(&Led2)
#define VD gpio_pin_get_dt(&Led0)

#define LED0_NODE DT_ALIAS(led0)
#define LED2_NODE DT_ALIAS(led2)


// Verifica se o LED está definido no Device Tree
#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
static const struct gpio_dt_spec Led0= GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif


#if DT_NODE_HAS_STATUS(LED2_NODE, okay)
static const struct gpio_dt_spec Led2=GPIO_DT_SPEC_GET(LED2_NODE, gpios);
#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif


void main(void)
{
    int ret;


    if (!gpio_is_ready_dt(&Led0)) {
        printk("Error: LED device %s is not ready\n", Led0.port->name);
        return;
    }
    if (!gpio_is_ready_dt(&Led2)) {
        printk("Error: LED device %s is not ready\n", Led2.port->name);
        return;
    }


    // Configura o pino como saída
    ret = gpio_pin_configure_dt(&Led0, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }
    ret = gpio_pin_configure_dt(&Led2, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }
    printk("LED blinking on %s pin %d\n", Led2.port->name, Led2.pin);
    gpio_pin_set_dt(&Led2, 0);  
    
    int verde, vermelho, amarelo;
    int estado=verde;

    while (1) {
        if(estado==verde) {
            k_msleep(3*Tempo);
            gpio_pin_set_dt(&Led2, 1);
            estado=amarelo;
        }
        if(estado==amarelo) {
            k_msleep(Tempo);
            gpio_pin_set_dt(&Led0, 0);
            estado=vermelho;
        }
        if(estado==vermelho) {
            k_msleep(4*Tempo);
            gpio_pin_set_dt(&Led0, 1);
            gpio_pin_set_dt(&Led2, 0);
            estado=verde;
        }
    }
}
