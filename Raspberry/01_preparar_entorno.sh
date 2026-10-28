#!/bin/bash
# Paso 1: preparacion del entorno en la Raspberry Pi.
# AWS IoT Greengrass corre sobre Java, por lo que se instala el JRE y unzip.
set -e
sudo apt update
sudo apt install default-jre unzip -y
echo "--- Verificacion de Java ---"
java --version
