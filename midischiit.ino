#define CH 1
#define LED_PIN 13
#define MAX_LED_PIN_TIME 200 
#define PEDAL_MARGIN 64

/* MIDIUSB */

#include "MIDIUSB.h"
void noteOn(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
    MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
    MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
    midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
    MidiUSB.sendMIDI(event);
}

int switchPins[] = {9, 10, 11, 12, 6, 7};
int btNote[] = {61, 60, 69, 74, 79, 84}; //NOTE NUMBER
int btCC[]   = {21, 22, 23, 24, 25, 26}; //CC NUMBER //
int switchPreVal[] = { -1, -1, -1, -1, -1, -1 };
int switchStatus[] = {1, 1, 1, 1, 1, 1};

int ledState = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
    Serial.begin(115200); //MIDIUSB
    pinMode(LED_PIN, OUTPUT);
    for (int i = 0; i < 4; i++) {
        pinMode(switchPins[i], INPUT);
        digitalWrite(switchPins[i], HIGH);
    }
    delay(100);
}


void loop() {
    for (int i = 0; i < 4; i++) {

        int val = digitalRead(switchPins[i]);

        if (val != switchPreVal[i]) {
            lastDebounceTime = millis();
        }

        if ((millis() - lastDebounceTime) > debounceDelay) {
            
        
        
            if (val != switchStatus[i]) {
                //switchPreVal[i] = val;
                //ledState = !ledState;
    
                
      
                  switchStatus[i] = val;
      
                  if (switchStatus[i] == true) {
                      noteOn(0, btNote[i], 127);  // channel, pitch, velocity
                      controlChange(0, btCC[i], 127); //channel, CC#, value
                      MidiUSB.flush();
                      
                  } else {
                      noteOff(0, btNote[i], 127);  // channel, pitch, velocity
                      controlChange(0, btCC[i], 0); //channel, CC#, value
                      MidiUSB.flush();
       
                  }
      
              }
        }
        switchPreVal[i] = val;
    }
}
