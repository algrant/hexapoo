# Arduino stewart platform implementation using a teensy 4.1, 6 crappy stepper motors & 6 switches


# GOALS


## necessary

--> stealing some math from https://www.instructables.com/Stewart-Platform/
& 
- write function to xyz postion & euler rotation to control platform & have platform move there...
  - convert xyz & euler to stepper positions
    - all the maaaath...
  - move steppers to this position

- Homing function
  - on startup let all the servos move to 0 position & wait there for serial input...
