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

float TEST = 0.0;
uint8_t ACCEL_OUT = 0;
uint8_t REGEN_OUT = 0;

void CPP_UserSetup(void)
{
  // Initialize routine that sends telemetry data
  telem_tx_timer_id = osTimerNew((osThreadFunc_t)SendTelemetryData, osTimerPeriodic, NULL, &telem_tx_timer_attr);
  if (telem_tx_timer_id == NULL)
  {
      Error_Handler();
  }
  // Initialize routine that sends CAN Data
  can_tx_timer_id = osTimerNew((osThreadFunc_t)SendCanMsgs, osTimerPeriodic, NULL, &can_tx_timer_attr);
  if (can_tx_timer_id == NULL)
  {
      Error_Handler();
  }
  // Mitsuba Stuff
  CANController.AddRxModule(&Motor_Rx_0);
  CANController.AddRxModule(&Motor_Rx_1);
  CANController.AddRxModule(&Motor_Rx_2);
  // Orion Stuff
  CANController.AddRxModule(&BMS_Rx_0);
  CANController.AddRxModule(&BMS_Rx_1);
  CANController.AddRxModule(&BMS_Rx_2);
  CANController.AddRxModule(&BMS_Rx_3);
  CANController.AddRxModule(&BMS_Rx_4);
  CANController.AddRxModule(&BMS_Rx_5);
  // Ready CAN
  CANController.Init();
  // Start Timers
  osTimerStart(telem_tx_timer_id, 1000);  // Pit Transmission
  osTimerStart(can_tx_timer_id, 5000);    // CAN Tx Transmission
  // Initialize DACs
  accel.SetRefVcc();
  regen.SetRefVcc();
}

void SendCanMsgs()
{
  // Request Mitsuba Data
  Motor_Tx.SetRequestAllFrames();
  CANController.Send(&Motor_Tx);
  accel.WriteAndUpdate(ACCEL_OUT);
  regen.WriteAndUpdate(REGEN_OUT);
}

void SendTelemetryData()
{
  pit.SendDataModule(Motor_Rx_0);
  TEST = BMS_Rx_0.getAvgCellVolt();
  TEST = BMS_Rx_4.getPackSoc();
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CANController.SetRxFlag();
  HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}
