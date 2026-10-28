#!/usr/bin/env python3
"""Cliente de prueba para el broker MQTT local de la Raspberry Pi.

Instalacion:  pip install paho-mqtt
Publicar:     python3 cliente_prueba_mqtt.py pub  esp32/temperatura 25.4
Suscribirse:  python3 cliente_prueba_mqtt.py sub  esp32/#
"""
import sys
import time
import paho.mqtt.client as mqtt

BROKER = "localhost"   # Cambiar por la IP de la Raspberry si se corre desde otra maquina
PUERTO = 1883


def crear_cliente():
    try:
        # paho-mqtt 2.x
        return mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    except AttributeError:
        # paho-mqtt 1.x
        return mqtt.Client()


def publicar(topico, mensaje):
    cliente = crear_cliente()
    cliente.connect(BROKER, PUERTO, 60)
    cliente.loop_start()
    info = cliente.publish(topico, mensaje, qos=1)
    info.wait_for_publish()
    print(f"Publicado en {topico}: {mensaje}")
    cliente.loop_stop()
    cliente.disconnect()


def suscribir(topico):
    def al_conectar(c, userdata, flags, rc, *args):
        print(f"Conectado al broker (codigo {rc}), suscrito a {topico}")
        c.subscribe(topico, qos=1)

    def al_mensaje(c, userdata, msg):
        print(f"[{time.strftime('%H:%M:%S')}] {msg.topic}: {msg.payload.decode(errors='replace')}")

    cliente = crear_cliente()
    cliente.on_connect = al_conectar
    cliente.on_message = al_mensaje
    cliente.connect(BROKER, PUERTO, 60)
    cliente.loop_forever()


if __name__ == "__main__":
    if len(sys.argv) >= 4 and sys.argv[1] == "pub":
        publicar(sys.argv[2], sys.argv[3])
    elif len(sys.argv) >= 3 and sys.argv[1] == "sub":
        suscribir(sys.argv[2])
    else:
        print(__doc__)
