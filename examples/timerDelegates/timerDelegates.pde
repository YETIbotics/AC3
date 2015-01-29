#include <PinEffector.h>
#include <SimpleTimer.h>

// globals
SimpleTimer timer;
PinEffector pin;

void someFunction(int tid){
  Serial.print("Timer id ref ");
  Serial.println(tid);
}

void paramFunction(int var){
  Serial.print("Receive ");
  Serial.println(var);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Delegates test...");

  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  
  //PinEffector uses the same SimpleTimer library, look in the .h and .cpp
  //for the code, its really basic stuff but powerfull
  pin.trigger(3, 1000);
  
  timer.setTimer(3000, someFunction, 1);
  
  //overload the setTimer function to pass your own int
  timer.setTimer(5000, paramFunction, 1, 123);
}

void update() {
  pin.update();
  timer.run();
}

void loop(){
  update();
}
