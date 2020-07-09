/* this is a (2D & 2DoF) robotic arm program
first it will ask you wether you want to use the forward or inverse method
if forward was chosen you will give angles and the program gives you the position of end-effector
also the first motor will move and the 2nd after it as a result to the input angles 
if Inverse was chosen the program will asks if you know the summation angle to get a certain result
if yes you will enter it in addition to the x and y (end-effector point)
if no you will enter only x and y
and the program will caculate the angles for you */

#include <Servo.h>
#include <math.h>

Servo m1;  
Servo m2;

float L1 = 10; // the length related to the 1st angle
float L2 = 10; // the length related to the 2nd angle
float x; // position of end-effector on the horizontal axis
float y; // position of end-effector on the vertical axis
float Q1; // first angle
float Q2; // second angele starting from the axis of 1st angle
float Q;  // the angle from the main horizontal axis to 2nd angle
String mainMethod; // to choose the method (Forward or Inverse)
String inverseMethod; // to know if the user want to enter Q in Inverse kinematic
int srv1 = 7; // first servo motor will move according to Q1
int srv2 = 8; // second servo and will move according to Q2

void setup() {
  
  Serial.begin(9600); } // speed of contacting the user
  

void loop() {
  
  // choose Forward or Inverse 
  
  Serial.println("Press A for Forward Kinematic or B for Inverse");
  while(Serial.available() == 0) {}
  mainMethod = Serial.readString();
  
  // Forward Kinematic
  if ( mainMethod == "A" || mainMethod == "a") {
    
    Serial.println("enter first angle (0-180) in degree: ");
    while (Serial.available()== 0) {}
    Q1 = Serial.parseFloat();
    
    if ( Q1 >= 0 && Q1 <= 180) { // must be 0-180 because this is the range of servo motor angle
      
      Serial.println("enter second angle (0-180) in degree: ");
      while (Serial.available()== 0) {}
      Q2 = Serial.parseFloat();
    
      Q=Q1+Q2;
      
      if ( Q2 >= 0 && Q2 <= 180) { // must be 0-180 because this is the range of servo motor angle

        // from the trajectory sketch we found out the following #see attached PDF
        
        x = L1*cos(Q1*(PI/180)) + L2*cos(Q*(PI/180));
        y = L1*sin(Q1*(PI/180)) + L2*sin(Q*(PI/180));
    
    
        Serial.println(" The position of the end-effector is at point: ");
        Serial.print(" x= "); Serial.print(x); Serial.println(" cm");
        Serial.print(" y= "); Serial.print(y); Serial.println(" cm");
        Serial.print(" first angle + second angle= "); Serial.print(Q); Serial.println(" degree");

        m1.attach(srv1); 
        m1.write(Q1);
        delay(1000);
        m2.attach(srv2);
        m2.write(Q2);
        delay(1000); }  
      
      else { Serial.println("The angle must be in the range 0-180"); } }
    
    
    else { Serial.println("The angle must be in the range 0-180"); }
  }
  
  // Inverse Kinematic
  
  else if ( mainMethod == "b" || mainMethod == "B") {
    
    Serial.println("Press C if you have the summed angle or D if you don't");
    while (Serial.available()== 0) {}
    inverseMethod = Serial.readString();
    
    
    if ( inverseMethod == "C" || inverseMethod == "c" || inverseMethod == "D" || inverseMethod == "d") {
 
      Serial.println("enter the horizontal axis in cm: ");
      while (Serial.available()== 0) {}
      x = Serial.parseFloat();
      Serial.println("enter the vertical axis in cm: ");
      while (Serial.available()== 0) {}
      y = Serial.parseFloat();
    
      if ( inverseMethod == "C" || inverseMethod == "c" ) {
    
        Serial.println("enter the summation angle");
        Serial.println("(1st angle + 2nd angle) in degree: ");
        while (Serial.available()== 0) {}
        Q = Serial.parseFloat(); }
    
    
      float radQ2 = acos((pow(x,2)+pow(y,2)-pow(L1,2)-pow(L2,2))/(2*L1*L2));
      Q2 = radQ2 * (180/PI);
    
      if ( inverseMethod == "C" || inverseMethod == "c" ) {
      
        Q1 = Q - Q2; /*finding Q1 with Q known*/ }
    
      if ( inverseMethod == "D" || inverseMethod == "d" ) {
    
    
        // To find Q1 with unkown Q (not very accurate)
        //(using sine & cosine laws we could find out the following equations #see the attached PDF)
        // also trigonometic funs in arduino working on radians thats behind multiplying of 180/PI or PI/180
    
        float alpha = (180-Q2)*PI/180;
        float beta = atan(y/x) * 180/PI ;
        float O = asin((sin(alpha)*L2)/(sqrt(pow(x,2)+pow(y,2)))) * 180/PI;
        Q1 = beta - O;  }

      Serial.print(" The first angle= "); Serial.print(Q1); Serial.println(" degree"); 
      Serial.print(" The second angle= "); Serial.print(Q2); Serial.println(" degree");
    
      m1.attach(srv1);
      m1.write(Q1);
      delay(1000);
      m2.attach(srv2);
      m2.write(Q2);
      delay(1000); }

   else { Serial.println("Pleas press C or D only"); }
  }
    
  else { Serial.println( "please enter A or B only" ); }
}