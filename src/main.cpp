#include <signal.h>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/hal/HAL.hpp>
#include "ControlSystem.hpp"
#include "MyRobotSafetyProperties.hpp"
#include "MainSequence.hpp"

void signalHandler(int signum)
{
    eeros::safety::SafetySystem::exitHandler();
    eeros::sequencer::Sequencer::instance().abort();
}

int main(int argc, char **argv)
{
    const double dt = 0.001; 
   // const double dt_100 = 0.1; // Time domain with 100ms clock

    eeros::logger::Logger::setDefaultStreamLogger(std::cout);
    eeros::logger::Logger log = eeros::logger::Logger::getLogger();

    log.info() << "Starting strat3 project...";

    log.info() << "Initializing hardware...";
    eeros::hal::HAL& hal = eeros::hal::HAL::instance();
    //hal.readConfigFromFile(&argc, argv);
    hal.readConfigFromFile("HwConfigBBBlue.json");

    log.info() << "Initializing control system...";
    ControlSystem cs(dt);

    log.info() << "Initializing safety system...";
    MyRobotSafetyProperties sp(cs, dt);
    eeros::safety::SafetySystem ss(sp, dt);
    cs.timedomain.registerSafetyEvent(ss, sp.doSystemOff); // fired if timedomain fails to run properly
    cs.signalChecker.registerSafetyEvent(ss, sp.doEmergency); // If a signalChecker triggers, it will do the event doEmergency.
    cs.signalChecker.setActiveLevel(sp.slSystemOn); // Only check for signal at Level SystemOn and higher
    signal(SIGINT, signalHandler); // Catches signal from keyboard to properly shutdown the program

    log.info() << "Initializing sequencer...";
    auto &sequencer = eeros::sequencer::Sequencer::instance();
    MainSequence mainSequence("Main Sequence", sequencer, ss, sp, cs);
    mainSequence();

    log.info() << "Initializing executor...";
    auto &executor = eeros::Executor::instance();
    executor.setMainTask(ss);
    ss.triggerEvent(sp.doSystemOn);
    executor.run();

    mainSequence.wait();

    log.info() << "Robo project finished...";

    return 0;
}