#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt) // names such as "quat1", "motor1" must correspond to the HWConfig.json
    : q1("quat1"),
      gain(2.0),
      signalChecker(-0.2, 0.2), // Checks if a signal is in this range (triggers event if outside)
      motorVoltageSetpoint(0.0),
      motor("motor1"),
      E2("enc2"),
      cont(21.2/2.0/M_PI),
      qdMax(21.2),
      i(3441.0/104.0),
      kM(8.44e-3),
      timedomain("Main time domain", dt, true)

{
    // Name all blocks
    gain.setName("Gain");
    q1.setName("Q1");
    signalChecker.setName("SignalChecker");
    motorVoltageSetpoint.setName("motorVoltageSetpont");
    motor.setName("motor");
    E2.setName("E2");
    cont.setName("cont");
    qdMax.setName("qdMax");
    i.setName("i");
    kM.setName("kM");



    // Name all signals
    q1.getOut().getSignal().setName("alpha/2");
    gain.getOut().getSignal().setName("alpha");
    motorVoltageSetpoint.getOut().getSignal().setName("Motor Voltage Setpoint");
    E2.getOut().getSignal().setName("q2[rad]");
    cont.getOut().getSignal().setName("qd1[rad/s]");
    qdMax.getOut().getSignal().setName("qd1[rad/s]");
    i.getOut().getSignal().setName("om1[rad/s]");
    kM.getOut().getSignal().setName("U[V]");

    // Connect signals
    // gain.getIn().connect(q1.getOut());
    // signalChecker.getIn().connect(gain.getOut());
    // motor.getIn().connect(motorVoltageSetpoint.getOut());

    // DC Motor Lab

    cont.getIn().connect(E2.getOut());
    qdMax.getIn().connect(cont.getOut());
    i.getIn().connect(qdMax.getOut());
    kM.getIn().connect(i.getOut());
    motor.getIn().connect(kM.getOut());

    // Add blocks to timedomain
    timedomain.addBlock(q1);
    // timedomain.addBlock(gain);
    //timedomain.addBlock(signalChecker);
    //timedomain.addBlock(motorVoltageSetpoint);
    timedomain.addBlock(E2);
    timedomain.addBlock(cont);
    timedomain.addBlock(qdMax);
    timedomain.addBlock(i);
    timedomain.addBlock(kM);
    timedomain.addBlock(motor);


    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}