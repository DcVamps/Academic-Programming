###########################################################
# Author: Benjamin Yats                                   #
# ID: yats6638                                            #
# Data Display for Group A Autonomous Vision System       #
###########################################################

import tkinter as tk
from PIL import Image, ImageTk

# method to change data displayed in windows when refresh button is pressed
def buttonPushDataRefresh():
    # Error handling: try to refresh display only if there is no error opening the image file
    try:

        global file_object
        # Read the current line at in the data file
        line = file_object.readline()

        # All lines in the file will adhere to the following format:
        # Name_Of_Image xx xx xx xx xx xx xx xx xx
        # Where the hex bytes 0-2 are the steering angle, the bytes 3-4 are brake, 5-6 are the acclerator, and 7-8 is vehicle speed

        # Split the string using the spaces
        data_list = line.split(' ')

        # Get the tkinter window object by invoking the global identifier
        global root

        # Get the path to the image file; this line will need to change depending on the placing of the folder containing the data
        path = "/media/capstone/SENIOR DESI/final test/obstacle avoidance 1/"+data_list[0]

        # For the next four elements (plus the time stamp grabbed from the image file name)
        for i in range(1, 6):
            # Grab the associated bytes and format as text label; each label will have Calibri font with text size 16, anchored to the right side and placed within the grid of the tkinter window
            if (i == 1):
                data_label = tk.Label(root, text="Streering Angle: "+str(data_list[1])+" "+str(data_list[2])+" "+str(data_list[3]), font=("Calibri", 16), anchor = "e").grid(row=i-1)
            elif (i == 2):
                data_label = tk.Label(root, text="Brake Pedal: "+str(data_list[4])+" "+str(data_list[5]), font=("Calibri", 16), anchor = "e").grid(row=i-1)              
            elif (i == 3):
                data_label = tk.Label(root, text="Accelerator Pedal: "+str(data_list[6])+" "+str(data_list[7]), font=("Calibri", 16), anchor = "e").grid(row=i-1)
            elif (i == 4):
                data_label = tk.Label(root, text="Vehicle Speed: "+str(data_list[8])+" "+str(data_list[9]), font=("Calibri", 16), anchor = "e").grid(row=i-1)
            elif (i == 5):
                timestamp = data_list[0].split('.')
                data_label = tk.Label(root, text="Timestamp: "+str(timestamp[0]), font=("Calibri", 16), anchor = "e").grid(row=i-1)

        # Get the canvas used to display the image by invoking the global identifier
        # Delete the last image displayed in the canvas
        global w
        w.delete("ALL")
        global fig

        # Open the image
        i = Image.open(path)

        # Convert the image to the tkinter image module
        fig = ImageTk.PhotoImage(i)

        # Create and display the image
        w.create_image(0, 0, image=fig)
        
    except IOError:
        # If there is an IO Error, let the user know
        print("The image file failed to open.")
    
# Main method to create windows, open data file, and display first line of data

# Define root as global so it can be accessed by the button refresh method
global root

# Create the main window which will display text data and contain the button to allow the user to refresh
root = tk.Tk()

# Set the data window size to 225 by 225 and set it to the side so that it does not cover the window displaying images
root.geometry("225x225+450+0")

# Give the data window a title that describes what it displays
root.title("Data Display")

global imageWindow

# Create second window to display the image
imageWindow = tk.Toplevel()

# Set the window size to 700 by 500
imageWindow.geometry("375x275")

# Give the window a descriptive title
imageWindow.title("Camera Image")

# Error handling: try to refresh display only if there is no error opening data or image file
try:
    # Create global identifier for the file object so it can be accessed by the button refresh method
    global file_object

    # Open the file and allow the object to read the file only; the file path will change based on placement of the python code and the data folder
    file_object = open("/media/capstone/SENIOR DESI/final test/obstacle avoidance 1/Test_data.txt", "r")

    # Read the current line in the file
    line = file_object.readline()

    # All lines in the file will adhere to the following format:
    # Name_Of_Image xx xx xx xx xx xx xx xx xx
    # Where the hex bytes 0-2 are the steering angle, the bytes 3-4 are brake, 5-6 are the acclerator, and 7-8 is vehicle speed

    # Split the string using the spaces
    data_list = line.split(' ')

    # Get the path to the image file; this line will need to change depending on the placing of the folder containing the data and the folder where the python code is located
    path = "/media/capstone/SENIOR DESI/final test/obstacle avoidance 1/"+data_list[0]

    # For the next four elements (plus the time stamp grabbed from the image file name)
    for i in range(1, 6):
        # Grab the associated bytes and format as text label; each label will have Calibri font with text size 16, anchored to the right side and placed within the grid of the tkinter window
        if(i == 1):
            data_label = tk.Label(root, text="Streering Angle: "+str(data_list[1])+" "+str(data_list[2])+" "+str(data_list[3]), font=("Calibri", 16), anchor = "e").grid(row=i-1)
        elif(i == 2):
            data_label = tk.Label(root, text="Brake Pedal: "+str(data_list[4])+" "+str(data_list[5]), font=("Calibri", 16), anchor = "e").grid(row=i-1)              
        elif(i == 3):
            data_label = tk.Label(root, text="Accelerator Pedal: "+str(data_list[6])+" "+str(data_list[7]), font=("Calibri", 16), anchor = "e").grid(row=i-1)
        elif(i == 4):
            data_label = tk.Label(root, text="Vehicle Speed: "+str(data_list[8])+" "+str(data_list[9]), font=("Calibri", 16), anchor = "e").grid(row=i-1)
        elif(i == 5):
            timestamp = data_list[0].split('.')
            data_label = tk.Label(root, text="Timestamp: "+str(timestamp[0]), font=("Calibri", 16), anchor = "e").grid(row=i-1)

    # Open the image
    i = Image.open(path)

    # Convert the image to the tkinter image module
    fig = ImageTk.PhotoImage(i)

    # Create global identifier for the canvas so that it can be accessed by the button refresh method
    global w

    # Create the canvas in the Camera Image window and give it size 700 by 500
    w = tk.Canvas(imageWindow, width=400, height=400)

    # Pack the canvas into the window
    w.pack()

    # Create and display the image
    w.create_image(0, 0, image=fig)
    
except IOError:
    # If there is an IO Error, let the user know
    print("One of the files did not open")

# Create the button with descriptive text and set the method to the already-defined refresh method and place it in the grid of the Data Display window
B = tk.Button(root, text="New Data Set", command=buttonPushDataRefresh).grid(row=7)

# Run the main loop of the tkinter window
root.mainloop()
