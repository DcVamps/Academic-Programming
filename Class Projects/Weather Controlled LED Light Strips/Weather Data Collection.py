import time
import pyowm
import smbus

# initilize I2C communications as master
send = smbus.SMBus(1)
address = 0x04
# define the function to write data over the I2C line
def writeData(value):
    send.write_byte_data(address,0,value)
    return -1

owm = pyowm.OWM('29b6750a90d4e318b283784b812599d9')

loc1 = 'Flint,MI'
loc2 = 'Houston,TX'
loc3 = 'San Diego,CA'
loc4 = 'New York City,NY'
i = 0
loc = loc1
response = 0
print("loop starting")
while True:
    response = 0
    i += 1
    if i == 1:
        loc = loc1
    elif i == 2:
        loc = loc2
    elif i == 3:
        loc = loc3
    elif i == 4:
        loc = loc4
        i = 0
    print(loc) # prints the location that data is being recived from
    observation = owm.weather_at_place(loc)
    weather = observation.get_weather()
    obsTime = weather.get_reference_time('iso')
    obsStat = weather.get_status()
    obsTemp = weather.get_temperature('fahrenheit')
    print(obsTime) # prints the time data was recived
    print(obsTemp['temp']) # prints the current temperature at location
    print(obsStat) # prints the current weather descriptor at location
    # change the weather descriptors to a code to be sent to the arduino
    if obsStat == "Clouds":
        obsStatInt = 10
    elif obsStat == "Sunny":
        obsStatInt = 100
    elif obsStat == "Rain":
        obsStatInt = 20
    elif obsStat == "Snow":
        obsStatInt = 30
    elif obsStat == "Clear":
        obsStatInt = 40
    else:
        obsStatInt = 50
    print(obsStatInt)
    data = int(obsTemp['temp'])
    print(data) # prints the value for the temperature to be sent
    writeData(240) # code for sending Temp reading
    time.sleep(.5)
    writeData(data)
    data = obsStatInt
    print(data) # prints the weather descriptor code to be sent
    writeData(250) # code for sending weather condition code
    time.sleep(.5)
    writeData(data)
    time.sleep(10)
    
