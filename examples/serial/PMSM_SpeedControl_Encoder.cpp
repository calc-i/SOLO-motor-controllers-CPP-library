/**
 *******************************************************************************
 * @file    PMSM_SpeedControl_Encoder.cpp
 * @authors SOLO Motor Controllers
 * @brief   PMSM motor Speed Control with Encoder
 * 
 * @date    Date: 2024
 * @version 1.3.0
 * *******************************************************************************    
 * @attention
 * Copyright: (c) 2021-2024, SOLO motor controllers project
 * MIT License (see LICENSE file for more details)
 ******************************************************************************* 
 */

//The Motor used for Testings: teknic m-2310P-LN-04K
#include <iostream>
using std::cout;
using std::endl;

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#define Sleep(x) usleep(x*1000)
#endif

#include "SOLOMotorControllersSerial.h"

//For this Test, make sure you have calibrated your Motor and Incremental Encoders before
//to know more please read: https://www.solomotorcontrollers.com/how-to-connect-calibrate-incremental-encoder-with-solo/

//instanciate a SOLO object:
SOLOMotorControllers *solo;  

//Desired Switching or PWM Frequency at Output
long pwmFrequency = 20; 

//Motor's Number of Poles
long numberOfPoles = 8; 

// Current Limit of the Motor
float currentLimit = 7.0; 

//Motor's Number of Encoder Lines (PPR pre-quad)
long numberOfEncoderLines = 1000; 

//Speed controller Kp
float speedControllerKp = 0.15; 

//Speed controller Ki
float speedControllerKi = 0.005; 

// Battery or Bus Voltage
float busVoltage = 0; 

// Motor Torque feedback
float actualMotorTorque = 0; 

// Motor speed feedback
long actualMotorSpeed = 0; 

// Motor position feedback
long actualMotorPosition = 0; 

void soloConfigInit(char *portName) {
  //In this example, make sure you put SOLO into Closed-Loop Mode
  
  //Initialize the SOLO object
  //Equivalent, avoiding the default parameter of SOLO Device Address:  solo = new SOLOMotorControllersSerial((char*)"COM3",0);
  solo = new SOLOMotorControllersSerial(portName, 0, SOLOMotorControllers::UartBaudrate::rate115200, 100, 10, true);

  //TRY CONNECT LOOP
  while(solo->CommunicationIsWorking() == false ){
    std::cout << "Solo connection failed. Retry" << std::endl;
    Sleep(500);
    solo->Connect();
  }
  std::cout << "Solo connected!" << std::endl;

  // Initial Configuration of the device and the Motor
  solo->SetOutputPwmFrequencyKhz(pwmFrequency);
  solo->SetCurrentLimit(currentLimit);
  solo->SetMotorPolesCounts(numberOfPoles);
  solo->SetIncrementalEncoderLines(numberOfEncoderLines);
  solo->SetCommandMode(SOLOMotorControllers::CommandMode::digital);
  solo->SetMotorType(SOLOMotorControllers::MotorType::bldcPmsm);
  solo->SetFeedbackControlMode(SOLOMotorControllers::FeedbackControlMode::encoders);
  solo->SetSpeedControllerKp(speedControllerKp);
  solo->SetSpeedControllerKi(speedControllerKi);
  solo->SetControlMode(SOLOMotorControllers::ControlMode::speedMode);
 
  //run the motor identification to Auto-tune the current controller gains Kp and Ki needed for Torque Loop
  //run ID. always after selecting the Motor Type!
  //ID. doesn't need to be called everytime, only one time after wiring up the Motor will be enough
  //the ID. values will be remembered by SOLO after power recycling
  solo->MotorParametersIdentification(SOLOMotorControllers::Action::start);
  std::cout << "Identifying the Motor" << std::endl;

  //wait at least for 2sec till ID. is done
  Sleep(2000); 
}

int main(int argc, char *argv[]) {
  if(argc != 2) {
#ifdef _WIN32
    std::cout << "Usage: " << argv[0] << " <COMPort e.g. COM3>" << std::endl;
#else
    std::cout << "Usage: " << argv[0] << " <device name e.g. ttyS0>" << std::endl;
#endif
    return 1;
  }

  soloConfigInit(argv[1]);

  //Infinite Loop
  while(true){
    //set the Direction on C.C.W. 
    solo->SetMotorDirection(SOLOMotorControllers::Direction::counterclockwise);
    //set an arbitrary Positive speed reference[RPM]
    solo->SetSpeedReference(1500);
    // wait till motor reaches to the reference 
    Sleep(300);
    actualMotorSpeed = solo->GetSpeedFeedback();
    std::cout << "Measured Speed[RPM]: "<< actualMotorSpeed << std::endl;

    actualMotorTorque = solo->GetQuadratureCurrentIqFeedback();
    std::cout << "Measured Iq/Torque[A]: "<< actualMotorTorque << std::endl;
    Sleep(3000);

    //set the Direction on C.W. 
    solo->SetMotorDirection(SOLOMotorControllers::Direction::clockwise);
    //set an arbitrary Positive speed reference[RPM]
    solo->SetSpeedReference(3000);
    // wait till motor reaches to the reference 
    Sleep(300);
    actualMotorSpeed = solo->GetSpeedFeedback();
    std::cout << "Measured Speed[RPM]: "<< actualMotorSpeed << std::endl;

    actualMotorTorque = solo->GetQuadratureCurrentIqFeedback();
    std::cout << "Measured Iq/Torque[A]: "<< actualMotorTorque << std::endl;
    Sleep(3000);
  }
  return 0;
}
