#include <string.h>

#include "Mockgpio_interface.h"
#include "Mockmmio_ops.h"
#include "dvp_device.h"

const bool dvp_instantiated[1] = {1};
DVP_Instance DVP_Instances[1] = {0};

static const GPIO_Pin pin_configurations_dvp[DVP_MAX_PINCONFIGS][16] = {
  {PA4, PA5, PA6, PA9, PA10, PC8, PC9, PC11, PB6, PB8, PB9, PC10, PC12, PD6, PD2},
  {PA4, PA5, PA6, PA9, PA10, PC8, PC9, PC11, PB3, PB8, PB9, PC10, PC12, PD6, PD2},
};

void setUp(void)
{
  DVP_Instances[0].base = DVP_BASE;
  DVP_Instances[0].rcc_devid = RCC_DVPEN;
  DVP_Instances[0].irqn = DVP_IRQn;
  DVP_Instances[0].enabled = false;
  DVP_Instances[0].configured = false;
  DVP_Instances[0].chosen_pinconfig = DVP_DEFAULT_MAPPING;

  memcpy(&DVP_Instances[0].pin_configuration, pin_configurations_dvp, sizeof(pin_configurations_dvp));
}

void tearDown(void)
{
  memset(&DVP_Instances[0], 0, sizeof(DVP_Instances));
}

int main(void)
{
  UNITY_BEGIN();
  //  RUN_TEST()
  return UNITY_END();
}
