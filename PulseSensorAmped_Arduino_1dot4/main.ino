void setup(){
  Serial.begin(115200);
  setupPulseSensor();
  setupPolarT34();
}

void loop(){
  loopPulseSensor();
  loopPolarT34();
}
