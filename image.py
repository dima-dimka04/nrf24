import serial
from PIL import Image
import numpy as np

# Настройка порта
ser = serial.Serial('COM3', 921600)  # замени на свой порт

# Чтение данных
print("Ожидание данных...")
data = ser.read(1000000)  # читаем 1 000 000 байт
print("Готово!")

# Преобразование в изображение
img_array = np.frombuffer(data, dtype=np.uint8)

# Предположим, что изображение квадратное: 1000 x 1000
img = img_array.reshape((1000, 1000))

# Создание и отображение изображения
image = Image.fromarray(img, mode='L')  # 'L' — ч/б формат
image.show()  # открыть в стандартном просмотрщике
image.save("output_image.png")
