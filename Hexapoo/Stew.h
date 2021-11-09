// MultiStepper.h

#ifndef Stew_h
#define Stew_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

// Not sure why or if we need this, but so be it...
class AccelStepper;

class Stew
{
public:
    /// Constructor
    Stew();

    /// Set the target positions of all managed steppers 
    /// according to a coordinate array.
    /// New speeds will be computed for each stepper so they will all arrive at their 
    /// respective targets at very close to the same time.
    /// \param[in] absolute An array of desired absolute stepper positions. absolute[0] will be used to set
    /// the absolute position of the first stepper added by addStepper() etc.
    void setPosition(long absolute[]);
    
    /// Calls runSpeed() on all the managed steppers
    /// that have not acheived their target position.
    /// \return true if any stepper is still in the process of running to its target position.
    boolean run();

    /// Runs all managed steppers until they acheived their target position.
    /// Blocks until all that position is acheived.
    void    runSpeedToPosition();
    
private:
    // Array of pointers to the steppers we are controlling.
    // Fills from 0 onwards
    AccelStepper* _steppers[MULTISTEPPER_MAX_STEPPERS];

    // Number of steppers we are controlling and the number
    // of steppers in _steppers[]
    uint8_t       _num_steppers;
};

/// @example MultiStepper.pde
/// Use MultiStepper class to manage multiple steppers and make them all move to 
/// the same position at the same time for linear 2d (or 3d) motion.

#endif