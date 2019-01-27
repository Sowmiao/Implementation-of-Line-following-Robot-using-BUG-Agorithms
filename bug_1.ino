#include <RedBot.h>RedBotSensor left = RedBotSensor(A3); // initialize a left sensor object on A3
RedBotSensor center = RedBotSensor(A6); // initialize a center sensor object on A6
RedBotSensor right = RedBotSensor(A7); // initialize a right sensor object on A7
RedBotBumper lBumper = RedBotBumper(3); // initialzes bumper object on pin 3
RedBotBumper rBumper = RedBotBumper(11); // initialzes bumper object on pin 11
#define LINETHRESHOLD 750
#define SPEED 65
//motor
RedBotMotors motors;
//speed
int leftSpeed; // variable used to store the leftMotor speed
int rightSpeed; // variable used to store the rightMotor speed
//encoder
RedBotEncoder encoder = RedBotEncoder(A2, 10); // initializes encoder on pins A2 and 10
float distance = 50;
int buttonPin = 12;
int countsPerRev = 192; // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
float wheelDiam = 2.56; // diam = 65mm / 25.4 mm/in
float wheelCirc = PI*wheelDiam; // Redbot wheel circumference = pi*D
int lCount = 0;
int rCount = 0;
float numRev;
float constant1 =(float) wheelCirc/countsPerRev; //
float constant2 = (float)distance/constant1;
//bumper
int buttonPin1 = 12; // variable to store the button Pin
int lBumperState; // state variable to store the bumper value
int rBumperState; // state variable to store the bumper value
//tap start
int start = 0;
RedBotAccel accelerometer;
//position of robot
float xi=0;
float yi=0;
float xf=20;
float yf=20;
//to update x and y co
int turn = 0;
//bug1
int distanceBug1 = 0;
//temp store value
long long int temp = 0;
long long int count_temp = 0;
long long int rcount_temp = 0;
long long int turnTicker = 0;
long long int turnTicker_white = 0;
float min_distance = 0;
float calculated_distance = 0;
long long int white_dist = 0;
long long int turnTicker1 = 0;
long long int shiftTicker = 0;
float dist_staright = 0;
float dist_staright2 = 0;
int final_turn_counter = 0;
int dest_count_2 = 0;
void setup()
{
Serial.begin(9600);
Serial.println("Welcome to experiment 6.2 - Line Following");
Serial.println("------------------------------------------");
delay(2000);
Serial.println("IR Sensor Readings: ");
delay(500);
pinMode(buttonPin, INPUT_PULLUP);
Serial.begin(9600);
Serial.println("left right xf_val yf_val dist_staright2");
Serial.println("================");
}
void loop()
{
lCount = encoder.getTicks(LEFT); // read the left motor encoder
rCount = encoder.getTicks(RIGHT); // read the right motor encoder
Serial.print(left.read());
Serial.print("\t"); // tab character
Serial.print(center.read());
Serial.print("\t"); // tab character
Serial.print(right.read());
Serial.print("\t");
Serial.print(xf);
Serial.print("\t");

Serial.print(yf);
Serial.print("\t");
Serial.print(calculated_distance);
Serial.println();
//starts the robot
startRobot();
if (start == 1)
{
if (((xf > -1) && (xf < 5)) && ((yf > -1) && (yf < 2)))
{
motors.stop();
delay(10000);
}
else
if(((left.read() <600) && (center.read() <600) && (right.read() <600)))
{
//move towards goal algorithm
motors.drive(-SPEED);
dist_staright2 = -(float)(constant1 * (rCount-rcount_temp));
destination_drive();
shiftTicker = 1;
rcount_temp = rCount;
//todo reset the lcount values
}
else
// when you hit a boundary.
if(((left.read() > LINETHRESHOLD)||(center.read() > LINETHRESHOLD)||(right.read() >
LINETHRESHOLD)))
{
if (shiftTicker == 1)
{
//variables that need to be reset after it hits the obstacle after going towards goal
count_temp = lCount;
}
shiftTicker = shiftTicker +1;
rcount_temp = rCount;
//to turn when you hit the boundary
if(((left.read() > LINETHRESHOLD)&&(center.read() >
LINETHRESHOLD)&&(right.read() > LINETHRESHOLD)))

{
turn_function();
}
else
{
lineFollow();
}
calculated_distance = sqrt ( pow((xf - xi),2) + pow((yf - yi),2));
if (calculated_distance < min_distance)
{
min_distance = calculated_distance;
}
else if ((calculated_distance< min_distance+2)&&(calculated_distance> min_distance-1))
{
//write leave code
motors.stop();
//delay(2000);
//turn function
turn_function();
}
}
}
//add the below else part in the final code cus this is the stopping condition
//else
}
//tap start for robot
void startRobot()
{
//tap start code
accelerometer.read(); // updates the x, y, and z axis readings on the accelerometer
if (accelerometer.x>0 || accelerometer.y>1000) // Accelerometer readings from the
accelerometer over the monitor
{
start = 1;
} else {
start = 0;
}
}
//when you hit a boundary
void lineFollow()
{
//turn right function
if((center.read() > LINETHRESHOLD)&&(right.read() > LINETHRESHOLD))
{
turn_right();
flip2();
clearInitial();
}
//turn left function
else if ((center.read() > LINETHRESHOLD)&&(left.read() > LINETHRESHOLD))
{
turn_left();
flip1();
clearInitial();
}
//if the line is under the center sensor then drive forward
else if(center.read() > LINETHRESHOLD)
{
shiftTicker = 50;
motors.drive(-SPEED);
dist_staright = -(float)(constant1 * (lCount-count_temp));
turnTicker = 0 ;
}
// if the line is under the right sensor, adjust relative speeds to turn to the right
else if(right.read() > LINETHRESHOLD)
{
shiftTicker = 2;
turnTicker = turnTicker + 1;
motors.rightMotor(SPEED);
motors.leftMotor((SPEED ));
//delay(50);
turn = 0;
//flip2();
}
// if the line is under the left sensor, adjust relative speeds to turn to the left
else if(left.read() > LINETHRESHOLD)
{
shiftTicker = 2;
turnTicker = turnTicker + 1;
motors.rightMotor(-(SPEED )); // Turn on right motor clockwise medium power
(motorPower = 150)
//delay(50);
motors.leftMotor(-SPEED);
turn = 1;
//flip1();
}
}
void clearInitial()
{
xi = 0;
yi = 0;
count_temp = lCount;
//lCount = 0;
turnTicker = 0;
}
void flip1()
{
if((xf>0) && (yf>0))
//if(x positive and y positive)
{
//
temp = xf;
xf = yf;
yf = -xf;
}else
if((xf>0) && (yf<0))
//if(x positive and y negative)
{
//if( turnTicker == 1)
temp = xf;
xf = yf;
yf = -xf;
//both negative;
}else
if((xf<0) && (yf>0))
//if(x negative and y positive)
{
temp = xf;
xf = yf;
yf = -xf;
}else
if((xf<0) && (yf<0))
{
temp = yf;
yf = -xf;
xf = yf;
}
xf = xf - dist_staright;
//call function to reset the values of xi and yi
if(center.read() > LINETHRESHOLD)
{
//clearInitial();
}
}
//turn right
//x=
void flip2()
{
if((xf>0) && (yf>0))
//if(x positive and y positive)
{
temp = xf;
xf = -yf;
yf = xf;
}else
if((xf>0) && (yf<0))
//if(x positive and y negative)
{
//if( turnTicker == 1)
temp = xf;
xf = -yf;
yf = xf;
//both negative;
}else
if((xf<0) && (yf>0))
//if(x negative and y positive)
{
temp = xf;
xf = -yf;
yf = xf;
}else
if((xf<0) && (yf<0))
{
temp = yf;
yf = xf;
xf = -yf;
}
// {
yf = yf - dist_staright;
//
//
//
//
//
//
//
//
//
}
temp = xf;
//xf = temp;
xf = yf;
yf = -temp;
}
//call function to reset the values of xi and yi
if(center.read() > LINETHRESHOLD)
{
clearInitial();
}
void turn_function()
{
motors.rightMotor(-80); // Turn on right motor clockwise medium power (motorPower =
150)
motors.leftMotor(10);
delay(1500);
flip1();
}
void flip_white()
{
if( turnTicker1 == 1)
{
yf = yf - yi;
}
//call function to reset the values of xi and yi
if(center.read() > LINETHRESHOLD)
{
clearInitial();
}
}
void destination_drive()
{
// dest_count_2 = dest_count_2 +1;
if((yf > 0))
{
yf = yf - dist_staright2;
}
if (((yf > -3) && (yf < 2))&&(dest_count_2 == 0))
{
dest_count_2 = dest_count_2 +1;
motors.brake();
delay(500);
if((xf>3)&&(dest_count_2 == 1))
{
turn_right();
//
delay(100);
motors.stop();
delay(500);
flip2();
}
if ( (xf<-3) )
{
//freeTicker = freeTicker + 1;
final_turn_counter = final_turn_counter + 1;
reverse();
}
// else if( (!((xf > -10) && (xf < 5))) && (xf > 0) && (final_turn_counter ==1))
// {
//
// xf = xf - dist_staright2;
//}
}
}
void reverse()
{
//if (freeTicker == 1)
{
motors.drive(255);
delay(500);
motors.brake();
delay(200); // short delay to let robot fully stop
motors.rightMotor(225); // Turn on right motor clockwise medium power (motorPower =
150)
motors.leftMotor(225);
delay(450);
xf = -xf;
}
}
void turn_function_to_xaxis()
{
motors.rightMotor(-30); // Turn on right motor clockwise medium power (motorPower =
150)
motors.leftMotor(30);
delay(2000);
flip2();
}
void turn_right()
{
motors.drive(-40);
delay(500);
motors.brake();
delay(500);
motors.stop();
motors.rightMotor(150);
motors.leftMotor(150);
delay(250);
}
void turn_left()
{
  motors.drive(-40);
delay(500);
motors.brake();
delay(500);
motors.stop();
motors.rightMotor(-150);
motors.leftMotor(-150);
delay(250);
}
