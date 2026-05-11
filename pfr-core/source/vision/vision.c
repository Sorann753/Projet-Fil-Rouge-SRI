#include "vision/vision.h"



void initVision(void){
    srand(time(NULL));
}

void flagConnected(long x, long y, Matrix* flagMatrix, const Matrix* const mask){
    size_t index = matrixGetIndex(mask, x, y);
    const byte* srcBlocks = (const byte*)mask->content;
    byte* flags = (byte*)(flagMatrix->content);

    if(flags[index] == 1) return; // already visited
    if(srcBlocks[index] == 0) return; // outside of the object
    /** @post we are in the object at an unvisited point */

    flags[index] = 1;
    for(int8_t i = -1; i < 2; i++){
        for(int8_t j = -1; j < 2; j++){
            if(i == 0 && j == 0) continue; // skip yourself
            // prevent the out of bounds
            if(!isValidAccess(mask, x+i, y+j)) continue; 
            
            // flag the local neighbors
            flagConnected(x+i, y+j, flagMatrix, mask);
        }
    }
}

size_t getObjectSurface(long x, long y, const Matrix* mask, Matrix* flagMatrix, size_t depthLimit){
    if(mask == NULL) return 0;
    assert(mask->isValid);
    
    bool isFlagOwner = false;

    if(flagMatrix == NULL){
        //init the flags
        flagMatrix = initMatrixPtr(mask->lines, mask->columns, sizeof(byte));
        if(flagMatrix == NULL){ //init failed
            return 0; //degraded mode
        }
        isFlagOwner = true;

        // zero the flag matrix to avoid UB
        for(size_t i = 0; i < mask->columns; i++){
            for(size_t j = 0; j < mask->lines; j++){
                byte_matrixSet(flagMatrix, i, j, 0);
            }
        } /** @post all elements of the flagMatrix are valid */
    }
    else if(byte_matrixGet(flagMatrix, x, y) == 1){
        return 0; //already counted
    }
    else if(byte_matrixGet(mask, x, y) == 0){
        byte_matrixSet(flagMatrix, x, y, 1);
        return 0; //we're out of the object
    }
    /** @post we're in the object at an uncounted pixel */

    byte_matrixSet(flagMatrix, x, y, 1);

    if(depthLimit == 0){
        assert(!isFlagOwner); //would be weird
        return 1; 
    }

    size_t surface = 1;
    for(int8_t i = -1; i < 2; i++){
        for(int8_t j = -1; j < 2; j++){
            if(i == 0 && j == 0) continue; // skip yourself
            // prevent the out of bounds
            if(!isValidAccess(mask, x+i, y+j)) continue; 
            
            // add the uncounted neighbors
            surface += getObjectSurface(x+i, y+j, mask, flagMatrix, depthLimit-1);
        }
    }

    if(isFlagOwner){
        freeMatrix(flagMatrix); //free the content
        free(flagMatrix); //free the pointer
        flagMatrix = NULL;
    }
    return surface;
}

bool addObject(Objects* objectList, long X, long Y, size_t surface){
    if(objectList->count == objectList->capacity){
        // we need to extend the space in the array
        Object* oldElements = objectList->elements;
        size_t oldCapacity = objectList->capacity;
        objectList->capacity *= 2;
        objectList->elements = (Object*)malloc(objectList->capacity * sizeof(Object));
        if(objectList->elements == NULL){
            //TODO : handle allocation error
            free(oldElements);
            return false;
        }

        // copy the oldPoints to the new memory block
        memcpy(objectList->elements, oldElements, oldCapacity * sizeof(Object));

        free(oldElements);
        oldElements = NULL;
    } /** @post we have enough space to add the new element */

    (objectList->elements)[objectList->count] = (Object){
        .point = (IntCoordinate){
            .x = X,
            .y = Y,
        },
        .surface = surface,
    };

    (objectList->count)++;
    return true;
}

