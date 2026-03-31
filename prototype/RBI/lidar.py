import matplotlib.pyplot as plt
import numpy as np
from rplidar import RPLidar, RPLidarException
import time

PORT_NAME = 'COM9'

def run_scan():
    lidar = RPLidar(PORT_NAME, timeout=3)
    try:
        lidar.stop()
        time.sleep(0.1)
        lidar.clean_input()
        lidar.start_motor()
        time.sleep(2)

        all_angles = []
        all_distances = []

        iterator = lidar.iter_scans(max_buf_meas=10000)
        
        count = 0
        while count < 10:
            try:
                scan = next(iterator)
                for (_, angle, distance) in scan:
                    all_angles.append(np.radians(angle))
                    all_distances.append(distance)
                count += 1
            except (RPLidarException, StopIteration):
                lidar.clean_input()
                continue

        if all_angles:
            ax = plt.subplot(111, projection='polar')
            ax.scatter(all_angles, all_distances, s=2, c=all_distances, cmap='hsv')
            ax.set_theta_zero_location('N')
            ax.set_theta_direction(-1)
            plt.show()

    finally:
        lidar.stop()
        lidar.stop_motor()
        lidar.disconnect()

if __name__ == '__main__':
    run_scan()
    