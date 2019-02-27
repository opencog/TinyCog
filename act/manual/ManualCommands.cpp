/*
  File: manualCommands.cpp
  Author: Aman Kassahun <amankassahun190@gmail.com>
  License: AGPL
  Date: February, 2019
*/


#include<iostream>

#include "ManualCommands.hpp"

ManualCommands *ManualCommands::instance=0;

ManualCommands::ManualCommands()
{
	feedbackPin =
	{
		{mouth,19},
		{head,21},
		{forehead,22},
		{eye    ,23},
		{check ,24}
	};

	currentPosition = 
	{
		{mouth,0},
		{head,0},
		{forehead,0},
		{eye,0},
		{check,0}
	};
	pin =
	{
		{mouth,{3,5}},
		{head, {7,12}},
		{eye,  {12,13}},
		{forehead,{10,11}},
		{check,  {15,16}},
	};
}

ManualCommands::~ManualCommands() {}

int ManualCommands::getCurrentPosition(string body)
{
	return currentPosition[body];
}

bool ManualCommands::isBodyOpened(string body)
{
	pinMode(feedbackPin[body],INPUT);
	if(digitalRead(feedbackPin[body])==HIGH)
	{
		return true;
	}
	else if(digitalRead(feedbackPin[body])==LOW)
	{
		return false;
	}
}

void ManualCommands::moveBodyRelative(string body,int position,int speed)
{
	pinMode(pin[body][0],OUTPUT);
	pinMode(pin[body][1],OUTPUT);
	pinMode(38,OUTPUT);
	softPwmCreate(38,0,100);
	softPwmWrite(38,speed);

	//positon is dependent on speed.
	double speedConstant = SPEED_VOLTAGE_CONSTANT/speed;

	//change double value to near integer
	int k= (int)(speedConstant < 0 ? (speedConstant-0.5) :
	                                 (speedConstant+0.5));
	currentPosition[body]+=position;

	/*
	Since positions are only between 0 and 45 degrees
	change above or below these values to min. or max.
	*/
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
		digitalWrite(pin[body][0],HIGH);
		digitalWrite(pin[body][1],LOW);
		delay(k*position);
	}
	else
	{
		digitalWrite(pin[body][0],LOW);
		digitalWrite(pin[body][1],HIGH);
		delay(k*abs(position)); 
	}
}

void ManualCommands::moveBodyAbsolute(string body,int position,int speed)
{
	if(position>=0)
	{
		moveBodyRelative(body,position-currentPosition[body],speed);
	}
	else
	{
		cout<<"ERROR: absolute position can not be negative"<<endl;
	}
}

void ManualCommands::resetAll()
{
	for(auto body:pin)
	{
		moveBodyRelative(body.first,-45,100);
		currentPosition[body.first]=0;
	}
}
