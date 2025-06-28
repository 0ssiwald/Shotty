#include "sensor.h"


/***************************************************************************************************/
void sensor_init(void)
{
  pinMode(SENSOR_PIN, INPUT);
} /* sensor_init */


/***************************************************************************************************/
int sensor_measure(void)
{
  return analogRead(SENSOR_PIN);
} /* sensor_measure */
