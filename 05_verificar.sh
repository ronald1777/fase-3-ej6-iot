#!/bin/bash
# Verificacion: el servicio debe aparecer en verde como "active (running)".
sudo systemctl status greengrass.service --no-pager
echo "--- Ultimas lineas del log del nucleo ---"
sudo tail -n 20 /greengrass/v2/logs/greengrass.log
