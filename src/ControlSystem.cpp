#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt) // names such as "quat1", "motor1" must correspond to the HWConfig.json
    : q1("quat1"),
      gain(2.0),
      signalChecker(-0.2, 0.2), // Checks if a signal is in this range (triggers event if outside)
      motorVoltageSetpoint(0.0),
      motor("motor1"),
      E2("enc2"),
      cont(0.03 / 2.0 / M_PI),
      QMax(0.1),
      iInv(104.0 / 3441.0),
      kMInv(1 / 8.44e-3),
      R(8.0),
      timedomain("Main time domain", dt, true)

{
    // Name all blocks
    // gain.setName("Gain");
    q1.setName("Q1");
    // signalChecker.setName("SignalChecker");
    // motorVoltageSetpoint.setName("motorVoltageSetpont");
    E2.setName("E2");
    cont.setName("cont");
    QMax.setName("QMax");
    iInv.setName("i");
    kMInv.setName("kM");
    R.setName("R");
    motor.setName("motor");

    // Name all signals
    q1.getOut().getSignal().setName("alpha/2");
   // gain.getOut().getSignal().setName("alpha");
   // motorVoltageSetpoint.getOut().getSignal().setName("Motor Voltage Setpoint");
    E2.getOut().getSignal().setName("q2[rad]");
    cont.getOut().getSignal().setName("qd1[rad/s]");

    // qdMax.getOut().getSignal().setName("qd1[rad/s]");
    // i.getOut().getSignal().setName("om1[rad/s]");
    // kM.getOut().getSignal().setName("U[V]");

    // Connect signals
    // gain.getIn().connect(q1.getOut());
    // signalChecker.getIn().connect(gain.getOut());
    // motor.getIn().connect(motorVoltageSetpoint.getOut());

    // DC Motor Lab

    cont.getIn().connect(E2.getOut());
    QMax.getIn().connect(cont.getOut());
    iInv.getIn().connect(QMax.getOut());
    kMInv.getIn().connect(iInv.getOut());
    R.getIn().connect(kMInv.getOut());
    motor.getIn().connect(R.getOut());

    // Add blocks to timedomain (order is critical!)
    timedomain.addBlock(q1);
    // timedomain.addBlock(gain);
    // timedomain.addBlock(signalChecker);
    // timedomain.addBlock(motorVoltageSetpoint);
    timedomain.addBlock(E2);
    timedomain.addBlock(cont);
    timedomain.addBlock(QMax);
    timedomain.addBlock(iInv);
    timedomain.addBlock(kMInv);
    timedomain.addBlock(R);

    // timedomain.addBlock(qdMax);
    // timedomain.addBlock(i);
    // timedomain.addBlock(kM);
    timedomain.addBlock(motor);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}