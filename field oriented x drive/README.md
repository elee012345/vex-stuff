# Field Oriented X Drive
Some nice little field oriented x drive code I made for fun. Turned out to be really useful.
Our robot is basically a swerve bot with less pushing power now. Wasn't actually too hard to make.

Original code found here: https://www.robotmesh.com/studio/5be40c90c8f17a1f5796fd35
I just made it field oriented.

# Thought Process
An x drive can already drive in all 4 directions (forward, backward, left, right) and also turn,
but can only do that as a robot-oriented drive. I realized that, if the robot is facing, say, 90
degrees to the right, then if you rotated its current heading (90 degrees) by where the joystick is, then 
you'll make it go left compared to itself, which is forward compared to the field. For example, say the joystick
values are from -1 to 1 for both x and y directions. The joystick coordinates at this current time are (1, 0), meaning
that you want the robot to go right. If the robot is facing 90 degrees already, then it should go forward. Rotate those
coordinates by the robot heading (90 degrees), and you get the joystick coordinates (0, 1), making the robot
go forward compared to itself, which is right compared to the field. This works for all orientations.

A faster way to rotate the joystick coordinates was found on this website: https://danceswithcode.net/engineeringnotes/rotations_in_2d/rotations_in_2d.html
This decreases lag and increases performance.

I used the code here: https://github.com/elee012345/vex-spin-up
