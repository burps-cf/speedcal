# speedcal - Calibrate linear speed of a robot car

This code maps the motor PWM values to actual speed of a robot car fitted with an ultrasonic sensor.

The algorithm:
1. For value in valid range of values do
2. Obtain distance to obstacle in front (from ultrasonic sensor) and time T0
3. Energize the motors for forward movement using the value
4. Drive forwarward until the distance to obstacle reaches limit
5. Obtain time T1
6. Calculate linear speed (initial distance - final distance)/(T1-T0) for value
7. Reverse to initial distance
8. Repeat

The speed will be dependent on the radius of wheels fitted to the car, so calibration needs
to be done for each car/wheels combination.

The calculated speed will not be precise - there will be an error resulting from limited
precision of distance and time measurements, as well as from assuming the car's motion is uniform and
ignoring the acceleration and deceleration at the start and end of movement. But it should be sufficient
for estimating the PWM values needed to drive the car with required speed.
