from machine import Pin, I2C
import time

i2c = I2C(0, freq=10000, scl=Pin(5), sda=Pin(4))

print("Scanning I2C bus...")
devices = i2c.scan()
print("Devices found:", [hex(addr) for addr in devices])

if 0x60 in devices:
    try:
        # Try reading a different register
        reg_addr = 0x00  # Device status register
        
        # Read using the readfrom_mem method which handles the register addressing
        data = i2c.readfrom_mem(0x60, reg_addr, 1)
        print(f"Register 0x{reg_addr:02X} value: 0x{data[0]:02X}")
    except Exception as e:
        print(f"Error reading from device: {e}")
        
        # If the above fails, try a low-level approach
        print("Trying alternative method...")
        try:
            i2c.start()
            ack = i2c.write(0x60 << 1)  # Device address with write bit
            if ack:
                ack = i2c.write(reg_addr)  # Register address
                if ack:
                    i2c.start()  # Repeated start
                    ack = i2c.write((0x60 << 1) | 1)  # Device address with read bit
                    if ack:
                        data = i2c.read(1)
                        print(f"Alternative read result: 0x{data[0]:02X}")
            i2c.stop()
        except Exception as e2:
            print(f"Alternative method failed: {e2}")