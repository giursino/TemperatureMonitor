# Temperature Monitor

An application to monitor the temperature using Vimar By-me thermostat
devices.

The program has a client/server architecture that permits to monitor 
a particular message on knx bus that is the temperature datapoint.         

The server use the `libknxusb` library to hear the temperature knx 
datapoint and sends it to `/tmp/01847-temp` socket.

The client `temperature-to-cloud.py` written in Python it connects 
to the socket and the send the temperature data to the online 
datastore to plot it, using [tago.io](tago.io).

Another client `temperature-to-file.py` written in Python it connects
to the socket and then write the temperature data to `data.csv` CSV file

# Publish a package

```
mkdir build
cd build
cmake ..
make package
make package_source
```