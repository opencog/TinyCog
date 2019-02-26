/*
  File: manualCommands.cpp
  Author: Aman Kassahun <amankassahun190@gmail.com>
  License: AGPL
  Date: February, 2019
*/


#include "ManualCommands.hpp"


int main(int argc, char** argv)
{
     if(wiringPiSetupPhys()==-1)
     {
          cout<<"Setup wiring pi failed";
          return 1;
     }

     ManualCommands *manualCommands= manualCommands->getInstance();
     
     //Winking
     manualCommands->moveBodyAbsolute(forehead,0,100);
     manualCommands->moveBodyAbsolute(forehead,20,100);
     manualCommands->moveBodyAbsolute(check,20,100);
     manualCommands->moveBodyAbsolute(check,0,100);

     
     //Yawning
     manualCommands->moveBodyAbsolute(mouth,45,100);
     sleep(3);
     manualCommands->moveBodyRelative(mouth,-5,100);//moveBodyAbsolute(mouth,43,100);
     manualCommands->moveBodyAbsolute(mouth,45,100);
     for( int i=1;i<=5;i++)
     {
          manualCommands->moveBodyRelative(mouth,-5,5);
     }

     
     //laughing
     for(int i=0;i<4;i++)
     {
          for(int i=0;i<=16;i++)
          {

               manualCommands->moveBodyAbsolute(mouth,45,100);
               manualCommands->moveBodyRelative(mouth,-5,100);
          }
          manualCommands->moveBodyAbsolute(mouth,35,20);
     }

return 0;
}
