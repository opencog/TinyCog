/*
  Project: OpenCogER
  File: manualCommands.cpp
  Author: Aman Kassahun <amankassahun190@gmail.com>
  License: AGPL
  Date: February, 2019
*/

/*

* Assumption: position is between 0  and 45 degrees. negetive degrees imply rotation in reverse direction.
            : speed is between 
* SPEEDVOLTAGECONSTANT is minimum delay of the motor that could bring the maximum rotation(position) multiplied by 100.
 It is dependent on voltage supplied. forexample 1800 is SPEEDVOLTAGECONSTANT for 5V supply
* 'isOpened' boolean return true for objects opened more than 11 degrees else it returns false 
* currentPosition is valid only the whole life of one uploaded program, else it returns zero.
* It is better always to start by resetAll(), unless CurrentPosition &  Absolute move commands may not bring expected results.
* same pwm pin is connected for all bodys i.e it is not possible to differnt body motors wit differnt speeds at the same time.
*/


#ifndef MANUAL_H_

#include<iostream>
#include <vector>
#include <map>
#include <wiringPi.h>
#include<softPwm.h>
using namespace std;
#define SPEEDVOLTAGECONSTANT 1800

string mouth="mouth",head="head",eye="eye",forehead="forehead",check="check";

map<std::string,int>  currentPosition ={{"mouth",0},{"head",0},{"forehead",0},{"eye",0},{"check",0}};

map<std::string,std::vector<int> > pinStore ={
{mouth,{3,5}},
{head,{7,12}},
{eye,{12,13}},
{forehead,{10,11}},
{check,{15,16}},
};
map<std::string,int> feedbackPinStore ={
{mouth,19},
{head,21},
{forehead,22},
{eye,23},
{check,24}
};


bool isBodyOpened(string body){
pinMode(feedbackPinStore[body],INPUT);
if(digitalRead(feedbackPinStore[body])==HIGH){
return true;
}else if(digitalRead(feedbackPinStore[body])==LOW) {
return false;
}

}


void moveBodyRelative(string body,int position, int speed){

pinMode(pinStore[body][0],OUTPUT);
pinMode(pinStore[body][1],OUTPUT); 
pinMode(38,OUTPUT);
softPwmCreate(38,0,100); 

softPwmWrite(38,speed);

double speedConstant= SPEEDVOLTAGECONSTANT/speed; 
int k = (int)(speedConstant < 0 ? (speedConstant - 0.5) : (speedConstant + 0.5));

currentPosition[body]+=position;
if(currentPosition[body]>45)
{
currentPosition[body]=45;
}
else if(currentPosition[body]<-45)
{
currentPosition[body]=-45;
}
if(position>=0)
{
digitalWrite(pinStore[body][0],HIGH); 
digitalWrite(pinStore[body][1],LOW); 
delay(k*position); 
}
else
{
digitalWrite(pinStore[body][0],LOW); 
digitalWrite(pinStore[body][1],HIGH); 
delay(abs(position)*k); 
}
}




void moveBodyAbsolute(string body,int position,int speed)
{
if(position>=0)
{
moveBodyRelative(body,position-currentPosition[body],speed);
}
else
{
cout<<"ERROR: absolute position can not be negative";
}
}


void resetAll(){
for(auto body:pinStore)
{
moveBodyRelative(body.first,-45,100);
currentPosition[body.first]=0;
}
}
#endif //MANUAL_H_
