import argparse
import serial
import requests
import json
import signal
import sys


def make_request(url, payload):
    all_payloads = json.loads(payload)
    all_responses = "["

    for request_payload in all_payloads:
        res = requests.post(url=url, json=request_payload)

        if res.status_code == 200:
            all_responses += res.text
        else:
            raise Exception("Couldn't get the response STATUS CODE: " + str(res.status_code))

    all_responses += "]\0"

    return all_responses


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Device to connect to.')
    parser.add_argument('device', metavar='-d', type=str,
                        help='Serial/TTY devices to connect to, can be multiple, separated by spaces')
    args = parser.parse_args()

    def signal_handler(sig, frame):
        print('{} on {}'.format(sig, frame))
        print('Bye!')
        if ser:
            ser.close()
        sys.exit(0)

    signal.signal(signal.SIGINT, signal_handler)

    ser = serial.Serial(
        port=args.device,
        baudrate=115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=0)

    print("connected to: " + ser.portstr)
    print('Press Ctrl+C to exit.')
    read_string = ""

    while True:
        read_complete = False

        for character in ser.read():
            # type casting integer ascii value to character string
            character = chr(character)
            # print('received char: {}'.format(chr(character)))
            read_string += character
            if character == '\0':
                read_complete = True
                print("read_complete")
                read_string = read_string.strip('\0')
                if len(read_string) > 0:
                    print('received: {}'.format(read_string))
                    url, payload = read_string.split(";")
                    response = make_request(url, payload)
                    ser.write(response.encode());
                    read_string = ""


