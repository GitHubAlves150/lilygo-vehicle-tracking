# Passo a passo da configuração do ambiente

## 1. Construir a imagem Docker

- docker compose build

## 2. Subir o container
- docker compose up -d

## 3. Acessar o container
- docker exec -it lilygo-vehicle-tracking bash

## 4. Instalar ESP-IDF (dentro do container) 
- cd /opt/esp
- git clone --recursive --depth 1 --branch release/v5.3 https://github.com/espressif/esp-idf.git
- cd esp-idf
- ./install.sh
- . ./export.sh

## 5. Verificar instalação  
- idf.py --version # Deve mostrar: ESP-IDF v5.3.5

## 6. Configurar ambiente automático  
- echo '. /opt/esp/esp-idf/export.sh' >> ~/.bashrc  

## 7. Sair e testar novamente  
- exit
- docker exec -it lilygo-vehicle-tracking bash
- idf.py --version  # Ainda funciona!

--- 
### ..........Fim...............