import machine
import time
import math
import si5351

class Radio:

    MIN_FREQUENCY = const( 3800000 )
    MAX_FREQUENCY = const( 30000000 )
    
# 	MIN_RADIX = const( 0 )
# 	MAX_RADIX = const( 6 )
# 	
# 	MODE_FREQUENCY = const( 1 )
# 	MODE_RADIX = const( 2 )
# 	
# 	STARTING_RADIX = const(2)
    
    def __init__(self, s, frequency):
        self._si5351 = s
        self._frequency = frequency
        self._mult = 100
        self._old_mult = 0
        self._si5351.clock_0.drive_strength( si5351.STRENGTH_2MA )
        self._si5351.clock_1.drive_strength( si5351.STRENGTH_2MA )

    def setFrequency(self):
        f = self._frequency
        if f < 8000000:
            self._mult = 100
        elif f < 11000000:
            self._mult = 80
        elif f < 15000000:
            self._mult = 50
        else:
            self._mult = 30

        self._si5351.set_frequency( self._frequency, self._si5351.clock_0, self._si5351.pll_a, self._mult )
        self._si5351.set_frequency( self._frequency, self._si5351.clock_1, self._si5351.pll_a, self._mult )
        
        if ( self._mult != self._old_mult ):
            self._si5351.set_phase( self._si5351.clock_1, self._si5351.pll_a, self._mult )
        self._old_mult = self._mult

    def run(self):
        #print(self._frequency)
        self.setFrequency()
        while True:
            user_input = input()
            if user_input == "FREQ,":
                print(self._frequency)
                self.setFrequency()
                print("OK")
            elif user_input.startswith("FREQ,"):
                try:
                    self._frequency = int(user_input[5:13])
                    print(self._frequency)
                    self.setFrequency()
                    print("OK")
                except ValueError:
                    print("Error: Invalid frequency")
            elif user_input.startswith("VER"):
                print("SDR Pi Pico version 0.1")
                print("OK")
            time.sleep(0.05)

