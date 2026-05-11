#!/bin/bash

#172.24.49.56
scp ./main.cpp ./xmake.lua pfr@10.184.234.56:/home/pfr/camera/ 
scp ./../../../pfr-core/include/vision/image.h pfr@10.184.234.56:/home/pfr/camera/pfr-core/include/vision/
scp ./../../../pfr-core/include/utils/matrix_impl.h pfr@10.184.234.56:/home/pfr/camera/pfr-core/include/utils/
scp ./../../../pfr-core/include/utils/matrix.h pfr@10.184.234.56:/home/pfr/camera/pfr-core/include/utils/

scp ./../../../pfr-core/source/vision/image.c pfr@10.184.234.56:/home/pfr/camera/pfr-core/source/vision/
scp ./../../../pfr-core/source/utils/matrix.c pfr@10.184.234.56:/home/pfr/camera/pfr-core/source/utils/
scp ./../../../pfr-core/source/utils/matrix_impl.c pfr@10.184.234.56:/home/pfr/camera/pfr-core/source/utils/
