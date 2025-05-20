#include "MyRobotSafetyProperties.hpp"

MyRobotSafetyProperties::MyRobotSafetyProperties(ControlSystem &cs, double dt)
    : cs(cs),
    
      slSystemOff("System is offline"),
      slSystemTurningOn("System is turning On"),
      slSystemTurningOff("System is turning Off"),
      slSystemOn("System is online"),
      slSystemMotorPowered("Motor is powered"),
      slSystemMoving("System is moving"),
      slSystemEmergency("System is in EMERGENCY STATE"),

      doSystemOn("Startup the system"),
      doSystemOff("Shutting down the system"),
      doSystemStopped("Stopping and turning off the system"),
      doSystemStarted("Starting the system"),
      doEmergency("Emergency occured"),
      doResetEmergency("Emergency has been reset"),
      doPowerOn("Power up the motors"),
      doPowerOff("Power down the motors"),
      doStartMoving("Start movement"),
      doStopMoving("Halt movement")
{
    eeros::hal::HAL &hal = eeros::hal::HAL::instance();

    // Declare and add critical outputs
    LED_g = hal.getLogicOutput("onBoardLEDgreen"); // relates to config file names
    LED_r = hal.getLogicOutput("onBoardLEDred");

    criticalOutputs = {LED_g};//, LED_r};

    // Declare and add critical inputs
    button_m = eeros::hal::HAL::instance().getLogicInput("onBoardButtonMode"); //trigger emergency
    button_p = eeros::hal::HAL::instance().getLogicInput("onBoardButtonPause"); //reset emergency

    criticalInputs = {button_m, button_p};

    // Add all safety levels to the safety system
    // (Order defines Emergency level structure)
    addLevel(slSystemOff);
    addLevel(slSystemTurningOn);
    addLevel(slSystemTurningOff);
    addLevel(slSystemOn);
    addLevel(slSystemMotorPowered);
    addLevel(slSystemMoving);
    addLevel(slSystemEmergency);

    // Add events to individual safety levels
    slSystemOff.addEvent(doSystemOn, slSystemTurningOn, kPublicEvent);
    slSystemTurningOn.addEvent(doSystemStarted, slSystemOn, kPublicEvent);
    slSystemTurningOff.addEvent(doSystemOff, slSystemOff, kPublicEvent);
    slSystemOn.addEvent(doPowerOn, slSystemMotorPowered, kPublicEvent);
    slSystemOn.addEvent(doSystemStopped, slSystemTurningOff, kPublicEvent);
    slSystemMotorPowered.addEvent(doStartMoving, slSystemMoving, kPublicEvent);
    slSystemMotorPowered.addEvent(doPowerOff, slSystemOn, kPublicEvent);
    slSystemMoving.addEvent(doStopMoving, slSystemMotorPowered, kPublicEvent);
    slSystemEmergency.addEvent(doResetEmergency, slSystemOn, kPublicEvent);


    // Add events to multiple safety levels
    addEventToAllLevelsBetween(slSystemOn, slSystemMoving, doEmergency, slSystemEmergency, kPublicEvent);
    addEventToLevelAndAbove(slSystemTurningOn, doSystemOff, slSystemOff, kPublicEvent);

    // Define input actions for all levels
    slSystemOff.setInputActions({ignore(button_p), ignore(button_m)});
    slSystemTurningOn.setInputActions({ignore(button_p), ignore(button_m)});
    slSystemTurningOff.setInputActions({ignore(button_p), ignore(button_m)});
    slSystemOn.setInputActions({check(button_p, false, doEmergency), ignore(button_m)});
    slSystemMotorPowered.setInputActions({check(button_p, false, doEmergency), ignore(button_m)});
    slSystemMoving.setInputActions({check(button_p, false, doEmergency), ignore(button_m)});
    slSystemEmergency.setInputActions({ignore(button_p), check(button_m, false, doResetEmergency)});

    // Define output actions for all levels
    slSystemOff.setOutputActions({set(LED_g, false), set(LED_r, false)});
    slSystemTurningOn.setOutputActions({set(LED_g, false), set(LED_r, true)});
    slSystemTurningOff.setOutputActions({set(LED_g, false), set(LED_r, true)});
    slSystemOn.setOutputActions({set(LED_g, true), set(LED_r, false)});
    slSystemMotorPowered.setOutputActions({set(LED_g, true), set(LED_r, true)});
    slSystemMoving.setOutputActions({set(LED_g, true), set(LED_r, true)});
    slSystemEmergency.setOutputActions({set(LED_g, true), set(LED_r, true)});
    

    // Define and add level actions
    slSystemOff.setLevelAction([&](SafetyContext *privateContext) { //TODO: Move this to "turningOff" state
        cs.timedomain.stop();
        eeros::Executor::stop();
    });

    slSystemTurningOn.setLevelAction([&,dt](SafetyContext *privateContext) { // Trans.State during startup
        if(slSystemTurningOn.getNofActivations()*dt >= 1) //dt increases in 0.001s steps. if >=1, then loop has been done 1000 times
            {
              privateContext->triggerEvent(doSystemStarted);  
            }
    });

    slSystemEmergency.setLevelAction([&](SafetyContext *privateContext) { 
        cs.motorVoltageSetpoint.setValue(0.0); // In case of emergency state, set the motor to 0 Volt
    });

    slSystemOn.setLevelAction([&](SafetyContext *privateContext) {
        cs.signalChecker.reset(); //Reset signal checker so it can trigger again
        cs.timedomain.start();
    });

    // Define entry level
    setEntryLevel(slSystemOff);

    // Define exit function
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doSystemOff);
    });
}
