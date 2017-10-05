#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

with open("bubble_result.txt") as f:
    data1 = f.read()

with open("merge_result.txt") as f:
    data2 = f.read()

data1 = data1.split(' ')[:-1]
data2 = data2.split(' ')[:-1]
x = range(5, 10005, 5)


fig = plt.figure()
ax1 = fig.add_subplot(211)
ax1.set_title('Bubble Sort')
ax1.set_xlabel('no of words')
ax1.set_ylabel('time')
ax1.plot(x, data1, 'r-')

ax2 = fig.add_subplot(212)
ax2.set_title('Merge Sort')
ax2.set_xlabel('no of words')
ax2.set_ylabel('time')
ax2.plot(x, data2, 'b-')

plt.show()
