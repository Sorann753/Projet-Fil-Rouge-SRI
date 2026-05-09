import sys
import time
import struct
from rplidar import RPLidar

# Constante pour la commande PWM
SET_PWM_BYTE = b'\xF0'

def run():
    lidar = None
    port = '/dev/ttyUSB0' 
    
    try:
        while True:
            line = sys.stdin.readline()
            if not line: break
            
            cmd = line.strip().upper()

            if cmd == "START":
                if not lidar:
                    try:
                        lidar = RPLidar(port, baudrate=115200, timeout=3)
                        lidar._serial.flushInput()
                        lidar.start_motor()
                        time.sleep(1)

                        pwm_value = 100
                        payload = struct.pack('<H', pwm_value)
                        lidar._send_payload_cmd(SET_PWM_BYTE, payload)
                        
                        time.sleep(2)
                        lidar.clean_input()
                    except Exception as e:
                        sys.stderr.write(f"Erreur START: {e}\n")
                        lidar = None

            elif cmd == "SCAN":
                if lidar:
                    try:
                        lidar._serial.flushInput() 
                        
                        for scan in lidar.iter_scans(max_buf_meas=1000,):
                            if len(scan) > 5:
                                for (_, angle, dist) in scan:
                                    sys.stdout.write(f"{angle:.2f},{dist:.2f}\n")
                                break
                        
                        sys.stdout.write("END\n")
                        sys.stdout.flush()
                        lidar.stop() 
                    except Exception as e:
                        sys.stderr.write(f"Erreur SCAN: {e}\n")
                        sys.stdout.write("END\n")
                        sys.stdout.flush()

            elif cmd == "EXIT":
                break
    finally:
        if lidar:
            try:
                lidar.stop()
                lidar.stop_motor()
                lidar.disconnect()
            except:
                pass

if __name__ == '__main__':
    run()