import serial
import time
import numpy as np  # Ensure you have imported NumPy

def read_position(ser):
    """Send 'e' command and read the position from the Arduino."""
    # print("reading pos")
    ser.write(b'e\r')  # Request position
    response = ser.read_until(b'\x03').decode().strip().split()
    print(f"response: {response}") 
    
    # Extract first valid float from response 
    try:
        return float(response[0])
    except ValueError:
        print(f"Invalid response: {response}")
        return None 

def send_position_command(ser, p_next):
    """Send position command 'p <p_next>' to the Arduino."""
    command = f'p {p_next:.6f}\r' 
    ser.write( command.encode() ) 
    print(f"Sent: {command.strip()}")

    response = ser.read_until(b'\x03').decode().strip()
    print(f"Response: {response}") 

def init(ser): 
    print("initialising")
    ser.write(b"i odrive 1 19200 Serial1\r") 
    time.sleep(2) 
    response = ser.read_until(b'\x03').decode().strip() 
    print(f"Response: {response}") 

def main():
    port = '/dev/ttyACM0'  # Update based on your system
    baud_rate = 115200  # Adjust as needed

    try:
        ser = serial.Serial(port, baud_rate, timeout=1)
        print("Connected to:", ser.name)
        time.sleep(2)  # Allow connection to stabilize

        # Flush buffers
        ser.reset_input_buffer()
        ser.reset_output_buffer()

        # Initialize ODrive
        time.sleep(1)
        init(ser) 

        t0 = time.time()  # Record the start time
        cmd_pos = 0

        while True:
            t = time.time() - t0  # Calculate elapsed time
            position = read_position(ser) 
            
            if position is not None:
                print(f"Time: {t:.2f} s | Cmd pos: {cmd_pos:.2f} rad | Current Position: {position:.2f} rad | diff: {cmd_pos - position : .2f}")
                cmd_pos = 2 * np.pi * np.sin(2 * np.pi * t / 10)  # One cycle per second
                send_position_command(ser, cmd_pos)
            
            time.sleep(0.1)  # Ensure smooth motion, adjust if needed
    
    except serial.SerialException as e:
        print(f"Serial error: {e}")
    
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Serial connection closed.")

if __name__ == "__main__":
    main()
