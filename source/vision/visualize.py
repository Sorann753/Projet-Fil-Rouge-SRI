import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Circle
import os

inputFolder = "./../../build/linux/x86_64/debug/export/"
dataFolder = "./../../data/imageRecognized/"

def read_masks(path):
    with open(path, "rb") as f:
        # we have to assume little endian, though i don't know much hardware in big endian
        w = int.from_bytes(f.read(8), "little")
        h = int.from_bytes(f.read(8), "little")
        data = f.read(3 * w * h)
    
    return w, h, data

def read_sphere(path):
    if not os.path.exists(path):
        return (None, None)

    with open(path, "rb") as f:
        # we have to assume little endian, though i don't know much hardware in big endian
        count = int.from_bytes(f.read(8), "little")
        data = f.read(3 * 8 * count)
    
    return count, data

def display_image(w, h, data, objects):
    img = np.frombuffer(data, dtype=np.uint8).reshape((h, w, 3))

    fig, ax = plt.subplots()
    ax.set_aspect("equal")
    fig.set_facecolor("black")

    ax.imshow(img)
    for x, y, radius in objects:
        circle = Circle((x, y), radius, fill=False, linewidth=2, edgecolor="white")
        ax.add_patch(circle)

    ax.axis("off")

    count = 0
    savePath = dataFolder + "reconstructed"
    while os.path.exists(savePath + ".png"):
        count += 1
        savePath = dataFolder + "reconstructed" + str(count)

    plt.savefig(savePath + ".png", dpi=300, bbox_inches="tight")
    plt.show()


width, height, data = read_masks(inputFolder + "img.bin")
RedCount, RedSpheres = read_sphere(inputFolder + "red.bin")
greenCount, greenSpheres = read_sphere(inputFolder + "green.bin")
BlueCount, BlueSpheres = read_sphere(inputFolder + "blue.bin")
YellowCount, YellowSpheres = read_sphere(inputFolder + "yellow.bin")
blackCount, BlackSpheres = read_sphere(inputFolder + "black.bin")
whiteCount, WhiteSpheres = read_sphere(inputFolder + "white.bin")

red = None
green = None
blue = None
yellow  = None
black = None
white = None

if(RedSpheres != None):
    red = np.frombuffer(RedSpheres, dtype=np.uint64).reshape((RedCount, 3))
    print(red)
if(greenCount != None):
    green = np.frombuffer(greenSpheres, dtype=np.uint64).reshape((greenCount, 3))
    print(green)
if(BlueCount != None):
    blue = np.frombuffer(BlueSpheres, dtype=np.uint64).reshape((BlueCount, 3))
    print(blue)
if(YellowSpheres != None):
    yellow = np.frombuffer(YellowSpheres, dtype=np.uint64).reshape((YellowCount, 3))
    print(yellow)
if(BlackSpheres != None):
    black = np.frombuffer(BlackSpheres, dtype=np.uint64).reshape((blackCount, 3))
    print(black)
if(WhiteSpheres != None):
    white = np.frombuffer(WhiteSpheres, dtype=np.uint64).reshape((whiteCount, 3))
    print(white)

arr = [red, green, blue, yellow, black, white]
valid = [a for a in arr if a is not None]

if valid:
    allSpheres = np.vstack(valid)
else:
    allSpheres = np.empty((0, 3), dtype=np.uint64)

display_image(width, height, data, allSpheres)

# cleanup the folder
for name in os.listdir(inputFolder):
    path = os.path.join(inputFolder, name)
    os.remove(path)