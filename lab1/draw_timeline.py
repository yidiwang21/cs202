import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import sys

list1 = [1, 3, 2, 1, 2, 1, 3, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 3, 1, 2, 1, 2, 1, 3, 2, 1, 1, 2, 1, 1, 3, 2, 2, 3, 2, 2, 3, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3]

plt.figure()
plt.subplot(3,1,1)
plt.title('Case1: [30, 20, 10]')
for i in range(len(list1)):
    if list1[i] == 1:
        h = 1
        plt.hlines(y=h, xmin=i, xmax=i+1, lw=10, colors='r')
    elif list1[i] == 2:
        h = 0.8
        plt.hlines(y=h, xmin=i, xmax=i+1, lw=10, colors='y')
    elif list1[i] == 3:
        h = 0.6
        plt.hlines(y=h, xmin=i, xmax=i+1, lw=10, colors='c') 

plt.grid(linestyle='--', linewidth=1)
plt.xticks(np.arange(50))

list2 = [1, 3, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 2, 1, 1, 2, 1, 3, 2, 3, 2, 2, 3, 2, 2, 3, 2, 2, 3, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3]
plt.subplot(3,1,2)
plt.title('Case2: [250, 100, 50]')
for i in range(len(list2)):
    if list2[i] == 1:
        h = 1
        plt.hlines(y=h, xmin=i, xmax=i+1, lw=10, colors='r')
    elif list2[i] == 2:
        h = 0.8
        plt.hlines(y=h, xmin=i, xmax=i+1, lw=10, colors='y')
    elif list2[i] == 3:
        h = 0.6
        plt.hlines(y=h, xmin=i, xmax=i+1, lw=10, colors='c') 

plt.grid(linestyle='--', linewidth=1)
plt.xticks(np.arange(50))


# naive
list3 = [1, 3, 2, 3, 2, 1, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 1, 3] 

plt.subplot(3,1,3)
plt.title('Naive scheduling')
for i in range(len(list3)):
    if list3[i] == 1:
        h = 1
        plt.hlines(y=h, xmin=i, xmax=i+1, lw=10, colors='r')
    elif list3[i] == 2:
        h = 0.8
        plt.hlines(y=h, xmin=i, xmax=i+1, lw=10, colors='y')
    elif list3[i] == 3:
        h = 0.6
        plt.hlines(y=h, xmin=i, xmax=i+1, lw=10, colors='c') 

plt.grid(linestyle='--', linewidth=1)
plt.xticks(np.arange(50))
plt.show()
