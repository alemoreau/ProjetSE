import serial
import time

ser = serial.Serial(
    port='/dev/ttyACM0',\
    baudrate=2000000,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.FIVEBITS,\
    timeout=1)

value = 0x00;

#print (ser)

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
print "$var wire 1 A data $end"
print "$var wire 1 B clock $end"
print "$upscope $end"
print "$enddefinitions $end"
print "$dumpvars"
out = ""
current_time = 0
start_time = 0
data = -1
clock = -1 

temps = 1;

while 1:
    donnee=ser.read(size=1)

    if(donnee != ""):
    	value =ord(donnee)

        current_time = time.time() * 10000000
        if start_time == 0:
            start_time = current_time
        #print "#" + str(temps)
        #temps +=1  
        print "#" + str(int(current_time - start_time))
        # Display data change (if any)
        if value & 0x01 != data:
            data = value & 0x01
            print str(data)+"A"     #data
        if (value & 0x02) >> 1 != clock:
            clock = (value & 0x02) >> 1
            print str(clock)+"B"    #clock

    	#print "%X" % value,
