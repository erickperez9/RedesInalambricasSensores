import os
import time
import sys
import paho.mqtt.client as mqtt
import json
import random
from pymodbus.client import ModbusSerialClient as ModbusClient

cliente = ModbusClient(method='rtu',port="/dev/ttyUSB0",stopbits=1,bytesize=8,parity='N',baudrate=9600)

connection = cliente.connect()
if connection == True:
    print('Conexión Exitosa')
else:
    print('Falla en la Conexión')
time.sleep(1)

#THINGSBOARD_HOST = 'demo.thingsboard.io'
#ACCESS_TOKEN = 'xZw7gs5mEg4lG9oUvBiD'
#ACCESS_TOKEN = 'Yt7SiedHhObkdIesCLf5'

THINGSBOARD_HOST = '192.168.52.151'
ACCESS_TOKEN = 'Solis'



# Data capture and upload interval in seconds. Less interval will eventually hang the DHT22.
INTERVAL=2

sensor_data = {'voltaje': 0, 'corriente': 0,'frecuencia': 0,'VAC': 0, 'IAC': 0,}

next_reading = time.time() 

client = mqtt.Client()

# Set access token
client.username_pw_set(ACCESS_TOKEN)

# Connect to ThingsBoard using default MQTT port and 60 seconds keepalive interval
client.connect(THINGSBOARD_HOST, 1883, 60)

client.loop_start()
telemtryData = dict()

try:
    while True:
        value = cliente.read_input_registers(3000,83,unit=1)
        date = [value.registers[74],value.registers[73],value.registers[72]]
        tt = [value.registers[75],value.registers[76],value.registers[77]]
        
        voltaje=value.registers[21]*0.1
        corriente=value.registers[22]*0.1
        frecuencia=value.registers[42]*0.01
        VAC=value.registers[35]*0.1
        IAC=value.registers[38]*0.1
        
        print('Frecuencia: '+str(round(frecuencia,1))+' [Hz]')
        print('Voltaje DC: '+str(round(voltaje,1))+' [V]')
        print('Corriente DC: '+str(round(corriente,1))+' [A]')
        print('Voltaje AC: '+str(round(VAC,1))+' [V]')
        print('Corriente AC: '+str(round(IAC,1))+' [A]')
        sensor_data['voltaje'] = voltaje
        sensor_data['corriente'] = corriente
        sensor_data['frecuencia'] = frecuencia
        sensor_data['VAC'] = VAC
        sensor_data['IAC'] = IAC
        client.publish('v1/devices/me/telemetry', json.dumps(sensor_data),1)

        next_reading += INTERVAL
        sleep_time = next_reading-time.time()
        if sleep_time > 0:
            time.sleep(sleep_time)
except KeyboardInterrupt:
    pass

client.loop_stop()
client.disconnect()
