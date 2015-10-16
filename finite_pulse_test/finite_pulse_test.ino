#include <TimerOne.h>
#include <util/atomic.h>
#include "Streaming.h"
#include "SerialReceiver.h"

const unsigned int TRIGGER_PIN = 9;
const unsigned long TRIGGER_HIGH_US = 50000;
const unsigned int DEFAULT_TRIGGER_FREQ = 30;
const unsigned long DEFAULT_MAX_TRIGGER_COUNT = 5;

const int CMD_ID_RUN = 0;
const int CMD_ID_SET_FREQ = 1;
const int CMD_ID_SET_NUM = 2;

unsigned int triggerFreq = DEFAULT_TRIGGER_FREQ;
unsigned long maxTriggerCount = DEFAULT_MAX_TRIGGER_COUNT;
unsigned long triggerCount = maxTriggerCount; 

void triggerCallback();

SerialReceiver receiver = SerialReceiver();

void setup()
{
    Serial.begin(9600);
    pinMode(TRIGGER_PIN,OUTPUT);
    digitalWrite(TRIGGER_PIN,LOW);

    Timer1.initialize((unsigned long)(1.0e6/float(triggerFreq)));
    Timer1.attachInterrupt(triggerCallback);
    Timer1.start();
}


void loop()
{
    while (Serial.available() > 0) {
        receiver.process(Serial.read());
        if (receiver.messageReady()) {

            Timer1.stop();

            int cmdId  = receiver.readInt(0);

            Serial << "cmdId: " << cmdId << endl;


            switch (cmdId)
            {
                case CMD_ID_RUN: 
                    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                    {
                        triggerCount = 0;
                    }
                    break;

                case CMD_ID_SET_FREQ:
                    triggerFreq = (unsigned int)(receiver.readInt(1));
                    Timer1.setPeriod((unsigned long)(1.0e6/float(triggerFreq)));
                    break;

                case CMD_ID_SET_NUM:
                    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                    {
                        maxTriggerCount = (unsigned long)(receiver.readLong(1)); 
                        triggerCount = maxTriggerCount;
                    }
                    break;

                default:
                    Serial << "unknown" << endl;
                    break;

            }
            receiver.reset();
            Timer1.start();
        }
    }
}


void triggerCallback()
{
    if (triggerCount < maxTriggerCount)
    {
        digitalWrite(TRIGGER_PIN,HIGH);
        delayMicroseconds(TRIGGER_HIGH_US);
        digitalWrite(TRIGGER_PIN, LOW);
        triggerCount++;
    }
}


