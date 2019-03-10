from PIL import Image
import datetime
import pyrealsense as pyrs
import time
import asyncio
import sys

sys.path.append("~/Downloads/Canlib/Samples/Python")
import canlib

pyrs.start()
global cam
cam = pyrs.Device(device_id = 0, streams = [pyrs.ColourStream(fps = 60)])
time.sleep(3)
cl = canlib.canlib()
file = open("Test_data.txt", "w")

# Open up communication channel using Kvaser + canlib library
channel = 0
handle1 = cl.openChannel(channel, canlib.canOPEN_ACCEPT_VIRTUAL)
handle1.setBusOutputControl(canlib.canDRIVER_NORMAL)
handle1.setBusParams(canlib.canBITRATE_500K)
# canSetAcceptanceFilter function is not available on Linux
# otherwise we could configure the Kvaser module to apply CAN
# filtering before it sends the msg
#handle1.canSetAcceptanceFilter(0xFFF, 0x000)
handle1.busOn()

# x represents bits we are interested in
# 0 if the bit is always 0
# 1 if the bit is always 1
# -----------------------------------------------------
# Steering Angle magnitude: 16-bits
# Varies from 0x80 00 up until 0xA5 10
# Varies from (+ 430* to - 430*)
#
# CANData[0] = MSByte - magnitude for steering angle 
#				[xxxx][xxxx]
# CANData[1] = LSByte - magnitude for steering angle
#			 	[xxxx][xxxx]
# -----------------------------------------------------
# Steering Angle Direction: 8-bits
# Either 0x80 or 0x00
# CANData[2] = MSb represents direction
#			   1 = Clockwise (Right)
#			   0 = Counter-Clockwise (Left)
#				[x000][0000]
# ------------------------------------------------------
# Brake: 16-bits
# Varies from 0x00 00 to 0x05 00
#
# CANData[3] = MSByte of brake
#				[0000][xxxx]
# CANData[4] = LSByte of brake
#				[xxxx][xxxx]
# ------------------------------------------------------
# Acceleration: 16-bits
# Varies from 0x00 00 to 0x03 E8
#
# CANData[5] = MSByte of accelerator
#				[0000][xxxxx]
# CANData[6] = LSByte of accelerator
#				[xxxx][xxxx]
# -------------------------------------------------------
# Vehicle Speed: 16-bits
# Varies from 0x 00 00 to 0x?? ??
#
# .006 to convert to mph
# .010 to convert to kph
#
# CANData[7] = MSByte of vehicle speed
#				[xxxx][xxxx]
# CANData[8] = LSByte of vehicle speed
#				[xxxx][xxxx]
# -------------------------------------------------------
CANData = [0, 0, 0, 0, 0, 0, 0, 0, 0]

async def takeImageSaveData():
	# wait for the camera to have a new frame available. camera refreshes at 60fps
	# capture frame that has become available
	cam.wait_for_frame()
	im = Image.fromarray(cam.colour, 'RGB')

	# take and save image to a jpeg file
	file_place = str(datetime.datetime.now().hour).zfill(2) + "_" + str(datetime.datetime.now().minute).zfill(2) + "_" + str(datetime.datetime.now().second).zfill(2) + "-" + str(datetime.datetime.now().microsecond).zfill(6) + ".jpeg"
	im.save("/media/capstone/SENIOR DESI/Images/" + file_place, "jpeg")
	im1 = Image.open("/media/capstone/SENIOR DESI/Images/" + file_place)

	# save image file name and can data to text document
	file.write(file_place + " " + "{:02x} {:02x} {:02x} {:02x} {:02x} {:02x} {:02x} {:02x} {:02x}".format(CANData[0], CANData[1], CANData[2], CANData[3], CANData[4], CANData[5], CANData[6], CANData[7], CANData[8]) + '\n')

	# change this delay to change relative fps of the system
	# must be a minimum of 20ms to allow for complete capture of CAN data
	await asyncio.sleep(.0645) # number of seconds for this function to be sleeping

async def getCanData():
	while True:
		# Access CAN bus using canlib .read() command
		# Returns a tuple called msg
		# msg[0] = CAN Bus identifier (decimal)
		# msg[1] = CAN bus byte array, 8 bytes
		# 	msg[1][x] accesses the X byte of the byte array
		msg = handle1.read(50)

		# Steering Angle
		# ID: 0x10
		#  - Magnitude: B6 B7 
		#  - Direction: MSb B4
		# 24 bits
		if (msg[0] == 16):
			CANData[0] = msg[1][6]
			CANData[1] = msg[1][7]
			CANData[2] = (msg[1][4] & 0x80)
	
		# Brake
		# ID: 0x7D
		# lower nibble B4
		# B5
		# 16 bits
		if (msg[0] == 125):
			CANData[3] = (msg[1][4] & 0x0f)
			CANData[4] = msg[1][5]

		# Accel
		# ID: 0x204
		# lower nibble B0
		# B1
		# 16 bits
		if (msg[0] == 516):
			CANData[5] = (msg[1][0] & 0x0f)
			CANData[6] = msg[1][1]
	
		# Vehicle Speed
		# ID: 0x160
		# B2
		# B3
		# 16 bits
		if (msg[0] == 352):
			CANData[7] = msg[1][2]
			CANData[8] = msg[1][3]
		
		await takeImageSaveData()

loop = asyncio.get_event_loop()
loop.run_until_complete(getCanData())
loop.close()
pyrs.stop()
file.close()

