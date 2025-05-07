import os
import struct
import time

def list_input_devices():
    """List available input devices."""
    devices = [f"/dev/input/{dev}" for dev in os.listdir("/dev/input") if dev.startswith("event")]
    for device in devices:
        with open(device, 'rb') as f:
            name = f.read(256).strip().decode(errors='ignore')
        print(f"Device: {device} - {name}")

def monitor_input_device(device_path):
    """Monitor events from a specific input device."""
    try:
        with open(device_path, 'rb') as f:
            print(f"Monitoring {device_path}... Press Ctrl+C to exit.")
            
            while True:
                event = f.read(24)
                if len(event) < 24:
                    continue
                
                (tv_sec, tv_usec, ev_type, code, value) = struct.unpack('llHHI', event)
                timestamp = tv_sec + (tv_usec / 1e6)
                print(f"Timestamp: {timestamp}, Type: {ev_type}, Code: {code}, Value: {value}")
    except KeyboardInterrupt:
        print("\nMonitoring stopped.")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    #print("Available input devices:")
    #list_input_devices()
    
    device_path = input("Enter the device path to monitor (e.g., /dev/input/eventX): ")
    monitor_input_device(device_path)
