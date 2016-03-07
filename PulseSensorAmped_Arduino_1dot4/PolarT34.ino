// Variables Polar T34
//Definitions
const int HR_RX = 7;
const int LED = 13;

volatile unsigned long polar34LastBeatTime = 0;
volatile int polar34Bpm = 0;

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
   for(int i = 0; i < 9; i++)
   {
      Serial.print(beatTimes[i]);
      Serial.print(":");
   }
   Serial.println("");
   
   
   for(int i = 0; i < 9; i++)
   {
      Serial.print(rate[i]);
      Serial.print(":");
   }
   Serial.println("");
   
}

int sizeOfBeatTimes() {
   return sizeof(beatTimes) / sizeof(unsigned long); 
}

void appendIntoBeatTimes(unsigned long beatTime) {
  int i;
  word polar34RunningTotal = 0;
  
  if (polar34LastBeatTime == 0) {
    polar34LastBeatTime = beatTime;
    return; 
  }
  
  for(i = 0; i < sizeOfBeatTimes()-1; i++) {
    beatTimes[i] = beatTimes[i+1];
    polar34RunningTotal += beatTimes[i];
  }

  beatTimes[sizeOfBeatTimes() - 1] = beatTime - polar34LastBeatTime;
  polar34LastBeatTime = beatTime;

  polar34RunningTotal += beatTimes[sizeOfBeatTimes()-1];
  polar34RunningTotal /= 10;
  polar34Bpm = 60000/polar34RunningTotal;
}

void showBpm() {
  // showBeatTimes();

  if (beatTimes[0] != 0 && beatTimes[sizeOfBeatTimes()-1] != 0) {
    
    Serial.print("Polar      : ");
    Serial.print(polar34Bpm);
    Serial.println("bpm");
    // showBeatTimes();
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
