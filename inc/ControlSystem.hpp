#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/TimeDomain.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/SignalChecker.hpp>
#include "customBlocks/customController.hpp"


using namespace eeros::control;

class ControlSystem
{
public:
    ControlSystem(double dt);

    // Define Blocks
    Gain<> gain;
    PeripheralInput<> q1;
    SignalChecker<> signalChecker;

    Constant<> motorVoltageSetpoint;
    //PeripheralInput<> encoder;
    PeripheralOutput<> motor;
    customController<> controller;

    

    TimeDomain timedomain;
};

#endif // CONTROLSYSTEM_HPP