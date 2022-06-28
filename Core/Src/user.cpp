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
void UpdateThrottle();
uint8_t CalcRegen(float* acceleration);

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
osTimerId_t speed_control_timer_id;
osTimerAttr_t speed_control_timer_attr =
{
    .name = "Speed"
};



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
  // Initialize routine that updates regen and throttle
  speed_control_timer_id = osTimerNew((osThreadFunc_t)UpdateThrottle, osTimerPeriodic, NULL, &speed_control_timer_attr);
  if (speed_control_timer_id == NULL)
  {
      Error_Handler();
  }
  // Front Lights (for throttle)
  CANController.AddRxModule(&FLights);
  CANController.AddRxModule(&RLights);
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
  osTimerStart(speed_control_timer_id, 100);    // Mitsuba throttle and regen
}

void SendCanMsgs()
{
  // Request Mitsuba Data
  Motor_Tx.SetRequestAllFrames();
  CANController.Send(&Motor_Tx);
}

void SendTelemetryData()
{
  pit.SendDataModule(Motor_Rx_0);
  pit.SendDataModule(BMS_Rx_0);
  pit.SendDataModule(BMS_Rx_4);
//  TEST = BMS_Rx_0.getAvgCellVolt();
//  TEST = BMS_Rx_4.getPackSoc();
}

void UpdateThrottle()
{
  uint8_t adjThrottleVal = static_cast<uint8_t>(FLights.GetThrottleVal() >> 5);
  // Probs dont want to do the below would be better to drop two bits then map 12 bits to 18 bits
  accel.WriteAndUpdate(adjThrottleVal); // shift over b\c we are sending 14 bit ADC to 8 bit DAC
  // If the throttle is 0 then we should regen so that we are hitting a 0.2g *deceleration*
  // TODO: Should probably have a && REGEN_ENABLED
  if(adjThrottleVal == 0)
  {
//    // Read IMU to get accel info for PID
//    LSM6DSR_Axes_t accel_info;
//    LSM6DSR_ACC_GetAxes(&imu, &accel_info);
//    // Calculate regen value
//    // Write regen value to motor controller
//    uint8_t regenVal = 0; // CalcRegen();
//    regen.WriteAndUpdate(regenVal);
  }
}

uint8_t CalcRegen(float* acceleration)
{
  // Variables (Static to keep off of stack)
  static float goal = 0.2;   // goal deceleration
  static uint16_t Pgain=1275; // proportional control gain
  static uint16_t Igain=2;    // integral control gain
  static uint16_t Dgain=50;   // derivative control gain
  // Calculate error
  float error = goal - acceleration[0]; // goal - most recent acceleration value
  // Proportion control
  float prop = Pgain*error;
  float deriv = 0; // TODO: Calc mean of the acceleration
  // Derivative control
  float D = Dgain*deriv;
  // Volume to regen
  return prop - D;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CANController.SetRxFlag();
  HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}
