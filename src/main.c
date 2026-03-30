
#define SIM_SCGC5 (*((volatile unsigned int*)0x40048038))
#define GPIOB_PDDR (*((volatile unsigned int*)0x400FF054))
#define PORTB_PCR19 (*((volatile unsigned int*)0x4004A04C))
#define GPIOB_PTOR (*((volatile unsigned int*)0x400FF04C))


void delayMs (int n) {
  volatile int i; 
  volatile int j; 
  for (i = 0; i < n; i++) 
    for (j = 0; j < 4000; j++) {} // tive que mudar o valor máximo de j para dar um Ms
}

int main (void) {
  void delayMs(int n);
  SIM_SCGC5|= (1<<10);  // habilita o clock da porta B
  GPIOB_PDDR|= (1<<19);  // pin é configurado para output para a função gpio
  PORTB_PCR19|= (1<<8); // GPIO (001)
  PORTB_PCR19&= ~(1<<9); //  GPIO (001)
  PORTB_PCR19&= ~(1<<10); //GPIO (001) ->and com …111101111…
  int n= 1000, a=3;
  int i, j;
  while (a==3) {
	GPIOB_PTOR|= (1<<19);
    delayMs(1000);
    GPIOB_PTOR&= ~(1<<19);
	delayMs(1000);
  }
}