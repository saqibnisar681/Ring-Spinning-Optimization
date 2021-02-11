import serial
import mysql.connector
from datetime import datetime
import time
count=0
data = []
serialPort = serial.Serial(port = "COM5", baudrate=9600)
serialString = ""                           # Used to hold data coming over UART
mydb = mysql.connector.connect(
host="127.0.0.1",
user="root",
password="Root123",
database="rpmdata"
,auth_plugin='mysql_native_password'
)
time.sleep(3)
while(1):
        
    # Wait until there is data waiting in the serial buffer
    if(serialPort.in_waiting > 0):
        count = count+1
        # Read data out of the buffer until a carraige return / new line is found
        serialString = serialPort.readline()
        
        # Print the contents of the serial data
        print(serialString.decode('Ascii'))
        data = serialString.split(','.encode())
##        if (count == 1):
##            count=-1
##            flag=1
##            index = index+1
        mycursor = mydb.cursor()

        sql = "INSERT INTO slave (entry, spindle_1, spindle_2, spindle_3, spindle_4, rec_time) VALUES (%s, %s, %s, %s, %s, %s)"
        val = (count,float(data[0]), float(data[1]),float(data[2]), float(data[3]), datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
        mycursor.execute(sql, val)
        mydb.commit()
        
        
