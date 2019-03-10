#imports
import RPi.GPIO as GPIO
import create
import time

#initialization
bot = create.create()
bot.startSerial()
bot.startBot()
bot.fullMode()

GPIO.setmode(GPIO.BOARD)  
buzzer_pin = 40 #set to GPIO pin 21
GPIO.setup(buzzer_pin, GPIO.IN)
GPIO.setup(buzzer_pin, GPIO.OUT)
#birthday instruction line
birthday_bit = 29
GPIO.setup(birthday_bit, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
#binary coded table lines
data_1 = 31
data_2 = 33
data_3 = 35
data_4 = 37
data_5 = 19
data_6 = 21
KIT = 23

GPIO.setup(KIT, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(data_1, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(data_2, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(data_3, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(data_4, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(data_5, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(data_6, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
#hold button to read in instruction while at kitchen
instr_btn = 7
GPIO.setup(instr_btn, GPIO.IN, pull_up_down=GPIO.PUD_UP)

#birthday trigger
birthday = False
#list of binary pin inputs
data_list = [0,0,0,0,0,0,0]
#list of destination pin inputs
destination = [0,0,0,0,0,0,0]
#table codes NA,T1,T2,T3,T4,T5,T6,K
tbl_code = [[0,0,0,0,0,0,0],[1,0,0,0,0,0,0],[0,1,0,0,0,0,0],[0,0,1,0,0,0,0],[0,0,0,1,0,0,0],[0,0,0,0,1,0,0],[0,0,0,0,0,1,0],[0,0,0,0,0,0,1]]

time.sleep(5)
print("ready")

#buzzer feature
class Buzzer(object):
    def buzz(self,pitch, duration):   #create the function buzz and feed it the pitch and duration)
     
      if(pitch==0):
       time.sleep(duration)
       return
      period = 1.0 / pitch     #in physics, the period (sec/cyc) is the inverse of the frequency (cyc/sec)
      delay = period / 2     #calcuate the time for half of the wave  
      cycles = int(duration * pitch)   #the number of waves to produce is the duration times the frequency

      for i in range(cycles):    #start a loop from 0 to the variable cycles calculated above
       GPIO.output(buzzer_pin, True)   #set pin 18 to high
       time.sleep(delay)    #wait with pin 18 high
       GPIO.output(buzzer_pin, False)    #set pin 18 to low
       time.sleep(delay)    #wait with pin 18 low

    def play(self, tune):
      GPIO.setmode(GPIO.BOARD)
      GPIO.setup(buzzer_pin, GPIO.OUT)
      x=0

      if(tune==1):
        pitches=[1047, 988,659]
        duration=[0.1,0.1,0.2]
        for p in pitches:
          self.buzz(p, duration[x])  #feed the pitch and duration to the func$
          time.sleep(duration[x] *0.5)
          x+=1

      elif(tune==2):
        pitches=[130.8,130.8,146.8,130.8,174.6,164.8,130.8,130.8,146.8,164.8,196.0,174.6,130.8,130.8,261.6,220.0,174.6,164.8,164.8,233.1,233.1,220.0,174.6,196,174.6]
        duration=[0.24,0.08,0.33,0.33,0.33,0.66,0.24,0.08,0.33,0.33,0.33,0.66,0.24,0.08,0.33,0.33,0.33,0.33,0.66,0.24,0.08,0.33,0.33,0.33,1]
        for p in pitches:
          self.buzz(p, duration[x])  #feed the pitch and duration to the func$
          time.sleep(duration[x] *0.5)
          x+=1

#      GPIO.setup(self.buzzer_pin, GPIO.IN)

#if __name__ == "__main__":
#  a = input("Enter Tune number 1-2:")
#  buzzer = Buzzer()
#  buzzer.play(int(a))

#initialize buzzer
buzzer = Buzzer()
buzzer.play(1)

while(True):
    input_state = GPIO.input(instr_btn)
    data_list[6] = GPIO.input(KIT)
    data_list[0] = GPIO.input(data_1)
    data_list[1] = GPIO.input(data_2)
    data_list[2] = GPIO.input(data_3)
    data_list[3] = GPIO.input(data_4)
    data_list[4] = GPIO.input(data_5)
    data_list[5] = GPIO.input(data_6)
    #print(data_list)
    if(input_state==False):
        print("btn down")
        if(GPIO.input(birthday_bit)==True):
            birthday = True
            buzzer.play(1)
        #if not all 0's
        elif(data_list[:] != tbl_code[0][:]):
            destination[0] = data_list[0]
            destination[1] = data_list[1]
            destination[2] = data_list[2]
            destination[3] = data_list[3]
            destination[4] = data_list[4]
            destination[5] = data_list[5]
            destination[6] = data_list[6]
            buzzer.play(1)
            time.sleep(5)
            print("driving to table")
            bot.drive(100)
            time.sleep(4)
            data_list[:] = tbl_code[0][:]
    else:              
        if(data_list[:] != tbl_code[0][:]):
            buzzer.play(1)
            print(data_list)
            
    #at destination
    if((data_list[:] == destination[:]) and (data_list[:] != tbl_code[0][:])):
        bot.drive(0)
        if(birthday == True):
            buzzer.play(2)
            birthday = False
        else:
            time.sleep(15)
        destination[:] = tbl_code[7][:]
        
    # table 1 instructions
    if(data_list[:] == tbl_code[1]):
        print("table 1")
        bot.drive(100,1)
        time.sleep(2.1)
        bot.drive(100)
        time.sleep(10.5)
    # table 2 instructions
    elif(data_list[:] == tbl_code[2]):
        print("table 2")
        bot.drive(100)
        time.sleep(10.5)
    # table 3 instructions
    elif(data_list[:] == tbl_code[3]):
        print("table 3")
        bot.drive(100,1)
        time.sleep(2.1)
        bot.drive(100)
        time.sleep(10.5)
    # table 4 instructions
    elif(data_list[:] == tbl_code[4]):
        print("table 4")
        bot.drive(100,1)
        time.sleep(2.1)
        bot.drive(100)
        time.sleep(10.5)
    # table 5 instructions
    elif(data_list[:] == tbl_code[5]):
        print("table 5")
        bot.drive(100)
        time.sleep(10.5)
    # table 6 instructions
    elif(data_list[:] == tbl_code[6]):
        print("table 6")
        bot.drive(100)
        time.sleep(1.75)
        bot.drive(100,1)
        time.sleep(2.1)
        bot.drive(100)
        time.sleep(4.5)
    # kitchen instructions
    elif(data_list[:] == tbl_code[7]):
        print("kitchen")
        bot.drive(100)
        time.sleep(.5)
        destination[:] = tbl_code[0][:]
        bot.drive(0)
    
        
