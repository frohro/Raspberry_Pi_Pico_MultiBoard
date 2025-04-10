import machine
import time
import sys

# Initialize GPIO pins
pin4 = machine.Pin(4, machine.Pin.OUT)
pin5 = machine.Pin(5, machine.Pin.OUT)

# Print header
print("GPIO Test - Toggle GPIO4 and GPIO5 every 0.25 seconds")
print("Press Ctrl+C to exit")
print("Time (s) | GPIO4 | GPIO5")
print("-----------------------")

# Start time reference
start_time = time.ticks_ms() / 1000

try:
    while True:
        # Toggle pins
        pin4.toggle()
        pin5.toggle()
        
        # Read back the actual values
        gpio4_value = pin4.value()
        gpio5_value = pin5.value()
        
        # Calculate elapsed time
        current_time = time.ticks_ms() / 1000
        elapsed = current_time - start_time
        
        # Print the results
        print(f"{elapsed:.2f}    | {gpio4_value}     | {gpio5_value}")
        
        # Wait for 0.25 seconds
        time.sleep(0.25)
        
except KeyboardInterrupt:
    print("\nProgram terminated by user")
    sys.exit()