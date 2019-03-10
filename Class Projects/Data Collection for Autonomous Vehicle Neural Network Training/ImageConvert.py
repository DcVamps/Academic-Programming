from PIL import Image

# open both text files
inFile = open("Test_data.txt", "r")
outFile = open("Final_data.txt", "w")

while True:
    data = inFile.readline()
    dataList = data.split(' ')
    picture = Image.open(dataList[0])
    # get the tuple data for each pixel in the image
    for pixel in iter(picture.getdata()):
        # write the pixel data to the Final Data File
        outFile.write(str(pixel))
        # append a comma in between each pixel, for readability
        outFile.write(",")
    # append the CAN data to the end of each line, start new line in the text document
    outFile.write(dataList[1] + "," + dataList[2] + "," + dataList[3] + "," + dataList[4] + "," + dataList[5] + "," + dataList[6] + "," + dataList[7] + "," + dataList[8] + "," + dataList[9] + '/n')
# close both files
outFile.close()
inFile.close()