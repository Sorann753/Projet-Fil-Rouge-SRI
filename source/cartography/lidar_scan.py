import sys
import time
from rplidar import RPLidar

def run():
    lidar = None
    try:
        while True:
            line = sys.stdin.readline()
            if not line: break
            
            cmd = line.strip().upper()

            if cmd == "START":
                if not lidar:
                    lidar = RPLidar('/dev/ttyUSB0', timeout=3)
                    lidar.start_motor()
                    time.sleep(1)

            elif cmd == "SCAN":
                if lidar:
                    lidar.clean_input()
                    for scan in lidar.iter_scans():
                        for (_, angle, dist) in scan:
                            sys.stdout.write(f"{angle:.2f},{dist:.2f}\n")
                        sys.stdout.write("END\n")
                        sys.stdout.flush()
                        break
                    lidar.stop()
                    time.sleep(0.1)

            elif cmd == "EXIT":
                break
    finally:
        if lidar:
            lidar.stop_motor()
            lidar.disconnect()

if __name__ == '__main__':
    run()