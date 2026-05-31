# ESP32 GPS Tracker com LILYGO T-A7670E

## 🎯 Objetivo do Projeto
MVP de um rastreador veicular utilizando a placa LILYGO T-A7670E (ESP32 + GPS). 
O projeto demonstra a transição de hardware para software, integrando:
- Leitura de GPS em tempo real
- Comunicação WiFi (hotspot do celular)
- Backend em Go para receber os dados (futuro)

## 📦 Hardware Utilizado
- Placa LILYGO T-A7670E R2 (com GPS)
- Bateria 18650 (opcional para testes)
- Conexão via USB-C para programação

## 🔧 Setup do Ambiente de Desenvolvimento
### Pré-requisitos
- VS Code + PlatformIO instalado
- Cabo USB-C para comunicação serial
- instalar o Docker Desktop  
- Sistema operacional utilizado; Linux Ubuntu 26.04(Resolute Racoon)
- Esp-idf

### Passo a passo
### Primeiro: Configurar o repositório remoto
1. Criar um repositório no github
1. Clone o repositório ```git@github.com:GitHubAlves150/lilygo-vehicle-tracking.git```
2. verifica se tem chave SSH ```ls -la ~/.ssh/id_ed25519.pub```   
3. Caso não tenha:  
    - Gerar nova chave SSH ``` ssh-keygen -t ed22519 -C "seu email@exemplo.com"```   
      **obs! Quando perguntar onde salvar, aperte Enter (aceita o padrão)**   
      **obs! Quando perguntar a senha, pode deixar vazia (Enter) ou colocar uma**   
    - Iniciar o agente SSH ```eval "$(ssh-agent -s)"```  
    - Adicionar a chave ao agente  ```eval "$(ssh-agent -s)"``` 
    - Copiar a chave pública (Colar no gitHub) ```cat ~/.ssh/id_ed25519.pub``` 
4. Verificar se o repositório esta sincronizado com o github
    - Entrar na pasta do projeto via terminal ```git remote -v  # Deve mostrar a URL SSH```

### Segundo: Criar os arquivos do Docker
1. Instalar o Docker Desktop
2. Verificar a instalação ``` docker --version ```   
3. Criar os arquivos padrões dentro da pasta do projeto ``` touch Dockerfile docker-compose.yml```  e ```nano Dockerfile```

## 🚀 Status do Projeto (MVP)
- [x] Configurações iniciais git/github e docker
- [ ] Hello World - comunicação serial funcionando
- [ ] Leitura de GPS
- [ ] Conexão WiFi via hotspot do celular
- [ ] Envio de dados para API

## 📁 Estrutura do Projeto
 ..........A completar.................

## Configurando o projeto lilygo-vehicle-traking  

### git/github Setup - LILYGO Vehicle Tracking

Primeiramente foi instalado o git local, criado as credenciais básicas. Em segundo, foi criado um repositório no gihub e clonado
para a máquina local. Neste projeto existe um guia de comandos de git e um guia de comandos de Docker para consultas rápidas.   

- git-github-guia.html  
- docker-guia.html 

### Docker Setup - LILYGO Vehicle Tracking  

Foi instalado o Docker engine em vez de instalar o Doker Desktop, éssa foi uma ecolha pessoal por eu estar familiarizado com os comandos de gerenciamento de versionamento de código git e para não mundar de ambiente, eu resolvi manter no console de comandos do linux para gerir os dois(terminal do linux). Outra vantagem de usar o docker engine é, docker engine é mais leve que o docker desktop pois ele consome mais recursos de memória e threads do processador. 

No arquivo **Dockerfile** esta definido a imagem usada no docker como ambiente isolado para desenvolvimento

- **FROM:** `ubuntu:22.04`
- **Propósito:** Ambiente isolado para desenvolvimento ESP-IDF

### Dependências instaladas no Dockerfile

#### Pacotes Sistema (apt)
```bash
- git               # Controle de versão
- wget, curl        # Download de arquivos
- python3, pip      # Runtime e gerenciamento de pacotes
- flex, bison       # Analisadores léxico/sintático
- gperf             # Gerador de hash perfeito
- ccache            # Acelera compilações
- dfu-util          # Upload via USB
- cmake             # Sistema de build
- libffi-dev        # Suporte a ctypes no Python
- libssl-dev        # Suporte a HTTPS
- usbutils, udev    # Acesso a dispositivos USB
- ninja-build       # Build system rápido

``` 
###Framwaorks e Ferramentas
```
- Python venv       # Ambiente virtual Python
- ESP-IDF v5.3.5    # Framework oficial Espressif
- ESP-IDF Tools     # Toolchains para ESP32

```
--- 

Após ter criado os arquivos essenciais do docker  
- Dockerfile
- docker-compose.yml  

Executei o comando ```docker conpose build ``` para criar a imagem de acordo com a confugração do Dockerfile,
o comando leu o arquivo, baixou a imagem  ubuntu:22:04 do Docker Hub, executou cada comando RUN do arquivo, instalou todas as depedẽncias (git, python, cmake, entre outros). Criou a imagem final ```lilygo-vehicle-tracking-esp32-dev ```  

Em seguida executei o comando ```docker compose up -d```, este comando criou um container a partir da imagem que foi construída, montou o volume .:/workspace (pasta dentro do container). Criou os volumes **esp_tools_cache** e **esp_idf_cache** ems eguida iniciou o container em backgroud (modo detached) sem prender o terminal para outros comandos que poderei executar como comandos do git ou até mesmo comandos de navegação do linux.

Tendo o docker rodando e com o container rodando, foi preciso entrar no terminal do container usando o comando ```docker exec -it lilygo-vehicle-tracking bash```. Dentro do terminal do ubuntu deste container foi feito instalações do esp-idf(dentro do container)

E para isto! foi preciso entrar na pasta ```cd /opt/esp``` e clonar o repositório da fabricante espressif pois o projeto todo é baseado na placa de desenvolvimento IoT lilygo A7670 com esp32 wroomE
- git clone --recursive --depth 1 --branch release/v5.3 https://github.com/espressif/esp-idf.git 
Este repositório baixa o código fonte do ESP-IDF e os sub-módulos(componentes e ferramentas que gerencia a memória do Hardware).

Depois de baixado é preciso entrar na pasta ```esp-idf``` executar o instalador ```./install.sh``` para concluir a instalação dos compiladores c/c++ e esp-df, python packages necessários e por fim configurar o ambiente de build. 

E por fim executar o comando ```export.sh``` para confugar as variaveis de ambiente para o idf.py poder funcionar no terminal(idf.py seria o mesmo que git e docker quando é preciso executar alguma comando pertinente)



### 🐳 Estrutura no Docker  
``` bash
Docker Host (seu computador)
├── Imagem: lilygo-vehicle-tracking-esp32-dev (4.5GB)
│   └── Ubuntu 22.04 + Dependências + ESP-IDF
│
└── Container: lilygo-vehicle-tracking (rodando)
    ├── Volume: /workspace → Seu código local
    ├── Volume: /opt/esp/tools → Cache toolchains
    └── Volume: /opt/esp/idf → Código ESP-IDF
``` 


