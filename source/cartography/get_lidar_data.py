import sys
import time
from rplidar import RPLidar, RPLidarException

def get_single_scan():
    lidar = RPLidar('/dev/ttyUSB0', timeout=3)
    try:
        lidar.stop()
        lidar.clean_input()
        lidar.start_motor()
        time.sleep(1)
        
        iterator = lidar.iter_scans(max_buf_meas=1000)
        
        for i, scan in enumerate(iterator):
            if i < 1: continue
            for (_, angle, distance) in scan:
                sys.stdout.write(f"{angle:.2f},{distance:.2f}\n")
            sys.stdout.flush()
            break
            
    finally:
        lidar.stop()
        lidar.stop_motor()
        lidar.disconnect()

if __name__ == '__main__':
    get_single_scan()