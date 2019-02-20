/*
  Project: OpenCogER
  File: exampleManualCommands.cpp
  Author: Aman Kassahun
  License: AGPL
  Date: February, 2019
*/

#include "act/manual/manualCommands.cpp"

 int main(int argc, char *argv[]) { 


if(wiringPiSetupPhys()==-1){ cout<<"Setup wiring pi failed"; 
return 1;
}

/*the follwing are some common facial expressions
  Winking,yawning and laughing.
*/

resetAll();
//Winking
moveBodyAbsolute(forehead,0,100);
moveBodyAbsolute(forehead,20,100);
moveBodyAbsolute(check,20,100);
moveBodyAbsolute(check,0,100);


//Yawning
moveBodyAbsolute(mouth,45,100);
sleep(3);
moveBodyRelative(mouth,-5,100);//moveBodyAbsolute(mouth,43,100);
moveBodyAbsolute(mouth,45,100);
for( int i=1;i<=5;i++)
moveBodyRelative(mouth,-5,5);

//laughing
for(int i=0;i<4;i++)
{
for(int i=0;i<=16;i++)
{
moveBodyAbsolute(mouth,45,100);
moveBodyRelative(mouth,-5,100);

}
moveBodyAbsolute(mouth,35,20);
}

}

