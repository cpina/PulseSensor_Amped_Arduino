void setup(){
  Serial.begin(9600);
  setupPulseSensor();
  setupPolarT34();
}

void loop(){
  loopPulseSensor();
  loopPolarT34();
}
