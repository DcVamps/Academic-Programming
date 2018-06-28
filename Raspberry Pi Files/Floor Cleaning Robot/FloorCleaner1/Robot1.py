import asyncio
import time
import smbus
import RPi.GPIO as GPIO
import numpy,random, GridMap
from BlockSparseMatrix import BlockSparseMatrix
from BresenhamAlgorithms import BresenhamLine,BresenhamTriangle,BresenhamPolygon

# initializations
bus = smbus.SMBus(0)
GPIO.setmode(GPIO.board)

# I2C Addresses
WIRELESS_ADDRESS = 0x25
DC_MOTOR_ADDRESS = 0x50
PUMP_ADDRESS = 0x40

# Pin Assignments
ULTRASONIC_L_TRIGGER = 24
ULTRASONIC_L_ECHO = 26
ULTRASONIC_C_TRIGGER = 32
ULTRASONIC_C_ECHO = 36
ULTRASONIC_R_TRIGGER = 38
ULTRASONIC_R_ECHO = 40
'''
INFRARED_1 = 29
INFRARED_2 = 31
INFRARED_3 = 33
INFRARED_4 = 35
'''
BATTERY_LOW = 37 #battery low interrupt pin

ROBOT_RADUIS = 25 # Radius of the robot /  distance of the sensor to the center


# GPIO Assigments
GPIO.setup(ULTRASONIC_L_TRIGGER, GPIO.OUT)
GPIO.setup(ULTRASONIC_L_ECHO, GPIO.IN)
GPIO.setup(ULTRASONIC_C_TRIGGER, GPIO.OUT)
GPIO.setup(ULTRASONIC_C_ECHO, GPIO.IN)
GPIO.setup(ULTRASONIC_R_TRIGGER, GPIO.OUT)
GPIO.setup(ULTRASONIC_R_ECHO, GPIO.IN)
'''
GPIO.setup(INFRARED_1, GPIO.IN)
GPIO.setup(INFRARED_2, GPIO.IN)
GPIO.setup(INFRARED_3, GPIO.IN)
GPIO.setup(INFRARED_4, GPIO.IN)
'''

GPIO.setup(BATTERY_LOW, GPIO.IN)

# I2C communication functions
def run_vacuum():
    bus.write_byte(DC_MOTOR_ADDRESS, 0x00)

def stop_vacuum():
    bus.write_byte(DC_MOTOR_ADDRESS, 0x01)

def run_pump():
    bus.write_byte(PUMP_ADDRESS, 0x01)

def stop_pump():
    bus.write_byte(PUMP_ADDRESS, 0x00)

def send_wireless_message(message):
    #0x01 = low battery message
    #0x02 = low cleaning fluid
    #0x03 = full vacuum chamber
    #0x04 = high battery message
    bus.write_byte(WIRELESS_ADDRESS, message)

async def read_wireless_message():
    incomingMessage = bus.read_byte(WIRELESS_ADDRESS, 0)
    await return incomingMessage

# Motor Control Functions
def move_forward(speed):
    #0x02 = normal speed
    #0x03 = half speed
    if speed == 150:
        bus.write_byte(DC_MOTOR_ADDRESS, 0x02)
    if speed == 75:
        bus.write_byte(DC_MOTOR_ADDRESS, 0x03)

def turn_right(angle):
    #0x07 = 90 degrees
    #0x08 = 90 degrees
    if angle == 90:
        bus.write_byte(DC_MOTOR_ADDRESS, 0x07)
    if angle == 45:
        bus.write_byte(DC_MOTOR_ADDRESS, 0x08)
    bus.read_byte(DC_MOTOR_ADDRESS, 0)

def turn_left(angle):
    #0x05 = 90 degrees
    #0x06 = 45 degrees
    #0xff = special turn 15 degrees
    if angle == 90:
        bus.write_byte(DC_MOTOR_ADDRESS, 0x05)
    if angle == 45:
        bus.write_byte(DC_MOTOR_ADDRESS, 0x06)
    if angle == 15: #special turn for mapping purposes mainly
        bus.write_byte(DC_MOTOR_ADDRESS, 0xff)
    bus.read_byte(DC_MOTOR_ADDRESS, 1)

