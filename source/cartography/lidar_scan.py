import sys
import time
from rplidar import RPLidar, RPLidarException

def run():
    lidar = None
    port = '/dev/ttyUSB0' 
    
    while True:
        line = sys.stdin.readline()
        if not line: break
        
        cmd = line.strip().upper()

        if cmd == "START":
            if not lidar:
                try:
                    lidar = RPLidar(port, timeout=3)
                    lidar.start_motor()
                    time.sleep(1) 
                except Exception as e:
                    sys.stderr.write(f"Erreur START: {e}\n")

        elif cmd == "SCAN":
            if lidar:
                try:
                    lidar.clean_input()
                   
                    for scan in lidar.iter_scans():
                        if len(scan) > 5: 
                            for (_, angle, dist) in scan:
                                sys.stdout.write(f"{angle:.2f},{dist:.2f}\n")
                            break
                    
                    sys.stdout.write("END\n")
                    sys.stdout.flush()
                    lidar.stop()
                    time.sleep(0.5)
                    
                except (RPLidarException, Exception) as e:
                    sys.stderr.write(f"Erreur SCAN: {e}\n")
                    
                    try:
                        lidar.stop()
                        lidar.reset()
                        time.sleep(1)
                    except:
                        pass
                    sys.stdout.write("END\n")
                    sys.stdout.flush()

        elif cmd == "EXIT":
            break

    if lidar:
        lidar.stop_motor()
        lidar.disconnect()

if __name__ == '__main__':
    run()