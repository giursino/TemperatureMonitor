#!/usr/bin/python
import binascii
import socket
import struct
import sys
import requests
import os
import ConfigParser

from optparse import OptionParser


_VERSION = "0.1.0"

# settings file is this filename with ini extension
SETTINGS_FILE = os.path.realpath(__file__).replace('.py', '.ini')

# settings for Tago.io cloud
DEFAULT_TAGO_DEVICE_TOKEN = ""

# settings for Tago.io cloud
DEFAULT_TAGO_URL = "http://api.tago.io/data"

if __name__ == "__main__":

    # Read configuration file
    config = ConfigParser.ConfigParser()
    print 'Reading %s settings file' % SETTINGS_FILE
    config.read(SETTINGS_FILE)
    TAGO_URL =  DEFAULT_TAGO_URL if not config.has_option('Tago', 'URL') else config.get('Tago', 'URL')
    TAGO_DEVICE_TOKEN =  DEFAULT_TAGO_DEVICE_TOKEN if not config.has_option('Tago', 'DEVICE_TOKEN') else config.get('Tago', 'DEVICE_TOKEN')

    # Read command line
    parser = OptionParser(version=_VERSION)
    parser.add_option("-u", "--tago-url", dest="tago_url", default=TAGO_URL,
                        help="Tago server URL.")
    parser.add_option("-d", "--tago-device-token", dest="tago_device_token", default=TAGO_DEVICE_TOKEN,
                        help="Tago device token.")

    (options, sys.argv) = parser.parse_args()

    if not options.tago_device_token:
        print >>sys.stderr, 'Invalid device token'
        sys.exit(1)

    # Create a UDS socket
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    # Connect the socket to the port where the server is listening
    server_address = '/tmp/01847-temp.socket'
    print >>sys.stderr, 'connecting to %s' % server_address
    try:
        sock.connect(server_address)
    except socket.error, msg:
        print >>sys.stderr, msg
        sys.exit(1)

    try:

        unpacker = struct.Struct('32s 32s f')

        while True:
            try:
                data = sock.recv(68)
                print >>sys.stderr, '> received %i bytes: "%s"' % (len(data),binascii.hexlify(data))

                if len(data) == 1: continue

                try:
                    unpacked_data = unpacker.unpack(data)
                    #print >>sys.stderr, 'unpacked:', unpacked_data
                except:
                    print >>sys.stderr, '! ERROR upacking message, discard it'
                    continue


                t=unpacked_data[0].split(b'\0',1)[0]
                track=unpacked_data[1].split(b'\0',1)[0]
                value=unpacked_data[2]

                print 'time=%s' % t
                print 'track=%s' % track
                print 'value=%s' % value

                if (track != "Unknown"):

                    payload = [0]*1;
                    payload[0] = {"variable":track,"value":"{:.1f}".format(value)}

                    tago_header = {"Content-type": "application/json","Device-Token":options.tago_device_token}

                    try:
                        r = requests.post(options.tago_url, json=payload, headers=tago_header, timeout=2)
                        print '* status code: %s' % r.status_code
                        r.raise_for_status()
                    except:
                        # An error has occurred, likely due to a lost internet connection,
                        # and the post has failed.
                        print("HTTP POST failed.")

            except KeyboardInterrupt:
                print 'Exiting..'
                sys.exit()

    finally:
        print >>sys.stderr, 'closing socket'
        sock.close()