def reverse_motion(speed):
    #0x09 = normal speed
    #0x0a = half speed
    if speed == 150:
        bus.write_byte(DC_MOTOR_ADDRESS, 0x09)
    if speed == 75:
        bus.write_byte(DC_MOTOR_ADDRESS, 0x0a)

def stop_motion():
    bus.write_byte(DC_MOTOR_ADDRESS, 0x0b)

def turn_around():
    bus.write_byte(DC_MOTOR_ADDRESS, 0x04)
    bus.read_byte(DC_MOTOR_ADDRESS, 0)

# Ultrasonic Triggers/response
async def right_ultrasonic():
    GPIO.output(ULTRASONIC_R_TRIGGER, False)
    await time.sleep(.5)
    GPIO.output(ULTRASONIC_R_TRIGGER, True)
    await time.sleep(.00001)
    GPIO.output(ULTRASONIC_R_TRIGGER, False)
    while GPIO.input(ULTRASONIC_R_ECHO) == 0:
        pulse_start = time.time()
    while GPIO.input(ULTRASONIC_R_ECHO) == 1:
        pulse_end = time.time()
    pulse = pulse_end - pulse_start
    distance =pulse*17150
    distance = round(distance,4)
    return distance

async def center_ultrasonic():
    GPIO.output(ULTRASONIC_C_TRIGGER, False)
    await time.sleep(.5)
    GPIO.output(ULTRASONIC_C_TRIGGER, True)
    await time.sleep(.00001)
    GPIO.output(ULTRASONIC_C_TRIGGER, False)
    while GPIO.input(ULTRASONIC_C_ECHO) == 0:
        pulse_start = time.time()
    while GPIO.input(ULTRASONIC_C_ECHO) == 1:
        pulse_end = time.time()
    pulse = pulse_end - pulse_start
    distance = pulse * 17150
    distance = round(distance, 4)
    return distance

async def left_ultrasonic():
    GPIO.output(ULTRASONIC_L_TRIGGER, False)
    await time.sleep(.5)
    GPIO.output(ULTRASONIC_L_TRIGGER, True)
    await time.sleep(.00001)
    GPIO.output(ULTRASONIC_L_TRIGGER, False)
    while GPIO.input(ULTRASONIC_L_ECHO) == 0:
        pulse_start = time.time()
    while GPIO.input(ULTRASONIC_L_ECHO) == 1:
        pulse_end = time.time()
    pulse = pulse_end - pulse_start
    distance = pulse * 17150
    distance = round(distance, 4)
    return distance

# Localization Assistance with Wireless Beacons
async def determine_RSSI_distance(rssi):
    distance = 10**((TXpower - rssi)/20)
    return distance

async def determineFloor():
    '''
        use downward facing ultrasonics to determine the floor type under the robot
    '''

# Read sesnor Data for mapping
def getSensorData(a,b,theta):
    rightDistance = right_ultrasonic()
    map.update([a, b, theta], rightDistance, rightAngle, sensor)
    centerDistance = center_ultrasonic()
    map.update([a, b, theta], centerDistance, 0, sensor)
    leftDistance = left_ultrasonic()
    map.update([a, b, theta], leftDistance, leftAngle, sensor)

