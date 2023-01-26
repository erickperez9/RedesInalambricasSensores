
from pymodbus.client import ModbusSerialClient as ModbusClient
#from pymodbus.client.sync import ModbusSerialClient as ModbusClient
import time
import matplotlib
import matplotlib.pyplot as plt
import numpy as np

client = ModbusClient(method='rtu',port="/dev/ttyUSB0",stopbits=1,bytesize=8,parity='N',baudrate=9600)

connection = client.connect()
if connection == True:
    print('Conexión Exitosa')
else:
    print('Falla en la Conexión')
time.sleep(1)

i=0
voltaje=[]
corriente=[]
while i<5:
    value = client.read_input_registers(3000,83,unit=1)
    print(type(value))
    print(value.registers)
    print('----------------------')
    date = [value.registers[74],value.registers[73],value.registers[72]]
    tt = [value.registers[75],value.registers[76],value.registers[77]]
    
    print('Fecha: '+str(date[0])+'/'+str(date[1])+'/'+str(date[2])+' |  Hora: '+str(tt[0])+':'+str(tt[1])+':'+str(tt[2]))
    print('----------------------')
    
    valor=value.registers[21]*0.1
    print(valor)
    voltaje.append(valor)
    print('Voltaje DC: '+str(round(valor,1))+' [V]')
    
    valor=value.registers[22]*0.1
    print(valor)
    corriente.append(valor)
    print('Corriente DC: '+str(round(valor,1))+' [A]')
    
    valor=value.registers[42]*0.01
    print('Frecuencia: '+str(round(valor,1))+' [Hz]')
    i+=1
    time.sleep(1)
    
print(voltaje)
plt.figure(1)
plt.plot(voltaje,linewidth=3)
plt.ylim(120,150)
plt.legend(["Voltaje DC"])
plt.xlabel('Tiempo(s)')
plt.ylabel('Voltaje DC')
plt.title('Grafica Voltaje DC')
plt.figure(2)
plt.plot(corriente,linewidth=3)
plt.ylim(5,10)
plt.legend(["Corriente DC"])
plt.xlabel('Tiempo(s)')
plt.ylabel('Corriente DC')
plt.title('Grafica Corriente DC')
plt.show()

client.close()
