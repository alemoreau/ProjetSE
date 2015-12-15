import time
import serial

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyACM0',
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
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
start_time = 0
data = -1
clock = -1 
while 1 :
    a = ser.read(1)
    out += a
    if out != "":
        if ord(a)%2 != data and (ord(a)/2)%2 != clock: # If any change
            # Compute timestamp
            current_time = time.time() * 1000000
            if start_time == 0:
                start_time = current_time
            print "#" + str(int(current_time - start_time))
            # Display data change (if any)
            if (ord(a)%2) != data:
                data = ord(a)%2
                print str(data)+"$"     #data
            if ((ord(a)/2)%2) != clock:
                clock = (ord(a)/2)%2
                print str(clock)+"%"    #clock
            


    
