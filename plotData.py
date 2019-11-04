import matplotlib.pyplot as plt
import numpy as np

def appendSIBit(t, d, b):
    if len(t) == 0:
        t0 = 0
    else:
        t0 = t[-1]

    if b == 0:
        seq = [0, 0, 0, 1]
    else:
        seq = [0, 1, 1, 1]

    for i in range(4):
        d.append(seq[i])
        d.append(seq[i])
        t.append(t0)
        t0 = t0 + 1
        t.append(t0)

def appendSPIBit(t, d, c, b):
    if len(t) == 0:
        t0 = 0
    else:
        t0 = t[-1]

    d.append(b)
    d.append(b)
    d.append(b)
    d.append(b)
    c.append(1)
    c.append(1)
    c.append(0)
    c.append(0)
    t.append(t0)
    t0 = t0 + 2
    t.append(t0)
    t.append(t0)
    t0 = t0 + 2
    t.append(t0)



def appendPWMBit(t, p, b):
    if len(t) == 0:
        t0 = 0
    else:
        t0 = t[-1]

    p.append(1)
    p.append(1)
    p.append(0)
    p.append(0)
    p.append(1)
    p.append(1)
    p.append(0)
    p.append(0)
    t.append(t0)
    t0 = t0 + 1
    t.append(t0)
    t.append(t0)
    t0 = t0 + 1
    t.append(t0)
    t.append(t0)
    t0 = t0 + 1
    t.append(t0)
    t.append(t0)
    t0 = t0 + 1
    t.append(t0)


time = [-1, 0]
data = [1, 1]
appendSIBit(time, data, 0)
appendSIBit(time, data, 0)
appendSIBit(time, data, 0)
appendSIBit(time, data, 1)
appendSIBit(time, data, 0)
appendSIBit(time, data, 1)
appendSIBit(time, data, 1)
appendSIBit(time, data, 0)

fig, (ax1, ax2, ax3, ax4) = plt.subplots(4, sharex=True, gridspec_kw={'hspace': 0})
ax1.plot(time, data, c='b')
ax1.grid(1, 'major', 'x')

ax1.text(2, 0.5, '0', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax1.text(6, 0.5, '0', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax1.text(10, 0.5, '0', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax1.text(14, 0.5, '1', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax1.text(18, 0.5, '0', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax1.text(22, 0.5, '1', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax1.text(26, 0.5, '1', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax1.text(30, 0.5, '0', fontsize=16, horizontalalignment='center', verticalalignment='center')

time = [-1, 0]
data = [0, 0]
clk = [0, 0]
appendSPIBit(time, data, clk, 0)
appendSPIBit(time, data, clk, 0)
appendSPIBit(time, data, clk, 0)
appendSPIBit(time, data, clk, 1)
appendSPIBit(time, data, clk, 0)
appendSPIBit(time, data, clk, 1)
appendSPIBit(time, data, clk, 1)
appendSPIBit(time, data, clk, 0)

ax2.plot(time, data, c='g')
ax2.grid(1, 'major', 'x')
ax3.plot(time, clk, c='r')
ax3.grid(1, 'major', 'x')
ax3.set_xticks([0, 4, 8, 12, 16, 20, 24, 28, 32])

ax2.text(2, 0.5, '0', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax2.text(6, 0.5, '0', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax2.text(10, 0.5, '0', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax2.text(14, 0.5, '1', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax2.text(18, 0.5, '0', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax2.text(22, 0.5, '1', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax2.text(26, 0.5, '1', fontsize=16, horizontalalignment='center', verticalalignment='center')
ax2.text(30, 0.5, '0', fontsize=16, horizontalalignment='center', verticalalignment='center')


time = [-1, 0]
pwm = [0, 0]
appendPWMBit(time, pwm, 0)
appendPWMBit(time, pwm, 0)
appendPWMBit(time, pwm, 0)
appendPWMBit(time, pwm, 1)
appendPWMBit(time, pwm, 0)
appendPWMBit(time, pwm, 1)
appendPWMBit(time, pwm, 1)
appendPWMBit(time, pwm, 0)

ax4.plot(time, pwm, c='m')
ax4.grid(1, 'major', 'x')

ax1.set_yticks([])
ax2.set_yticks([])
ax3.set_yticks([])
ax4.set_yticks([])

ax1.set_ylabel('GC Data')
ax2.set_ylabel('SDO')
ax3.set_ylabel('SCK')
ax4.set_ylabel('PWM')

ax4.set_xlabel('time [$\mu s$]')

plt.show()