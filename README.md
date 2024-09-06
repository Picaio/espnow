# 📡 Comunicación ESP32-ESPNOW

Este repositorio contiene un proyecto que demuestra la comunicación punto a punto (P2P) utilizando **ESPNOW** en la plataforma de microcontroladores **ESP32**. ESPNOW es un protocolo de comunicación inalámbrica que permite la comunicación sin conexión y de baja latencia entre dispositivos ESP32, ideal para el intercambio rápido de datos y redes de sensores remotos.

## ✨ Características:
- 🔋 **Bajo Consumo de Energía**: Uso eficiente de energía mediante el protocolo ESPNOW, adecuado para dispositivos alimentados por baterías.
- 🔗 **Comunicación Uno a Uno y Uno a Muchos**: Soporta tanto la comunicación directa entre dispositivos como la comunicación multicast a varios dispositivos simultáneamente.
- 🚀 **Transmisión de Datos Simple y Rápida**: Permite la transmisión de pequeños paquetes (hasta 250 bytes) sin necesidad de una conexión Wi-Fi.
- ⏱️ **Comunicación en Tiempo Real**: Proporciona comunicación de baja latencia, ideal para aplicaciones que requieren rendimiento casi en tiempo real.

## 💡 Casos de Uso:
- 🌐 Sensores remotos en aplicaciones IoT
- 🎛️ Sistemas de control en tiempo real
- 🕸️ Redes en malla para sensores de bajo consumo

## 🛠️ Instrucciones de Configuración:
1. Clona el repositorio en tu máquina local.
2. Instala el entorno de desarrollo del ESP32 en tu IDE preferido (Arduino IDE, PlatformIO, etc.).
3. Carga el código en un dispositivo ESP32.
4. Configura nodos adicionales según sea necesario para establecer la comunicación P2P o multicast.

## 📝 Código de Ejemplo:
El proyecto incluye ejemplos para los dispositivos tanto **transmisores** como **receptores**, mostrando diferentes modos de comunicación ESPNOW:
- 📤 **Modo Emisor**: El dispositivo envía mensajes periódicos a una lista predefinida de pares.
- 📥 **Modo Receptor**: El dispositivo escucha mensajes entrantes de uno o más emisores.

---

Happy coding! 😎
