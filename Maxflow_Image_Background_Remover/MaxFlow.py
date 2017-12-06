""""""""""""""""""""""""""""""""""
By Jerry Wu, 2017/12/3 
"""""""""""""""""""""""""""""""""""
# Dependencies:
# numpy (1.13.3)
# opencv-python (3.3.0.10)
# PyMaxflow (1.2.4)
# matplotlib (2.0.2)

import numpy as np
import cv2
import maxflow
from matplotlib import pyplot as plt

paths = ["ex1.jpg", "ex2.jpg", "ex3.jpeg", "ex3.jpg", "ex4.jpg"]

# parameters
tolerance = 5000     # tolerance when similar colors are separated
image_id = 4        # id of images
reverse = True      # True if foreground is darker than background

# load image into np_array
image = cv2.imread(paths[image_id], 0)
pic = image / 256
size_x = image.shape[0]
size_y = image.shape[1]

# build plain graph
g = maxflow.GraphFloat()
nodeids = g.add_grid_nodes((size_x, size_y))
structure = [[0, 0, 0],
             [0, 0, 1],
             [0, 1, 0]]
# add edges by the difference
for i in range(size_x):
    for j in range(size_y):
        if i < size_x - 1:
            cap_col = (1 - (pic[i][j] - pic[i + 1][j]) ** 2) / tolerance
            g.add_edge(nodeids[i][j], nodeids[i + 1][j], cap_col, cap_col)
        if j < size_y - 1:
            cap_row = (1 - (pic[i][j] - pic[i][j + 1]) ** 2) / tolerance
            g.add_edge(nodeids[i][j], nodeids[i][j + 1], cap_row, cap_row)


# build max flow graph (add source and destination)
s, t = g.add_nodes(2)
for i in range(size_x):
    for j in range(size_y):
        g.add_tedge(nodeids[i][j], pic[i][j], 1 - pic[i][j])

# compute max flow
result = g.maxflow()
cut_rev = g.get_grid_segments(nodeids)
cut = list(map(lambda l: list(map(lambda x: not x, l)), cut_rev))

# merge two graph
merge_image = np.zeros((size_x, size_y))
for i in range(size_x):
    for j in range(size_y):
        if reverse:
            merge_image[i][j] = image[i][j] if cut_rev[i][j] else 255
        else:
            merge_image[i][j] = image[i][j] if cut[i][j] else 0

# show the result
print(result)
fig = plt.figure()
fig.add_subplot(2, 2, 1)
plt.title("Original Image")
plt.xticks([])
plt.yticks([])
plt.imshow(image, cmap='gray', interpolation='bicubic')
fig.add_subplot(2, 2, 2)
plt.title("Cuts by MaxFlow")
plt.xticks([])
plt.yticks([])
plt.imshow(cut, cmap='gray', interpolation='bicubic')
fig.add_subplot(2, 2, 3)
plt.title("Image with background removed")
plt.xticks([])
plt.yticks([])
plt.imshow(merge_image, cmap='gray', interpolation='bicubic')
plt.show()
