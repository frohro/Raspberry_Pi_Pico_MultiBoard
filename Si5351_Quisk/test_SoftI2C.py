from machine import Pin, SoftI2C
import time

# Define pins for SoftI2C
scl_pin = Pin(5)
sda_pin = Pin(4)

# Create SoftI2C instance with a very low frequency
soft_i2c = SoftI2C(scl=scl_pin, sda=sda_pin, freq=1000)

print("Scanning I2C bus using SoftI2C...")
devices = soft_i2c.scan()
print("Devices found:", [hex(addr) for addr in devices])

if 0x60 in devices:
    try:
        # Try reading the device status register (0x00)
        reg_addr = 0x00
        
        # First, write the register address we want to read
        soft_i2c.start()
        soft_i2c.writeto(0x60, bytearray([reg_addr]))
        time.sleep(0.01)  # Small delay
        
        # Then read the data
        data = bytearray(1)
        soft_i2c.readfrom_into(0x60, data)
        print(f"Register 0x{reg_addr:02X} value: 0x{data[0]:02X}")
        
        # Try another register - Part ID (0x00)
        reg_addr = 0x01
        soft_i2c.writeto(0x60, bytearray([reg_addr]))
        time.sleep(0.01)
        soft_i2c.readfrom_into(0x60, data)
        print(f"Register 0x{reg_addr:02X} value: 0x{data[0]:02X}")
        
    except Exception as e:
        print(f"Error reading from device: {e}")
        
        # If standard methods fail, try bit-banging manually
        print("Trying low-level bit-banging approach...")
        try:
            # Read register 0
            soft_i2c.start()
            ack = soft_i2c.writeto(0x60, bytearray([0x00]), stop=False)
            if ack:
                data = soft_i2c.readfrom(0x60, 1)
                print(f"Manual read result: 0x{data[0]:02X}")
            soft_i2c.stop()
        except Exception as e2:
            print(f"Low-level approach failed: {e2}")
else:
    print("Si5351 device (0x60) not found on the bus!")
    
print("Test complete.")