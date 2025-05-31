import serial
from PIL import Image
import numpy as np

# Настройка порта. Чтобы не обновлял ESP при заходе
ser = serial.Serial()
ser.port = 'COM3'
ser.baudrate = 115200
ser.dtr = False
ser.rts = False
ser.open()

ser.write(b'get\n')
print("Ожидание данных...")
data1 = ser.read(5)
print(data1.decode('ascii'))
data = ser.read(1000000)
print("Готово!")

# Преобразование в изображение
img_array = np.frombuffer(data, dtype=np.uint8)
img = img_array.reshape((1000, 1000))

# Создание и отображение изображения
image = Image.fromarray(img, mode='L')  # 'L' — ч/б формат
image.show()  # открыть в стандартном просмотрщике
image.save("output_image3.png")
