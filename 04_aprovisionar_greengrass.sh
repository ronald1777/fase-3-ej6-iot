#!/bin/bash
# Paso 4: aprovisionamiento automatico de Greengrass.
# Uso: source 02_variables_entorno.sh && bash 04_aprovisionar_greengrass.sh NOMBRE_DISPOSITIVO
# Debe ejecutarse desde la carpeta donde se descomprimio GreengrassCore.
set -e
THING_NAME="$1"
if [ -z "$THING_NAME" ]; then
  echo "Uso: $0 NOMBRE_DISPOSITIVO"
  exit 1
fi
if [ -z "$AWS_ACCESS_KEY_ID" ] || [ -z "$AWS_SECRET_ACCESS_KEY" ]; then
  echo "Faltan las variables AWS_ACCESS_KEY_ID / AWS_SECRET_ACCESS_KEY (paso 2)."
  exit 1
fi

sudo -E java -Droot="/greengrass/v2" -Dlog.store=FILE \
  -jar ./GreengrassCore/lib/Greengrass.jar \
  --aws-region us-east-1 \
  --thing-name "$THING_NAME" \
  --thing-group-name CC3108_IoT \
  --component-default-user ggc_user:ggc_group \
  --provision true \
  --setup-system-service true \
  --deploy-dev-tools true
