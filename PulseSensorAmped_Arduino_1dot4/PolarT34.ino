// Variables Polar T34
//Definitions
const int HR_RX = 7;
const int LED = 13;
const int PIEZO = 8;


byte oldSample, sample;
boolean ledLight = false; 
unsigned long beatTimes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned long lastCalculation;
unsigned long refreshSeconds  = 2000;

void setupPolarT34() {
  pinMode (HR_RX, INPUT);  //Signal pin to input  

  pinMode(LED, OUTPUT);

  Serial.println("Waiting for heart beat...");

  //Wait until a heart beat is detected  
  while (!digitalRead(HR_RX)) {
  };
  Serial.println ("Heart beat detected!");

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
    Serial.print("BPM1:");
    Serial.print(BPM);
    Serial.println("");
  }
  else {
     Serial.println("Not enough data"); 
  }
}

void PolarT34() {
  sample = digitalRead(HR_RX);  //Store signal output 

  if (sample && (oldSample != sample)) {
    unsigned long now = millis();
    appendIntoBeatTimes(now);

    showBpm();
    /*if (now > lastCalculation + refreshSeconds) {
        showBpm();
        lastCalculation = now; 
    }*/
    
//     blinkLed();
  }
  oldSample = sample;           //Store last signal received 
}