Objects findObjects(const Matrix* const mask){
    Matrix flagMatrix = initMatrix(mask->lines, mask->columns, sizeof(byte));

    const byte* srcBlocks = (const byte*)mask->content;
    byte* flags = (byte*)flagMatrix.content;

    // zero the flag matrix to avoid UB
    for(size_t x = 0; x < mask->columns; x++){
        for(size_t y = 0; y < mask->lines; y++){
            size_t index = matrixGetIndex(&flagMatrix, x, y);
            flags[index] = 0;
        }
    } /** @post all elements of the flagMatrix are valid */

    Objects connectedElements = {
        .elements = (Object*)malloc(5 * sizeof(Object)),
        .count = 0,
        .capacity = 5,
    };
    if(connectedElements.elements == NULL){
        // memory allocation failed, we abort
        freeMatrix(&flagMatrix);
        connectedElements.capacity = 0;
        return connectedElements;
    }

    for(size_t x = 0; x < mask->columns; x++){
        for(size_t y = 0; y < mask->lines; y++){
            size_t index = matrixGetIndex(mask, x, y);
            // check that this pixel isn't flagged already
            if(flags[index] == 1) continue; // already known
            if(srcBlocks[index] == 0) continue; // not in an object

            // flag all the connected pixels
            // flagConnected(x, y, &flagMatrix, mask);
            size_t objectSurface = getObjectSurface(x, y, mask, &flagMatrix, 3000);
            if(objectSurface < OBJECT_MIN_SURFACE){
                // the object is too small so we ignore it
                continue;
            }

            // commit the object to the array
            bool success = addObject(&connectedElements, x, y, objectSurface);
            if(!success){
                // TODO : handle failure case
            }
        }
    }

    // cleanup
    freeMatrix(&flagMatrix);

    return connectedElements;
}

void traceChord(const Matrix* colorMask, MathLine line, IntCoordinate* forwardSearch, IntCoordinate* backwardSearch){
    const uint16_t runawayBound = 65000;

    bool forwardHit = false;
    bool backwardHit = false;
    uint16_t k = 0;
    while((!forwardHit || !backwardHit) && k < runawayBound){
        k++;
        if(!forwardHit){
            if(!isValidAccess(colorMask, forwardSearch->x, forwardSearch->y) || forwardSearch->x < 0 || forwardSearch->y < 0){
                forwardHit = true;
                forwardSearch->x = labs(forwardSearch->x);
                forwardSearch->y = labs(forwardSearch->y);
                continue;
            }

            byte forwardVal = byte_matrixGet(colorMask, forwardSearch->x, forwardSearch->y);
            if(forwardVal > 0){
                forwardSearch->x = line.point.x + round(line.direct.vx * k);
                forwardSearch->y = line.point.y + round(line.direct.vy * k);
            }
            else{
                forwardHit = true;
            }
        }

        // if we're already done with the backward we can skip to the next loop
        if(backwardHit) continue;

        if(!isValidAccess(colorMask, backwardSearch->x, backwardSearch->y) || backwardSearch->x < 0 || backwardSearch->y < 0){
            backwardHit = true;
            backwardSearch->x = labs(backwardSearch->x);
            backwardSearch->y = labs(backwardSearch->y);
            continue;
        }

        byte backwardVal = byte_matrixGet(colorMask, backwardSearch->x, backwardSearch->y);
        if(backwardVal > 0){
            backwardSearch->x = line.point.x - round(line.direct.vx * k);
            backwardSearch->y = line.point.y - round(line.direct.vy * k);
        }
        else{
            backwardHit = true;
        }
    }
}

IntCoordinate findCenterIntersect(const Matrix* colorMask, MathLine line){
    assert(colorMask->isValid);

    IntCoordinate forwardSearch = line.point;
    IntCoordinate backwardSearch = line.point;

    traceChord(colorMask, line, &forwardSearch, &backwardSearch);

    return findMiddle(forwardSearch, backwardSearch);
}

double findRadius(const Matrix* colorMask, MathLine line){
    assert(colorMask->isValid);

    IntCoordinate forwardSearch = line.point;
    IntCoordinate backwardSearch = line.point;

    traceChord(colorMask, line, &forwardSearch, &backwardSearch);

    double forwardDist = euclidDist(forwardSearch, line.point);
    double backwardDist = euclidDist(backwardSearch, line.point);

    return (forwardDist + backwardDist) / 2;
}

bool addBall(BallArray* ballList, Ball newBall){
    if(ballList->count == ballList->capacity){
        // we need to extend the space in the array
        Ball* oldBalls = ballList->balls;
        size_t oldCapacity = ballList->capacity;
        ballList->capacity *= 2;
        ballList->balls = (Ball*)malloc(ballList->capacity * sizeof(Ball));
        if(ballList->balls == NULL){
            //TODO : handle allocation error
            free(oldBalls);
            return false;
        }

        // copy the oldPoints to the new memory block
        memcpy(ballList->balls, oldBalls, oldCapacity * sizeof(Ball));

        free(oldBalls);
        oldBalls = NULL;
    } /** @post we have enough space to add the new element */

    (ballList->balls)[ballList->count] = newBall;

    (ballList->count)++;
    return true;
}

