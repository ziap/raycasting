#!/usr/bin/env python
import sys

import cv2
import numpy as np

in_file, out_file = sys.argv[1:]

np.set_printoptions(formatter={'int':hex})

img = cv2.imread(in_file).astype(np.uint32)

# Remove alpha channel
img = img[:,:,:3]

height, width = img.shape[:2]

# Flatten image
img.resize(img.shape[0] * img.shape[1], img.shape[2])

# Convert to RGBA format
heximg = (img[:,0] << 16) | (img[:,1] << 8) | (img[:,2] << 0)

# Make alpha channel fully opaque
heximg |= 0xff000000

# Format header file
data = """
namespace %s {
constexpr int width = %d;
constexpr int height = %d;
constexpr unsigned data[%d] = {%s};
}
""" % (out_file[:-2].upper(), width, height, width * height, ','.join([hex(i) for i in heximg]))

# Print the data, I can write to file but I prefer piping in the makefile
print(data)
