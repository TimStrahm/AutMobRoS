#ifndef MAINSEQUENCE_HPP_
#define MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Wait.hpp>
#include <eeros/sequencer/Monitor.hpp>
#include "MyRobotSafetyProperties.hpp"
#include "ControlSystem.hpp"
#include "customSteps/setMotorVoltage.hpp"
#include "customSequences/orientationException.hpp"

class MainSequence : public eeros::sequencer::Sequence
{
public:
    MainSequence(std::string name, eeros::sequencer::Sequencer &seq,
                 eeros::safety::SafetySystem &ss,
                 MyRobotSafetyProperties &sp,
                 ControlSystem &cs)
        : eeros::sequencer::Sequence(name, seq),
          ss(ss),
          sp(sp),
          cs(cs),

          sleep("Sleep", this)
         // setMotorVoltage("setMotorVoltage", this, cs),
         // checkOrientation(0.1, cs),
         // orientationException("Orientation Exception", this, cs, checkOrientation),
         // orientationMonitor("Orientation monitor", this, checkOrientation, eeros::sequencer::SequenceProp::resume, &orientationException)
          
    {
        //addMonitor(&orientationMonitor);
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        while (eeros::sequencer::Sequencer::running) // Contains all the steps to be done sequentially (like Tasklist)
        {
            sleep(1.0);
            //log.info() << "Angle of Wheel is at" << cs.E2.getOut().getSignal() << " rad.";
                log.info() << cs.E2.getOut().getSignal();

            // Motor sweep
            // setMotorVoltage(-1.5);
            // sleep(3.0);
            // setMotorVoltage(1.5);
            // sleep(3.0);

            // sleep(1.0);
            // log.info() << "Sweep completed...";
        }
        return 0;
    }

private:
    eeros::safety::SafetySystem &ss;
    ControlSystem &cs;
    MyRobotSafetyProperties &sp;

    eeros::sequencer::Wait sleep;
    // SetMotorVoltage setMotorVoltage;
    // CheckOrientation checkOrientation;
    // OrientationException orientationException;
    // eeros::sequencer::Monitor orientationMonitor;
};

#endif // MAINSEQUENCE_HPP_