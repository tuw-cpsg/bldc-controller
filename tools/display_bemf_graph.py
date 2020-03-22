import serial
import matplotlib.pyplot as plt
import numpy as np
import struct


class DataFromController:
    s1 = None
    s2 = None
    s3 = None

    def __init__(self, s1=None, s2=None, s3=None):
        self.s1 = s1
        self.s2 = s2
        self.s3 = s3

    def __str__(self):
        return "DataFromController(s1={}, s2={}, s3={})".format(self.s1, self.s2, self.s3)


def read_data(port):
    ser = serial.Serial()
    ser.port = port
    ser.baudrate = 2000000
    ser.timeout = 10
    ser.open()
    if ser.is_open:
        print("Serial port opened: ", ser)
    else:
        print("Error opening serial port")
        exit(1)

    ser.read_until(b'\r\n')

    while True:
        l = ser.read_until(b'\r\n')
        try:
            l = l[:-2]
            # p = l.split(b';')
            # s1 = int(p[0])
            s1 = int(l)
            # s2 = int(p[1])
            # s3 = int(p[2])
            # yield DataFromController(s1=s1, s2=s2, s3=s3)
            yield DataFromController(s1=s1, s2=0, s3=0)
        except:
            print("error parsing line: " + str(l))


# use ggplot style for more sophisticated visuals
plt.style.use('ggplot')


def handle_close(evt):
    exit(0)


size = 1000
x = np.linspace(0, 1, size + 1)[0:-1]
s1_data = np.zeros(size)
s2_data = np.zeros(size)
s3_data = np.zeros(size)
plt.ion()
fig = plt.figure(figsize=(13, 6))
fig.canvas.mpl_connect('close_event', handle_close)
a1 = fig.add_subplot(111)
line1, = a1.plot(x, s1_data)
line2, = a1.plot(x, s2_data)
line3, = a1.plot(x, s3_data)
# update plot label/title
plt.ylabel('Y Label')
plt.title('BEMF')
plt.show()
plt.ylim([0, 1000])

def update():
    line1.set_ydata(s1_data)
    line2.set_ydata(s2_data)
    line3.set_ydata(s3_data)
    plt.pause(0.0000001)


c = 0
for d in read_data('/dev/ttyUSB0'):
    if d.s1 > 4000 or d.s2 > 4000 or d.s3 > 4000:
        continue
    s1_data[-1] = d.s1
    s2_data[-1] = d.s2
    s3_data[-1] = d.s3
    print("avg: {}".format(np.average(s1_data)))
    c += 1
    if c == size / 10:
        update()
        c = 0
    s1_data = np.append(s1_data[1:], 0.0)
    s2_data = np.append(s2_data[1:], 0.0)
    s3_data = np.append(s3_data[1:], 0.0)