ObjectFeature modelFeatures(IntCoordinate current, const Matrix* colorMask, Ball model, Matrix* flagMatrix, size_t depthLimit){
    assert(colorMask != NULL);
    assert(colorMask->isValid);

    bool isFlagOwner = false;
    double dist = euclidDist(model.center, current);
    byte currentValue = byte_matrixGet(colorMask, current.x, current.y);
    
    if(flagMatrix == NULL){
        //init the flags
        flagMatrix = initMatrixPtr(colorMask->lines, colorMask->columns, sizeof(byte));
        if(flagMatrix == NULL){ //init failed
            return (ObjectFeature){
                .artifactPixelCount = 0,
                .holePixelCount = 0,
            }; //degraded mode
        }
        isFlagOwner = true;

        // zero the flag matrix to avoid UB
        for(size_t i = 0; i < colorMask->columns; i++){
            for(size_t j = 0; j < colorMask->lines; j++){
                byte_matrixSet(flagMatrix, i, j, 0);
            }
        } /** @post all elements of the flagMatrix are valid */
    }
    else if(byte_matrixGet(flagMatrix, current.x, current.y) == 1){
        return (ObjectFeature){
            .artifactPixelCount = 0,
            .holePixelCount = 0,
        }; //already counted
    }
    else if(currentValue == 0 && dist > model.radius){
        byte_matrixSet(flagMatrix, current.x, current.y, 1);
        return (ObjectFeature){
            .artifactPixelCount = 0,
            .holePixelCount = 0,
        }; //we're out of the object and out of the model
    }
    /** @post we're in the object or within the model at an uncounted pixel */

    byte_matrixSet(flagMatrix, current.x, current.y, 1);

    // stop if we've gone way too far and return a malus
    if(depthLimit == 0){
        assert(!isFlagOwner); //would make no sense
        return (ObjectFeature){
            .artifactPixelCount = 1,
            .holePixelCount = 1,
        };
    }

    ObjectFeature features = {
        .artifactPixelCount = 0,
        .holePixelCount = 0,
    };

    if(currentValue == 0 && dist < model.radius){
        // we're out of the object and yet we're still in the expected circle
        // there's a hole
        features.holePixelCount++;
    }
    else if(currentValue == 1 && dist > model.radius){
        // we're still in the object but out of the expected circle
        // that's an artifact
        features.artifactPixelCount++;
    }

    for(int8_t i = -1; i < 2; i++){
        for(int8_t j = -1; j < 2; j++){
            if(i == 0 && j == 0) continue; // skip yourself
            // prevent the out of bounds
            if(!isValidAccess(colorMask, current.x+i, current.y+j)) continue; 
            IntCoordinate next = {
                .x = current.x + i,
                .y = current.y + j,
            };
            
            // add the uncounted neighbors
            ObjectFeature nearby = modelFeatures(next, colorMask, model, flagMatrix, depthLimit-1);
            features.artifactPixelCount += nearby.artifactPixelCount;
            features.holePixelCount += nearby.holePixelCount;
        }
    }

    if(isFlagOwner){
        freeMatrix(flagMatrix); //free the content
        free(flagMatrix); //free the pointer
        flagMatrix = NULL;
    }

    return features;
}

