// Variables Polar T34
//Definitions
const int HR_RX = 7;
const int LED = 13;
const int PIEZO = 8;


byte oldSample, sample;
boolean ledLight = false; 
unsigned long beatTimes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned long lastCalculation;

void setupPolarT34() {
  pinMode (HR_RX, INPUT);  //Signal pin to input  

  pinMode(LED, OUTPUT);


  Serial.println("setupPolarT34() finished!");
}

void showBeatTimes() {
   Serial.print(beatTimes[0]);
   Serial.print(":");
   Serial.print(beatTimes[1]);
   Serial.print(":");
   Serial.print(beatTimes[2]);
   Serial.print(":");
   Serial.print(beatTimes[3]);
   Serial.print(":");
   Serial.print(beatTimes[4]);
   Serial.print(":");
   Serial.print(beatTimes[5]);
   Serial.print(":");
   Serial.print(beatTimes[6]);
   Serial.print(":");
   Serial.print(beatTimes[7]);
   Serial.print(":");
   Serial.print(beatTimes[8]);
   Serial.print(":");
   Serial.print(beatTimes[9]);
   Serial.println("");
}

int sizeOfBeatTimes() {
   return sizeof(beatTimes) / sizeof(unsigned long); 
}

void appendIntoBeatTimes(unsigned long beatTime) {
  int i;
  for(i = 0; i < sizeOfBeatTimes()-1; i++) {
    beatTimes[i] = beatTimes[i+1];
  }
  beatTimes[sizeOfBeatTimes() - 1] = beatTime;
}

void showBpm() {
  if (beatTimes[0] != 0 && beatTimes[sizeOfBeatTimes()-1] != 0) {
    unsigned long bpm = sizeOfBeatTimes() / ((beatTimes[sizeOfBeatTimes()-1] - beatTimes[0]) / 1000.0 / 60.0);
    Serial.print("Polar      : ");
    Serial.print(bpm);
    Serial.println("bpm");
    Serial.println(sizeOfBeatTimes());
    showBeatTimes();
  }
  else {
     Serial.println("Not enough data"); 
  }
}

void loopPolarT34() {
  sample = digitalRead(HR_RX);  //Store signal output 

  if (sample && (oldSample != sample)) {
    unsigned long now = millis();
    appendIntoBeatTimes(now);

    showBpm();
    
//     blinkLed();
  }
  oldSample = sample;           //Store last signal received 
}
