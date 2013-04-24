#!/usr/bin/python

import pylab
import time
import gobject
import numpy as np
import matplotlib
import sys

file_path = '/tmp/libec_daemon_demo'

file = open(file_path, 'r')
new_data = [float(x) for x in file.readline().split()]
file.close()

v = [[x] for x in new_data] 

fig = pylab.figure()
ax = fig.add_subplot(111)
line = [[] for x in range(1,len(new_data))]
for i in range(1,len(new_data)):
  line[i-1] = ax.plot(v[0], v[i], linewidth=2.0)

ymin = new_data[1]+1
ymax = new_data[1]-1
for i in range(1,len(new_data)):
  if (new_data[i] < ymin):
    ymin = new_data[i] - 10
  if (new_data[i] > ymax):
    ymax = new_data[i] + 10

ax.set_xlim(v[0][0], v[0][0]+1)
ax.set_ylim(ymin, ymax)
pylab.xlabel('Time (s)')
pylab.ylabel('Power (W)')
pylab.legend([line[0][0], line[1][0], line[2][0]], ["WATTMETER","CPU_MINMAX","CPU_ADAPT"], loc=3)
fig.canvas.set_window_title('ectools demo')

global pause
pause = False

def update():
  global v, line, ax, fig, ymin, ymax
  
  win_size = 100
  file = open(file_path, 'r')
  new_data = [float(x) for x in file.readline().split()]
  file.close()
  if (len(new_data) > 3):
    v[0].append(new_data[0])
    for i in range(1,len(new_data)):
      v[i].append(new_data[i])
      line[i-1][0].set_data(v[0],v[i])
      if (new_data 	[i] < ymin):
        ymin = new_data[i] - 10
      if (new_data[i] > ymax):
        ymax = new_data[i] + 10
    if (len(v[0]) > win_size):
      ax.set_xlim(v[0][len(v[0])-1-win_size], v[0][len(v[0])-1])
    else:
      ax.set_xlim(v[0][0], v[0][len(v[0])-1])

    ax.set_ylim(ymin, ymax)
    fig.canvas.draw_idle()
  return True  # return False to terminate the updates

def onpress(event):
  if event.key=='q':
    sys.exit()
  elif event.key=='w':
    global pause, update_id
    if pause:
      update_id = gobject.timeout_add(1000, update)
    else:
      gobject.source_remove(update_id) 
    pause = not pause

fig.canvas.mpl_connect('key_press_event', onpress)
update_id = gobject.timeout_add(1000, update)

pylab.show()

