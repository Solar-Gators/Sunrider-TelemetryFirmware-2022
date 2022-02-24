
#ifndef USER_HPP_
#define USER_HPP_

#include "main.h"
// Drivers
#include "CAN.hpp"
#include "RFD900x.hpp"
#include "PitComms.hpp"
// Data Module Info
#include "DataModuleInfo.hpp"
// Data Modules
#include "Steering.hpp"
#include "OrionBMS.hpp"
#include "Proton1.hpp"
#include "Mitsuba.hpp"

extern CAN_HandleTypeDef hcan;
extern UART_HandleTypeDef huart2;

SolarGators::Drivers::CANDriver CANController(&hcan, 0);
SolarGators::Drivers::RFD900x rfd(&huart2);
SolarGators::Drivers::PitComms pit(&rfd);

// Objects we want to get status on
// TODO: Steering
SolarGators::DataModules::MitsubaRequest motor_request(SolarGators::DataModuleInfo::MOTORTX_RL_MSG_ID);

// Objects that we will want to send data to the pit on
SolarGators::DataModules::OrionBMS bms(SolarGators::DataModuleInfo::Orion_Id);
SolarGators::DataModules::MitsubaRx0 motor_rx_0(SolarGators::DataModuleInfo::MOTORRX0_RL_MSG_ID, 0x04);
// TODO: Mppt1
// TODO: Mppt2
// TODO: Mppt3

#endif
