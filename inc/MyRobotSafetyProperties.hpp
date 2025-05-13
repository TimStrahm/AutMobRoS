#ifndef MyRobotSAFETYPROPERTIES_HPP_
#define MyRobotSAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "ControlSystem.hpp"

class MyRobotSafetyProperties : public eeros::safety::SafetyProperties
{
public:
    MyRobotSafetyProperties(ControlSystem &cs, double dt);

    // Define all possible events
    eeros::safety::SafetyEvent doSystemOff;
    eeros::safety::SafetyEvent doSystemOn;
    eeros::safety::SafetyEvent doSystemStopped;
    eeros::safety::SafetyEvent doSystemStarted;
    eeros::safety::SafetyEvent doPowerOn;
    eeros::safety::SafetyEvent doPowerOff;
    eeros::safety::SafetyEvent doStartMoving;
    eeros::safety::SafetyEvent doStopMoving;
    eeros::safety::SafetyEvent doEmergency;
    eeros::safety::SafetyEvent doResetEmergency;

    

    // Defina all possible levels§
    eeros::safety::SafetyLevel slSystemOff;
    eeros::safety::SafetyLevel slSystemTurningOn;
    eeros::safety::SafetyLevel slSystemTurningOff;
    eeros::safety::SafetyLevel slSystemOn;
    eeros::safety::SafetyLevel slSystemMotorPowered;
    eeros::safety::SafetyLevel slSystemMoving;
    eeros::safety::SafetyLevel slSystemEmergency;

private:
    // Define all critical outputs
    eeros::hal::Output<bool>* LED_g;
    eeros::hal::Output<bool>* LED_r;


    // Define all critical inputs
    eeros::hal::Input<bool>* button_p; // pause button
    eeros::hal::Input<bool>* button_m; // Mode button


    ControlSystem &cs;
};

#endif // MyRobotSAFETYPROPERTIES_HPP_
