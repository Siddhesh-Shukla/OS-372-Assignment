import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection

x_intersections = []
y_intersections = []
with open("time_taken.txt") as points:
    for point in points:
        x,y = point.split()
        x_intersections.append(float(x))
        y_intersections.append(float(y))

plt.scatter(x_intersections, y_intersections, c='r',label="")
plt.xlabel('Number of Threads')
plt.ylabel('Time Take (in Seconds)')
# plt.legend()
plt.show()