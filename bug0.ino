#include <RedBot.h>
#include <math.h>
RedBotSensor left = RedBotSensor(A3); // initialize a left sensor object on A3
RedBotSensor center = RedBotSensor(A6); // initialize a center sensor object on A6
RedBotSensor right = RedBotSensor(A7); // initialize a right sensor object on A7
#define LINETHRESHOLD 750
#define SPEED 65 // sets the nominal speed. Set to any number from 0 - 255.
RedBotMotors motors; // Instantiate the motor control object.
// SPEED CALCULATION
int leftSpeed; // variable used to store the leftMotor speed
int rightSpeed; // variable used to store the rightMotor speed
RedBotEncoder encoder = RedBotEncoder(A2, 10);
int countsPerRev = 192; // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
float wheelDiam = 2.56; // diam = 65mm / 25.4 mm/in
float wheelCirc = PI*wheelDiam; // Redbot wheel circumference = pi*D
int buttonPin = 12;
RedBotAccel accelerometer;
boolean Start = 0;
// We calculated as the number of ticks by the wheel to cover the 360 degree angle rotation
was 900.
// So we calculated the ticks required for 1 degree rotation as 2.5 ticks.
float TPD = 2.5/2; // TPD = Ticks per degree
void setup()
{
Serial.begin(9600);
Serial.println("Bug Algorithm");
Serial.println("------------------------------------------");
delay(2000);
Serial.println("IR Sensor Readings: ");
delay(500);
pinMode(buttonPin, INPUT_PULLUP);
Serial.begin(9600);
Serial.println("left
right
xf
yf dist_staright2");
Serial.println("================");
accelerometer.enableBump();
}
void loop()
{
if (accelerometer.checkBump()) // Checks for the tap on the robot
{
Start = 1; // Generating a signal of Start = 1
}

if(Start) // if Start is enabled
{
Goal_position(0,0,0,100,100);
Start = 0;
// Generating a signal Start = 0
}
}
void Goal_position(int x,int y,float theta,int xf,int yf)
{
long lCount = 0;
long rCount = 0;
float numRev;
int encClick_l = 0; // Defines the encoder clicks
int encClick_r = 0; // Defines the encoder clicks
int right_turn_l = 0;
int right_turn_r =0;
int left_turn_l =0;
int left_turn_r =0;
int encClick_dist = 0;
float t_right=0;
//Computes the angle to turn to goal
float Goal_theta = compute_orientation(x,y,xf,yf);
encoder.clearEnc(BOTH); // clear the encoder count
if (theta > Goal_theta) // this is done when bot angle is greater than goal, making turn right
towards goal
{
float goal_tick = (theta - Goal_theta)*TPD;
while((encoder.getTicks(RIGHT)) < goal_tick)
{Goal_theta;
leftSpeed = -70;
rightSpeed = -70;
motors.leftMotor(leftSpeed);
motors.rightMotor(rightSpeed);
}
encoder.clearEnc(BOTH);
}
else if (theta < Goal_theta) // If the bot angle is lesser than goal, turn left to turn towards
goal
{
float goal_tick = (Goal_theta-theta)*TPD;
while((-encoder.getTicks(LEFT)) < goal_tick)
{
leftSpeed = 70;
rightSpeed = 70;
motors.leftMotor(leftSpeed);
motors.rightMotor(rightSpeed);
}
encoder.clearEnc(BOTH);
}
motors.brake();
float Goal_distance = sqrt(((xf-x)*(xf-x)) + ((yf-y)*(yf-y)));
numRev = (float) Goal_distance / wheelCirc;
encoder.clearEnc(BOTH);
float r = 0;
if ((left.read() < LINETHRESHOLD) && (center.read() < LINETHRESHOLD) &&
(right.read() < LINETHRESHOLD))//If all the 3 sensors are white, move towards goal
{
motors.leftMotor(-70);
motors.rightMotor(70);
}
else
{
r= (((encoder.getTicks(LEFT)+ encoder.getTicks(RIGHT)/2)*
wheelCirc)/countsPerRev);
x = x+(r*cos(Goal_theta));
y = x+(r*sin(Goal_theta));
while(center.read() < LINETHRESHOLD)
{
motors.leftMotor(70);
motors.rightMotor(70);
}
}
while ((encClick_r+ encClick_l)/2 < numRev*countsPerRev)
{
r= (((encClick_r+ encClick_l)/2)* wheelCirc)/countsPerRev;
x = (r*cos(Goal_theta/57.325));
y = (r*sin(Goal_theta/57.325));
if(center.read() > LINETHRESHOLD) // If the center sensor reads black, drive straight.
{
leftSpeed = -SPEED;
rightSpeed = SPEED;
lCount = encoder.getTicks(LEFT);
rCount = encoder.getTicks(RIGHT);
motors.leftMotor(leftSpeed);
motors.rightMotor(rightSpeed);
encClick_l = lCount- right_turn_l - left_turn_l; // calculation for Subtracting the ticks of
left turn and right turn
encClick_r = rCount- right_turn_r - left_turn_r;
encClick_dist = (((encClick_r+ encClick_l)/2) *wheelCirc/countsPerRev);
}
else if(right.read() > LINETHRESHOLD)
{
right_turn_l = encoder.getTicks(LEFT) - lCount;
right_turn_r = encoder.getTicks(RIGHT) - rCount;
Serial.println(right_turn_l);
Serial.println(right_turn_r);

leftSpeed = -70;
rightSpeed = -70;
motors.leftMotor(leftSpeed);
motors.rightMotor(rightSpeed);3+621111;
t_right = (right_turn_l)/TPD;
}
else if(left.read() > LINETHRESHOLD)
{
left_turn_l = encoder.getTicks(LEFT) - lCount;
left_turn_r = encoder.getTicks(RIGHT) - rCount;
leftSpeed = 70;
rightSpeed = 70;
motors.leftMotor(leftSpeed);
motors.rightMotor(rightSpeed);
}
if((left.read() > LINETHRESHOLD) && (center.read() > LINETHRESHOLD) &&
(right.read() > LINETHRESHOLD) )
{
motors.stop();
}
}
motors.brake();

}
// Now to compute the Goal axis with x-axis.
float compute_orientation(int start_x, int start_y, int goal_x, int goal_y)
{
float theta = atan((goal_y- start_y )/ (goal_x- start_x)) *57.325;
return theta;
}

