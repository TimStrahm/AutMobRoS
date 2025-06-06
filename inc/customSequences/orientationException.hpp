#ifndef ORIENTATIONEXCEPTION_HPP_
#define ORIENTATIONEXCEPTION_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Condition.hpp>

class CheckOrientation : public eeros::sequencer::Condition
{
public:
    CheckOrientation(double angle, ControlSystem &cs) : angle(angle), cs(cs)
    {
    }

    bool validate()
    {
        return abs(cs.gain.getOut().getSignal().getValue()) > angle;
    }

private:
    // Define steps, sub-sequences, variables, conditions, monitors, exception sequences, ...
    ControlSystem &cs;
    double angle;
};

class OrientationException : public eeros::sequencer::Sequence
{
public:
    OrientationException(std::string name, eeros::sequencer::Sequence *caller,
                         ControlSystem &cs, CheckOrientation checkOrientation)
        : cs(cs), checkOrientation(checkOrientation),
          eeros::sequencer::Sequence(name, caller, true)
    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        log.warn() << "Orientation around x is outside of the allowed range!";
        log.warn() << "Stoping all motors!";
        cs.motorVoltageSetpoint.setValue(0);
        return 0;
    }

    bool checkExitCondition()
    {
        return !checkOrientation.validate();
        // Add check for current safety level, for example like safetySystemGetlevel should be higher than
        // emergency, and only leave exception if level is higher
    }

private:
    ControlSystem &cs;
    CheckOrientation checkOrientation;
};

#endif // ORIENTATIONEXCEPTION_HPP_