#include <TimerOne.h>

const unsigned int TRIGGER_PIN = 9;
const unsigned int DEFAULT_DUTY_CYCLE = 2;
const unsigned int DEFAULT_FREQ = 30;
const unsigned int MAX_FREQ = 1000;
const unsigned int MIN_FREQ = 5;

void setup()
{
    Serial.begin(9600);
    pinMode(TRIGGER_PIN,OUTPUT);
    Timer1.initialize((unsigned long)(1.0e6/float(DEFAULT_FREQ)));
    Timer1.pwm(TRIGGER_PIN,DEFAULT_DUTY_CYCLE);
    Timer1.start();
}


void loop()
{
    while (Serial.available() > 1)
    {
        uint8_t freqLSB = Serial.read();
        uint8_t freqMSB = Serial.read();
        unsigned int freq = (freqMSB << 8) | freqLSB;
        freq = freq < MAX_FREQ ? freq : MAX_FREQ;
        freq = freq > MIN_FREQ ? freq : MIN_FREQ;
        Timer1.pwm(
            TRIGGER_PIN, 
            DEFAULT_DUTY_CYCLE, 
            (unsigned long)(1.0e6/float(freq))
            );
    }
    delay(10);
}


