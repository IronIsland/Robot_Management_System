import serial

serial_port = 'COM3';
baud_rate = 9600; #In arduino, Serial.begin(baud_rate)
write_to_file_path = "output.txt";

output_file = open(write_to_file_path, "w+");
#ser = serial.Serial('COM3', baud_rate)
ser = serial.Serial('COM3', 9600, bytesize=8, parity='N', stopbits=1, timeout=1)
while True:
    line = ser.readline();
    line = line.decode("utf-8") #ser.readline returns a binary, convert to string
    print(line);
    output_file.write(line);
    #ser.close();

#import serial.tools.list_ports as ports

#com_ports = list(ports.comports())  # create a list of com ['COM1','COM2']
#for i in com_ports:
  #  print(i.device)  # returns 'COMx'
