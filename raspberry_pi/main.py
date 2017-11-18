from __future__ import print_function
import serial
import time
import sys
import datetime
#from pygame import mixer # Load the required library

# connect to the arduino through serial...
ser = serial.Serial('/dev/ttyACM0', 115200)

# wait to boot
print("Waiting for Arduino to boot...") 
while True:
  if ser.read():
    break

# now the arduino is ready to go!
print("done!")

# load up all of the onset timestamps
print("Loading onsets for fade.mp3")
with open(sys.argv[1]) as file:
  onsets = file.readlines()
  onsets = map(str.strip, onsets)
  onsets = map(float, onsets)
  print(onsets)

# let's load up some music
#mixer.init()
#mixer.music.load('./link.mp3')
#mixer.music.play()

print("starting in...")
print("3")
time.sleep(1)
print("2")
time.sleep(1)
print("1")
time.sleep(1)

print("begin!")

start = datetime.datetime.now()
oIdx = 0
flashData = '1|0,4|r|30|&'

while oIdx < len(onsets):
  if start.time() + onsets[oIdx] <= datetime.datetime.now().time():
    ser.write(flashData)
    print("Flash!")
    oIdx += 1
  
  if ser.read() == '0':
    # resend the flash
    ser.write(flashData)
    print("Flash redo!")
