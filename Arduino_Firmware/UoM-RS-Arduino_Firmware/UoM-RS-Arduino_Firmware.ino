
/*
TODO:
- Driver motors from MATLAB
  - Convert RAD to STS
  - Update robot target variable
  - Call DriveMotors();
- Setup MATLAB to send whether a motor is in pos or vel mode (set on startup, then add option to change live)
- Add FSM for reading FB, driving motors, idle, Arduino control mode, & updating firmware / changing modes
- Send Motor IDs to MATLAB
- Set MATLAB to ALWAY send/Receive MAX_ID worth of data
- Update Motor parameters
  - PID
  - Punch
  - ID


TO TEST
- Test fixed loop rate timers
- Test MATLAB Establish serial code
- Test SendDataSerial
- Test reading serial data from MATLAB using getDataSerial()
- Test that velocity is encoder counts per section (i.e., 4000 = 1 rev / second)
- Test reading feedback & sending to MATLAB
- Test receiving drive command from MATLAB and moving motors
- Test SerialMonitorMotorControl() & if it interferes with regular state operation

*/




#include <Robotics_Systems_Library.h>

#define TIMESTEP_MILLISECONDS 3

UOM_RS_Robot robot;
unsigned long tic = 0, toc = 0, timer = 0;




// bool control_mode[MAX_ID] = {
//                   POSITION, // Motor ID 1
//                   POSITION, // Motor ID 2
//                   POSITION, // Motor ID 3
//                   POSITION, // Motor ID 4
//                   POSITION, // Motor ID 5
//                   POSITION, // Motor ID 6
//                   POSITION, // Motor ID 7
//                   POSITION  // Motor ID 8
//                   };


void setup() {
  
  // int a = robot.EstablishMotorSerialConnection();
  robot.EstablishUSBConnection();
  robot.EstablishMotorSerialConnection();
  
  robot.getID();
  robot.sendMotorIDs();
  
  // if (robot.connection_status != 1) {
  //   Serial.println("Number of Connected Motors: " + String(robot.num_ID));
  // }
  

  robot.InitMotorFeedback();

  // // Set motors to position or velocity control mode
  // robot.SetMotorControlMode(control_mode);
  
  // for (int i=0; i<MAX_ID; i++) {
  //   Serial.print(String(robot.control_mode[i]) + ", ");
  // }
  // Serial.println();
}

void loop() {

  // Begin measuring execution time
  tic = millis();

  // robot.SerialMonitorMotorControl();
  // Serial.print("Control Mode - Loop Level: ");
  // for (int i=0; i<MAX_ID; i++) {
  //   Serial.print(String(robot.control_mode[i]) + ", ");
  // }
  // Serial.println();

  // Finite State Machine
  switch (state) {
    case IDLE:
      
      if (robot.connection_status == 1) {
        state = robot.getState();
      }
      // else {
      //   robot.SerialMonitorMotorControl();
      // }
      
      break;

    case READ_FB:
      robot.ReadFeedback();
      robot.SendFB2MATLAB();
      state = IDLE;
      break;

    case DRIVE_MOTOR:

      robot.getReference(); 
      robot.DriveMotors();
      state = IDLE;
      break;
      

    case UPDATE_DRIVE_MODE:

      robot.getControlMode();
      state = IDLE;
      break;
      
    case UPDATE_MOTOR:
      // TODO
      //  - PID, Punch, ID
      state = IDLE;
      break;
      
     default:
      state = IDLE;
      break;
  }

  



  // Wait until the full time-step unless running late.
  toc = millis();
  timer = toc - tic;

  if (timer > TIMESTEP_MILLISECONDS) {
    // Serial.println("Warning: Running late by " + String(timer - TIMESTEP_MILLISECONDS) + " milliseconds!");
    return;
  }
  

}