BallArray findSpheres(const Matrix* colorMask){
    assert(colorMask->isValid);

    const Objects things = findObjects(colorMask);
    //TODO : handle failure case here

    BallArray found = {
        .balls = NULL,
        .capacity = 0,
        .count = 0,
    };

    for(size_t s = 0; s < things.count; s++){
        const IntCoordinate seedPoint = things.elements[s].point;
        // we build a directing vector for the next part
        int8_t Xdirection = 0;
        int8_t Ydirection = 0;
        // TODO : handle out of bond edge case
        for(int i = -1; i < 2; i++){
            for(int j = -1; j < 2; j++){
                // prevent the out of bounds
                if(!isValidAccess(colorMask, seedPoint.x+i, seedPoint.y+j)) continue; 

                byte point = byte_matrixGet(colorMask, seedPoint.x + i, seedPoint.y + j);
                if(point){
                    Xdirection += i;
                    Ydirection += j;
                }
            }
        }
        if(Xdirection != 0){
            Xdirection = Xdirection / abs(Xdirection);
        }
        if(Ydirection != 0){
            Ydirection = Ydirection / abs(Ydirection);
        }
        if(Xdirection == 0 && Ydirection == 0){
            Xdirection = 1;
        }

        // we sample points along the line
        IntCoordinate it = seedPoint;
        bool broken = false;
        while(byte_matrixGet(colorMask, it.x, it.y) == 1 && !broken){
            it.x += Xdirection;
            it.y += Ydirection;

            // handle edge cases
            if((size_t)it.x >= colorMask->columns){
                it.x = colorMask->columns-1;
                broken = true;
            }
            if((size_t)it.y >= colorMask->lines){
                it.y = colorMask->lines-1;
                broken = true;
            }
        } /** @post it is now the index of the first 0 in that direction */
        IntCoordinate delta = {
            .x = (it.x - seedPoint.x) / (SAMPLE_POINT_COUNT + 1),
            .y = (it.y - seedPoint.y) / (SAMPLE_POINT_COUNT + 1),
        };

        IntCoordinate currentPoints[SAMPLE_POINT_COUNT];
        for(int i = 0; i < SAMPLE_POINT_COUNT; i++){
            currentPoints[i] = (IntCoordinate){
                .x = it.x - delta.x,
                .y = it.y - delta.y,
            };

            it.x -= delta.x;
            it.y -= delta.y;
        }


        MathLine lines[SAMPLE_POINT_COUNT];
        for(int i = 0; i < SAMPLE_POINT_COUNT; i++){
            // make line in random directions
            lines[i] = makeLine(currentPoints[i], makeRandomVec());
            
            // draw the chords and find their middle
            currentPoints[i] = findCenterIntersect(colorMask, lines[i]);

            // take the perpendicular of the lines
            lines[i] = perpendicular(lines[i], &(currentPoints[i]));

            // build the approximation points
            currentPoints[i] = findCenterIntersect(colorMask, lines[i]);
        }

        // make a global center estimate
        long sumX = 0;
        long sumY = 0;
        for(int i = 0; i < SAMPLE_POINT_COUNT; i++){
            sumX += currentPoints[i].x;
            sumY += currentPoints[i].y;
        }
        IntCoordinate center = (IntCoordinate){
            .x = sumX / SAMPLE_POINT_COUNT,
            .y = sumY / SAMPLE_POINT_COUNT,
        };

        // find the radius
        double radiusSum = 0;
        for(int i = 0; i < SAMPLE_POINT_COUNT; i++){
            radiusSum += findRadius(colorMask, lines[i]);
        }

        // build the virtual circle
        Ball sphere = {
            .center = center,
            .radius = round(radiusSum / (double)SAMPLE_POINT_COUNT),
        };

        // check the likelihood
        ObjectFeature features = modelFeatures(center, colorMask, sphere, NULL, 3000);
        const size_t objectSurface = things.elements[s].surface;
        
        char* holeFactorConfig = config_loader("config/visionConfig.toml", "holeFactor");
        char* artifactThresholdConfig = config_loader("config/visionConfig.toml", "artifactThreshold");

        const double holeFactor = atof(holeFactorConfig);
        const double artifactThreshold = atof(artifactThresholdConfig);

        free(holeFactorConfig);
        free(artifactThresholdConfig);

        double artifactRatio = (double)(features.artifactPixelCount) / (double)objectSurface;

        if(features.holePixelCount < objectSurface*holeFactor && artifactRatio < artifactThreshold){
            addBall(&found, sphere);
        }
    }

    free(things.elements);

    return found;
}

Cube findCube(const Matrix* colorMask){
    // TODO
    return (Cube){
        .center = (IntCoordinate){0, 0},
    };
}

/**
 * @brief estimage the direction of the object compared to the robot
 * @param ObjectCenter the estimated coordinates of an object
 * @param delta how far from the center before it's not centered anymore
 * @return the relative direction of the object, DIR_NONE being used for centered
 * 
 * @note this assume the coordinate system is zero at the center of the image
 * @note this function is estimating with a single measure and is very crude,
 * more precise estimations will be done later
 */
direction_t objectDirection(IntCoordinate ObjectCenter, int32_t delta){
    if(ObjectCenter.x < -delta){
        return DIR_LEFT;
    }
    else if(ObjectCenter.x > delta){
        return DIR_RIGHT;
    }
    else{
        return DIR_NONE;
    }
}

bool exportBallsToFile(const char* outPath, BallArray balls){
    if(balls.count == 0 || balls.balls == NULL){
        return true; // we succesfully writen nothing
    }

    FILE* file = fopen(outPath, "wb");
    if(file == NULL){
        return false;
    }

    fwrite(&(balls.count), sizeof(size_t), 1, file);

    //write the content of the image to the file
    fwrite(balls.balls, sizeof(Ball), balls.count, file); 

    fclose(file);
    return true;
}