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

## 🚀 Status do Projeto (MVP)
- [x] Hello World - comunicação serial funcionando
- [ ] Leitura de GPS
- [ ] Conexão WiFi via hotspot do celular
- [ ] Envio de dados para API

## 📁 Estrutura do Projeto


# Configurando o projeto lilygo-vehicle-traking  
