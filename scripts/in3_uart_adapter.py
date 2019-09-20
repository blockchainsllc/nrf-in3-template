import serial
import requests
import json

def makeRequest(url, payload):
    all_payloads = json.loads(payload)
    all_responses = "["

    for request_payload in all_payloads:
        res = requests.post(url=url, json=request_payload)

        if res.status_code == 200:
            all_responses += res.content
        else:
            raise Exception("Couldn't get the response STATUS CODE: " + str(res.status_code))

    all_responses += "]\0"

    return all_responses

ser = serial.Serial(
    port='/dev/tty.usbserial-00000000',\
    baudrate=115200,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
    timeout=0)

print("connected to: " + ser.portstr)
read_string = ""

while True:
    read_complete = False

    for character in ser.read():
        read_string += character
        if character == '\0':
            read_complete = True
            print("read_complete")
            read_string = read_string.strip('\0')
            if len(read_string) > 0:
                print(read_string)
                url, payload = read_string.split(";")
                response = makeRequest(url, payload)
                ser.write(response);
                read_string = ""

ser.close()
