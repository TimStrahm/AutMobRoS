#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt) // names such as "quat1", "motor1" must correspond to the HWConfig.json
    : q1("quat1"),
      gain(2.0),
      signalChecker(-0.2, 0.2), // Checks if a signal is in this range (triggers event if outside)
      motorVoltageSetpoint(0.0),
      motor("motor1"),
      E2("enc2"),
      timedomain("Main time domain", dt, true)

{
    // Name all blocks
    gain.setName("Gain");
    q1.setName("Q1");
    signalChecker.setName("SignalChecker");
    motorVoltageSetpoint.setName("motorVoltageSetpont");
    motor.setName("motor");
    E2.setName("E2");

    // Name all signals
    q1.getOut().getSignal().setName("alpha/2");
    gain.getOut().getSignal().setName("alpha");
    motorVoltageSetpoint.getOut().getSignal().setName("Motor Voltage Setpoint");
    E2.getOut().getSignal().setName("q2[rad]");

    // Connect signals
    gain.getIn().connect(q1.getOut());
    signalChecker.getIn().connect(gain.getOut());
    motor.getIn().connect(motorVoltageSetpoint.getOut());


    // Add blocks to timedomain
    timedomain.addBlock(q1);
    timedomain.addBlock(gain);
    timedomain.addBlock(signalChecker);
    timedomain.addBlock(motorVoltageSetpoint);
    timedomain.addBlock(motor);
    timedomain.addBlock(E2);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}