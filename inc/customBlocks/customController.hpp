#ifndef CUSTOMCONTROLLER_HPP_
#define CUSTOMCONTROLLER_HPP_

#include <eeros/control/Blockio.hpp>
// Include header files for the subblocks

using namespace eeros::control;

template <typename T = double>
class customController : public Blockio<1,1,T>   // Set the number of inputs and outputs
{
public:
    customController() : phi_s{0.0}, kp{2.0}, U{0.0}
    {
        // Connect subblocks, initialize variables, ...
    }
    void setSetpoint(double phi) {phi_s = phi;}

    // Implement getter functions for the subsystem inputs

    virtual void run()
    {
        // Calculate output values, set timestamps and 
        // call the run method of the subblocks
        U = kp*(phi_s - this->in.getSignal().getValue());
        this->out.getSignal().setValue(U);
        this->out.getSignal().setTimestamp(this->in.getSignal().getTimestamp());
    }

protected:
    // Define intermediate variables and subblocks
    double phi_s, kp, U;
};

#endif //CUSTOMCONTROLLER_HPP_
