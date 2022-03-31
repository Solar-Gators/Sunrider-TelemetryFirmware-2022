/*
 * user.cpp
 *
 *  Created on: Jan 29, 2022
 *      Author: John Carr
 */

#include "user.hpp"

using namespace SolarGators;

extern "C" void CPP_UserSetup(void);

void SendCanMsgs();
void SendTelemetryData();

// OS Configs
/* Definitions for CAN Tx Thread */
osTimerId_t can_tx_timer_id;
osTimerAttr_t can_tx_timer_attr =
{
    .name = "CAN Tx"
};
osTimerId_t telem_tx_timer_id;
osTimerAttr_t telem_tx_timer_attr =
{
    .name = "Telemetry"
};

uint8_t DAC_OUT_TEST = 0;

void CPP_UserSetup(void)
{
  // Software Timer that sends telemetry data
  telem_tx_timer_id = osTimerNew((osThreadFunc_t)SendTelemetryData, osTimerPeriodic, NULL, &telem_tx_timer_attr);
  if (telem_tx_timer_id == NULL)
  {
      Error_Handler();
  }
  // Start Thread that sends CAN Data
  can_tx_timer_id = osTimerNew((osThreadFunc_t)SendCanMsgs, osTimerPeriodic, NULL, &can_tx_timer_attr);
  if (can_tx_timer_id == NULL)
  {
      Error_Handler();
  }
  CANController.AddRxModule(&motor_rx_0);
  CANController.AddRxModule(&bms);
  CANController.Init();
  // Start Timers
  osTimerStart(telem_tx_timer_id, 1000);
  osTimerStart(can_tx_timer_id, 100);
  accel.SetRefVcc();
  regen.SetRefVcc();
  regen.WriteAndUpdate(DAC_OUT_TEST);
  accel.WriteAndUpdate(0);
}

void SendCanMsgs()
{
  // Request Mitsuba Data
  motor_request.SetRequestAllFrames();
  CANController.Send(&motor_request);
}

void SendTelemetryData()
{
  pit.SendDataModule(motor_rx_0);
  regen.WriteAndUpdate(DAC_OUT_TEST);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CANController.SetRxFlag();
  HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void UpdateDACs()
{

}
