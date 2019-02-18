#ifndef BASIC_H_
#include<iostream>
#include <vector>
#include <map>
#include <wiringPi.h>
#include<softPwm.h>
#define SPEEDVOLTAGECONSTANT 1800
using namespace std;
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
if(currentPosition[body]>45){
currentPosition[body]=45;
}
else if(currentPosition[body]<-45){
currentPosition[body]=-45;
}
if(position>=0){
digitalWrite(pinStore[body][0],HIGH); 
digitalWrite(pinStore[body][1],LOW); 
delay(k*position); 
}
else{
digitalWrite(pinStore[body][0],LOW); 
digitalWrite(pinStore[body][1],HIGH); 
delay(abs(position)*k); 
}
}
void moveBodyAbsolute(string body,int position,int speed){
if(position>=0){
moveBodyRelative(body,position-currentPosition[body],speed);
}
else{
std::cout<<"ERROR: absolute position can not be negative";
}
}
void resetAll(){
for(auto body:pinStore){
moveBodyRelative(body.first,-45,100);
currentPosition[body.first]=0;
}
}
 int main(int argc, char *argv[]) { 
if(wiringPiSetupPhys()==-1){ cout<<"Setup wiring pi failed"; 
return 1;
}
while(1){
moveBodyAbsolute(mouth,20,100);
cout<<currentPosition[mouth]<<endl;
moveBodyAbsolute(mouth,30,100);
cout<<currentPosition[mouth]<<endl;
//cout<<isBodyOpened(mouth)<<endl;
//resetAll();
//moveBodyRelative(mouth,-45,30);
//moveBodyRelative(mouth,45,100);
}
}
#endif //BASIC_H_
