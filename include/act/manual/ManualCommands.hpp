/*
  File: manualCommands.hpp
  Author: Aman Kassahun <amankassahun190@gmail.com>
  License: AGPL
  Date: February, 2019
*/

/*
      * Assumption: position is between 0  and 45 degrees and negetive degrees 
         imply rotation in reverse direction. 
      * SPEEDVOLTAGECONSTANT is minimum delay of the motor that could bring 
         the maximum rotation(position) multiplied by 100. It is dependent on
         voltage supplied. forexample 1800 is SPEEDVOLTAGECONSTANT for 5V supply
      * 'isOpened' boolean return true for objects opened more than 11 degrees
         else it returns false 
      * currentPosition is valid only the whole life of one uploaded program, 
         else it returns zero.
      * It is better always to start by resetAll(), unless CurrentPosition & 
         Absolute move commands may not bring expected results.
      * same pwm pin is connected for all body parts i.e it is not possible to
         move differnt body motors with differnt speeds at the same time.
*/


#ifndef _MANUAL_COMMANDS_HPP_
#define _MANUAL_COMMANDS_HPP_
#include<iostream>

#include<vector>
#include<map>

#include<wiringPi.h>
#include<softPwm.h>

using namespace std;

#define SPEED_VOLTAGE_CONSTANT 1800

#define mouth "mouth"
#define head  "head"
#define forehead "forehead"
#define eye    "eye"
#define check  "check"

class  ManualCommands {
     
     //singleton class design pattern.
     private:
             static ManualCommands *instance;
             
             ManualCommands();
             
             map<std::string,int> feedbackPin;
             map<std::string,int> currentPosition;
             map<std::string,std::vector<int> > pin;
     
      public:
             ~ManualCommands();
             
             static ManualCommands *getInstance()
             {
                  if(!instance)
                  {                       
                       instance = new ManualCommands;
                  }
                  return instance;
             }

             int getCurrentPosition(string body);
             
             bool isBodyOpened(string body);
             
             void moveBodyRelative(string body,int position,int speed);
             void moveBodyAbsolute(string body,int position, int speed);
             void resetAll();
};
#endif//_MANUAL_COMMANDS_HPP_
