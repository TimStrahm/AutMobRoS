#ifndef SETMOTORVOLTAGE_HPP_
#define SETMOTORVOLTAGE_HPP_

#include <eeros/sequencer/Step.hpp>
#include "ControlSystem.hpp"

class SetMotorVoltage : public eeros::sequencer::Step
{
public:
    SetMotorVoltage(std::string name, eeros::sequencer::Sequence *caller, ControlSystem &cs)
        : cs(cs), eeros::sequencer::Step(name, caller)
    {
        log.info() << "Step created: " << name;
    }

    int operator()(double motorVoltageSetpoint)
    {
        this->motorVoltageSetpoint = motorVoltageSetpoint;
        return start();
    }

    int action()
    {
        log.info() << "Setting Voltage to " << motorVoltageSetpoint << " Volts.";
        cs.motorVoltageSetpoint.setValue(motorVoltageSetpoint);
        return 0;
    }

private:
    // Define variables, conditions, monitors, exception sequences, ...
    ControlSystem &cs;
    double motorVoltageSetpoint;
};

#endif // SETMOTORVOLTAGE_HPP_
