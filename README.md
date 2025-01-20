# Pet-Feeder-Arduino
This was honestly a personal project but thought people who have pets at home could use a cheap food dispenser as long as they have the patience to DIY the hardware.
# How to use the code?
The Code consists of the following hardware->
Servo motor (Continuous or positional), a HX711 based load sensor, arduino board, ultrasonic sensor, LED, some cardboard and a keypad for input (I used a 3*3)
# Load sensor calibration
Mine has a calibration factor of 417050.0, this changes for every sensor so change this value in the code to match your sensor. You can find plenty of HX711 based sensor codes for calibration.

# Servo motors
In my code i unfortunately used continuous servos instead of the superior positional ones, if you are using the good one. Chnage the clockwise and anticlcokwise position values at the beginning of the code to be the degree of end point, if it's a continuous servo my code will simply have a rotation until I stop it. So remove the code which stops the servo (the stopper is usually "myservo.write(90)", just go trial and error.)

# Changable values
You can change the deployed weight by changing the load threshold, timer is handles by the keypad. If u don't want food to release when the food is already on the bowl then add another condition inside the if statement checking for deployment validity. You can also change the distance at which the ultrasonic sensor will light up the "low food warning" LED.

# Hardware blueprint
I'll soon link a video, it will show the most basic model you can do with just cardboard and a plastic bottle aside from the mechanical components. I won't explain HOW to make the structure as even my design is makeshift and can defenitely be improved with 3D printing or simply, a better idea.
