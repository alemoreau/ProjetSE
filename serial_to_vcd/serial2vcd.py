import time
import serial

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyACM0',
    baudrate=115200,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS
)

ser.isOpen()

print "$date"
print "   Coucou :)"
print "$end"
print "$version"
print "   Coucou :)"
print "$end"
print "$comment"
print "   Any comment text."
print "$end"
print "$timescale 1ps $end"
print "$scope module logic $end"
print "$var wire 1 $ data $end"
print "$var wire 1 % clock $end"
print "$upscope $end"
print "$enddefinitions $end"
print "$dumpvars"
out = ""
current_time = 0
start_time = time.time() * 1000000
while 1 :
    a = ser.read(1)
    out += a
    if out != "":
        current_time = time.time() * 1000000
    
        print "#" + str(int(current_time - start_time))
        print str(ord(a)%2)+"$"     #data
        print str((ord(a)/2)%2)+"%" #clock


    
