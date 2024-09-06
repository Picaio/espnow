# 📡 Comunicación ESP-NOW en ESP32: Emisor y Receptor

Este repositorio contiene un proyecto que muestra cómo configurar y programar **ESP-NOW** en ESP32 para la comunicación sin conexión Wi-Fi. En este ejemplo práctico, un dispositivo actúa como **emisor**, enviando valores de su pin analógico, mientras que otro dispositivo como **receptor** controla la luminosidad de un LED en función de los datos recibidos.

## ✨ Características:
- **Protocolo ESP-NOW**: Comunicación sin Wi-Fi, de baja latencia entre dispositivos ESP32.
- **Transmisión de Datos**: El emisor envía valores leídos del pin analógico.
- **Control de LED**: El receptor ajusta la intensidad del LED usando `digitalWrite()` basado en los valores recibidos.

## 🛠️ Instrucciones:
1. Configura ambos ESP32 (emisor y receptor).
2. Programa el emisor para leer datos del pin analógico y enviar los datos mediante ESP-NOW.
3. Programa el receptor para recibir los datos y ajustar el brillo del LED.

## 📝 Ejemplos:
- 📤 **Emisor**: Lee datos del pin analógico y los envía.
- 📥 **Receptor**: Recibe datos y ajusta el brillo del LED.

Happy coding! 😎
