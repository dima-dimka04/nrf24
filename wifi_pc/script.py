import serial
from PIL import Image
import numpy as np
import time

ser = serial.Serial()
ser.port = 'COM3'
ser.baudrate = 921600
ser.dtr = False
ser.rts = False
ser.open()
ser.write(b'get\n')
print("Ожидание данных...")
data1 = ser.read(5)
print(data1.decode('ascii'))
start_time = time.time()
data = ser.read(1000000)
end_time = time.time()
elapsed = end_time - start_time
print(f"Готово! Время приёма: {elapsed:.2f} секунд")
#print("Готово!")

img_array = np.frombuffer(data, dtype=np.uint8)
img = img_array.reshape((1000, 1000))
image = Image.fromarray(img, mode='L')
image.show()
image.save("output_image8.png")
