from __future__ import print_function
import serial
import time

MAX_FREQ = 1000 
MIN_FREQ = 5

class Trigger(serial.Serial):

    def __init__(self, port='/dev/ttyUSB0'):
        super(Trigger,self).__init__(port=port,baudrate=9600)
        time.sleep(2.0)

    def setFreq(self,freq):
        assert (freq >= MIN_FREQ and freq <= MAX_FREQ), 'freq out of range'
        print(freq)
        self.write(chr(freq & 0x00ff))
        self.write(chr((freq & 0xff00) >> 8))

# ------------------------------------------------------------------------
import sys

port = sys.argv[1]
freq = int(sys.argv[2])
dev = Trigger(port)
dev.setFreq(freq);

