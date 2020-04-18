#define LED_PIN 13
#define LONG_PRESS 500
#define BUTTONS_COUNT 2
#define DEBOUNCE_DELAY 100

#include "MIDIUSB.h"
#include <JC_Button.h>          // https://github.com/JChristensen/JC_Button

Button *buttons[BUTTONS_COUNT];

int switchPins[] = {9, 10, 11, 12, 6, 7};
int btNote[] = {61, 60, 69, 74, 79, 84}; //NOTE NUMBER
int btCC[]   = {21, 22, 23, 24, 25, 26}; //CC NUMBER //
int longPressCC[]   = {27, 28, 29, 30, 31, 32};

// https://help.ableton.com/hc/en-us/articles/209774945--Toggle-and-Momentary-MIDI-functions-in-Live
int isSwitchToggle[] = {1, 1, 1, 1, 1, 1};
int togglesState[] = {0, 0, 0, 0, 0, 0};
int longPressTogglesState[] = {0, 0, 0, 0, 0, 0};
int buttonOnHold[] = {0, 0, 0, 0, 0, 0};



void setup() {
    Serial.begin(115200); //MIDIUSB
    pinMode(LED_PIN, OUTPUT);

    setupButtons();

    delay(100);
}

void loop() {
    catchButtonEvents();
}



void setupButtons() {
    for (int i = 0; i < BUTTONS_COUNT; i++) {
        buttons[i] = new Button(switchPins[i], DEBOUNCE_DELAY);
        buttons[i]->begin();
    }
}

void catchButtonEvents() {

    for (int i = 0; i < BUTTONS_COUNT; i++) {
        processButton(i);
    }

}

void processButton(int i) {

    buttons[i]->read();

    performNoteSending(i);

    if (!detectLongPress(i))
        detectSimplePress(i);

}

void performNoteSending(int i) {
    if (buttons[i]->wasPressed()) {
        Serial.println("Note on");
        noteOn(0, btNote[i], 127);  
        MidiUSB.flush();
    }
    if (buttons[i]->wasReleased()) {
        Serial.println("Note off");
        noteOff(0, btNote[i], 127);  
        MidiUSB.flush();
    }
}

bool detectLongPress(int i) {
    if (buttons[i]->pressedFor(LONG_PRESS) && !buttonOnHold[i]) {
        buttonOnHold[i] = true;
        Serial.println("Pressed longer than 1s, holding");
    }
    if (buttons[i]->wasReleased() && buttonOnHold[i]) {
        Serial.println("Long press toggle released");
        controlChange(0, longPressCC[i], !longPressTogglesState[i] * 127); 
        longPressTogglesState[i] = !longPressTogglesState[i];
        buttonOnHold[i] = false;
        MidiUSB.flush();
        return true;
    }
    return false;
}

void detectSimplePress(int i) {
    if (isSwitchToggle[i]) {
        if (buttons[i]->wasReleased()) {
            controlChange(0, btCC[i], !togglesState[i] * 127); 
            togglesState[i] = !togglesState[i];
            MidiUSB.flush();
            Serial.println("toggle press");
        }
    } else {
        if (buttons[i]->wasPressed()) {
            controlChange(127, btCC[i], 0); 
            MidiUSB.flush();
            Serial.println("Non toggle press");
        }
        if (buttons[i]->wasReleased()) {
            controlChange(0, btCC[i], 0); 
            MidiUSB.flush();
            Serial.println("Non toggle release");
        }
    }
}


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