# Ultrasonic mapping function
async def define_map():
    sensor = {"spread": 15.*numpy.pi/180., "range": 400., "phitfree": -0.3, "phitoccupied": 3.}
    scale = 100
    gridScale = 0.5
    map = GridMap(scale=gridScale)
    rightAngle = 3**numpy.pi/4
    leftAngle = numpy.pi/4

    '''
        travel in a square-like pattern starting from the base station
        after defining the initial mapped area, go back to look for edge cases
            such as opening and other pathways
    '''

    rightDistance = right_ultrasonic()
    map.update([0,0,0],rightDistance,rightAngle,sensor)
    centerDistance = center_ultrasonic()
    map.update([0,0,0],centerDistance,0,sensor)
    leftDistance = left_ultrasonic()
    map.update([0,0,0],leftDistance,leftAngle,sensor)
    # determineFloor()
    reverse_motion(75)
    await time.sleep(1.5)
    stop_motion()
    centerDistance2 = center_ultrasonic()
    turn_around()

    theta = numpy.pi
    '''
        a = x position
        b = y position
        !! distanceOffset is the distance of travel at speed 75 for 1.5 seconds !!
        theta = angle of rotation & direction of travel
    '''
    distanceOffset = centerDistance2 - centerDistance

    a = 0
    b = 0 - distanceOffset #traveling in the negative Y direction
    while theta < 3 ** numpy.pi :
        getSensorData()

    centerDistance = center_ultrasonic()
    while centerDistance > 5:
        theta = numpy.pi
        move_forward(75)
        await time.sleep(1.5)
        stop_motion()
        b = b - distanceOffset
        # determineFloor()
        while theta < 3 ** numpy.pi :
            getSensorData(a,b,theta)
            theta = theta + (15 * numpy.pi / 180)
            turn_left(15)
        centerDistance = center_ultrasonic()

    turn_left(90)
    '''
    move_forward(75)
    await time.sleep(1.5)
    stop_motion()
    a = a + distanceOffset #traveling in the positive X direction
    '''

    centerDistance = center_ultrasonic()
    while centerDistance > 5:
        theta = numpy.pi
        move_forward(75)
        await time.sleep(1.5)
        stop_motion()
        a = a + distanceOffset
        # determineFloor()
        while theta < 3 ** numpy.pi:
            getSensorData(a, b, theta)
            theta = theta + (15 * numpy.pi / 180)
            turn_left(15)
        centerDistance = center_ultrasonic()

    turn_left(90)
    '''
    move_forward(75)
    await ime.sleep(1.5)
    stop_motion()
    b = b + distanceOffset #traveling in the positive Y direction
    '''

    centerDistance = center_ultrasonic()
    while centerDistance > 5:
        theta = numpy.pi
        move_forward(75)
        await
        time.sleep(1.5)
        stop_motion()
        b = b + distanceOffset
        # determineFloor()
        while theta < 3 ** numpy.pi:
            getSensorData(a, b, theta)
            theta = theta + (15 * numpy.pi / 180)
            turn_left(15)
        centerDistance = center_ultrasonic()
    '''
    move_forward(75)
    await ime.sleep(1.5)
    stop_motion()
    a = a - distanceOffset #traveling in the negative X direction
    '''

    centerDistance = center_ultrasonic()
    while centerDistance > 5:
        theta = numpy.pi
        move_forward(75)
        await time.sleep(1.5)
        stop_motion()
        a = a - distanceOffset
        # determineFloor()
        while theta < 3 ** numpy.pi:
            getSensorData(a, b, theta)
            theta = theta + (15 * numpy.pi / 180)
            turn_left(15)
        centerDistance = center_ultrasonic()
    '''
    Need to catch edge cases for odd shaped rooms
    unless the 15 degree turn with 3 sensors 45 degrees apart
    will catch those cases
    **this will be past of testing**
    '''
    #save map

def checkForMap():
    #check memory for a saved map
    return MAP

def returnToBaseStation():
    '''
        travels from anywhere in thr map to the base station
        in the most efficient manner possible
    '''

# Main motion control
def main():
    '''
        This is where the magic happens

        using the map created before in define_map()
        traverse the floor area first vaccuming everything
            then a second pass with the wet cleaning solution + pad
            for tile and hardwood surfaces

    '''
if checkForMap() == False:
    define_map()

main()
try:
    GPIO.wait_for_edge(BATTERY_LOW, GPIO.RISING)
    message = bus.read_byte(DC_MOTOR_ADDRESS,1)
    if message .contains("LB"):
        returnToBaseStation()
    if message.contains{"HB"}:
        send_wireless_message(0x04)

except:
    #keybard interrupt