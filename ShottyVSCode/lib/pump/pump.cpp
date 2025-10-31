#include "pump.h"

/***************************************************************************************************/
void pump_init(void)
{
  pinMode(RELAIS_PIN, OUTPUT);
} /* pump_init */


/***************************************************************************************************/
void pump_start(void)
{
  digitalWrite(RELAIS_PIN, HIGH);   /* Activate pump */
} /* pump_start */


/***************************************************************************************************/
void pump_stop(void)
{
  digitalWrite(RELAIS_PIN, LOW);   /* Deactivate pump */
} /* pump_stop */
