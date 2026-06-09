

<div align="center" markdown="1">
  <img src="https://github.com/GitHubAlves150/Consumo-Api/blob/main/img/ChatGPT%20Image%20Jun%201,%202026,%2012_25_07%20PM.png?raw=true" alt="LilyGo logo" width="700"/>
</div>

<h1 style="text-align: center; margin-top: 100px;">lilygo-vehicle-tracking</h1>

<div align="center">

<h2>SOBRE O PROJETO</h2>
<p style="text-indent: 2.5em;         
        text-align: justify;        
        line-height: 1.6;          
        font-family: Arial, sans-serif; 
        font-size: 16px;            
        color: #c1bbbb; ">
    Este projeto(MVP) é um <strong>rastreador veicular</strong> desenvolvido com a placa de desenvolvimento <strong>LILYGO T-A7670E</strong> (Esp32 + 4G + GPS Externo + Bateria). O objetivo deste projeto é demonstrar minha transição de carreira, unindo conhecimentos de Hardware aplicados na área de desenvolvimento de placas eletrônicas com <strong>software web(backend Golang), banco de dados, API, microserviços</strong>
</p>
<p style="text-indent: 2.5em;         
        text-align: justify;        
        line-height: 1.6;          
        font-family: Arial, sans-serif; 
        font-size: 16px;            
        color: #c1bbbb;  ">
    A placa Lilygo T-A7670E é um kit de desenvolvimento da própria empresa chinesa LilyGO, líder mundial P&D(Projeto & Desenvolvimento) em internet das coisas. Por possuir os quatro módulo essenciais de um rastreador veicular(Esp32 + 4G + GPS + Bateria), ter um hardware confiável e com muitas documentações de fábrica eu escolhi <strong>lilygo T-A7670E</strong> como sendo o hardware necessário para coletar dados de GPS e disponibilizar via API na web para fins de estudos e para fins profissionais para demoonstrar minhas habilidades em Engenharia de software e Engenharia de Eletrônica entre outras tecnologias atuais.
</p>
<p style="text-indent: 2.5em;         
        text-align: justify;        
        line-height: 1.6;          
        font-family: Arial, sans-serif; 
        font-size: 16px;            
        color: #c1bbbb;  ">
        O cerne deste projeto é, coletar dados GPS e envia-lo via API para a nuvem usando Golang como linguagem backend principal.
    A arquitetura do projeto compôe o firmware do Esp32(módulo inserido na placa LilyGo) escrito em linguagem CPP(C++) a nível de Hardware para gerenciar a memória dinâmica do microcontrolador e também, gerir o barramento UART de comunicação do GPS com o Esp32. Para este projeto, está sendo utilizado serviços AWS cloud com serverless Lambda, banco de dados DynamoDB e API Gateway para disponibilizar os dados de coordenadas GPS em formato JSON em qualquer navegador e ser utilizado em microserviços como páginas web.
</p>

<p style="text-indent: 2.5em;         
        text-align: justify;        
        line-height: 1.6;          
        font-family: Arial, sans-serif; 
        font-size: 16px;            
        color: #c1bbbb;  ">
    <strong>Diferencial deste projeto:</strong> Capacidade de entender os dois mundos - firmware embarcado para o microcontrolador Esp32 escrito em C++, gerenciamento de memória dinâmica. Este projeto demonstra um amplo entendimento em como hardware se comunicam via nuvem(handshake).
<p>

</div>

### Funcionalidades aplicadas a este projeto(MVP)

- [x] Ambiente Docker isolado para desenvolvimento (ubuntu 22:04)
- [x] Configuração do ESp32 
- [x] Build e flash automatizados com ESP-IDF
- [x] Firmware de teste para validação da placa lilygo T-A7670(ver as braches para este fim)
- [x] Desenvolvimento backend em Golang
- [x] Configuração AWS Cloud.
- [x] Configuração serverless Lambda.
- [x] Configuração do banco de dados NoSql DynamoDB.
- [x] Configuração da API Gateway.
- [x] URL pública para consulta das coordenadas geográficas.

### Tecnologias Utilizadas para o desenvolvimento deste MVP(Minimo produto viável)

| Componente | Tecnologia |
|------------|------------|
| **Hardware** | LILYGO T-A7670E (ESP32 + 4G + GPS) |
| **Firmware** | C++, ESP-IDF v5.3.5 |
| **Build System** | CMake, Ninja |
| **Container** | Docker, Docker Compose |
| **Editor de código** | VSCode |
| **Versionamento** | Git, GitHub (SSH) |
| **Backend** | Golang |
| **Arquitetura serverless** | AWS (Lambda, DynamoDB, API Gateway)|


 ### Como usar.


<p style="text-indent: 2.5em;         
        text-align: justify;        
        line-height: 1.6;          
        font-family: Arial, sans-serif; 
        font-size: 16px;            
        color: #c1bbbb;  ">
    O projeto lilygo-vehicle-tracking pode ser clonado com o seguinte comando git clone git@github.com:GitHubAlves150/lilygo-vehicle-tracking.git e pode ser gravado usando um terminal linux. È preciso ter instalado o docker Engine(não desktop). Baixando apenas o projeto lilygo-vehicle-tracking pode-se ter acesso as coordenadas via serial(dados brutos na tela usando um cabo USB-C), pois no desenvolvimento do projeto foi preciso validar a clareza dos dados. Quanto a conexão wifi foi utilizado Hotspot do celular para poder transportar pela cidade, mas a placa lilyGo tem estrutura elétrica para comportar SIM-Card 4G.
</p>

### Consulte a documentação do firmware ESP-IDF em [FIRMWARE.md](/docs/FIRMWARE.md)

<p style="text-indent: 2.5em;         
        text-align: justify;        
        line-height: 1.6;          
        font-family: Arial, sans-serif; 
        font-size: 16px;            
        color: #c1bbbb;  ">
    O projeto lambda_project pode ser clonado com o comando git clone git@github.com:GitHubAlves150/lambda_project.git. O projeto lambda_project é o backend escrito em Golang e deve ser concultado a documentação BACKEND.md primeiro e em seguida consultado a documentação AWS.md para abrir resuisições /POST/GET para envio e consulta dos dados via URL em qualquer navegador de qualquer lugar do mundo.
</p>

### Consulte a documentação do backend em [BACKEND.md](/docs/BACKEND.md) e a documentação da configuração do AWS cloud em [AWS.md](/docs/AWS.md)

<p style="text-indent: 2.5em;         
        text-align: justify;        
        line-height: 1.6;          
        font-family: Arial, sans-serif; 
        font-size: 16px;            
        color: #c1bbbb;  ">
Para todas as branch deste repositório, é preciso subir o container <strong>lilygo-vehicle-tracking</strong>. È preciso também ter em mãos a placa  <a href="https://lilygo.cc/en-us/products/t-sim-a7670e?srsltid=AfmBOor6rdfBl_cieeBHlcDflmsB9XpvkZ3t6VhoQHLQuaOGv8QbgviG">LILYGO T-A7670</a> para poder abrir conexão com o container <strong>OU</strong> se apenas preferir navegar pelo projeto basta entrar no arquivo <strong>docker-compose.yml e comentar a linha 16 que diz respeito ao compartilhamente das portas do container com a máquina local(notebook, desktop..)</strong>
</p>


```bash 
devices:
      - "/dev/ttyACM0:/dev/ttyACM0"   # ← comentar esta linha para poder subir o container
```
<div>
    <p style="text-indent: 2.5em;         
        text-align: justify;        
        line-height: 1.6;          
        font-family: Arial, sans-serif; 
        font-size: 16px;            
        color: #c1bbbb;  ">
    O projeto foi desenvolvido todo em sistema linux versão 26.04 resolute, mas caso esteja usando windows, basta instalar o WSL(Subsistema do Windows para Linux) para virtualizar o sistema linux
    </p>
    <h3>Pré-requisitos</h3>
    <ul>
            <li>Instalar o docker Engine(não Docker Desktop)</li>
            <li>Placa lilygo T-A7670E</li>
            <li>Antena GPS ligada</li>
            <li>VSCode Instalado</li>    
            <li>Ler a documentação AWS.md e BACKEND.md</li>           
    </ul>
    
</div>

 <h3>Comandos docker engine a serem usados para apenas monitorar os dados via serial</h3>


```bash
# Abra o VSCode e abra um novo terminal(ctrl + j)

# 1. Iniciar o container
docker compose up -d

# 2. verificar se o container foi inicializado
docker ps    # Ou 
docker ps -a #para ver todos os containers existentes

# 3. Entrar no container
docker exec -it lilygo-vehicle-tracking bash

# 4. Entrar na pasta do projeto e compilar o firmware
cd /workspace/vehicle_tracker && idf.py build

# 5. Gravar na placa
idf.py -p /dev/ttyACM0 flash

# 6. Monitorar a saída
idf.py -p /dev/ttyACM0 monitor

```

<div align="center" markdown="1">
    <h1>Saída serial</h1>
</div>

![alt text](/Repositorio-de-Imagens/image-20.png)

<div align="center" markdown="1">
    <h1>Dados em formato JSON acessado de um navegador</h1>
</div>

![alt text](/Repositorio-de-Imagens/image-21.png)




## 📚 Bibliografia e Referências

### 1. Fundamentação do Projeto

<p class="texto-formatado">
    Este projeto foi desenvolvido como parte da minha <strong>transição de carreira de Desenvolvimento de Hardware para desenvolvimento de software backend</strong>, com o objetivo em demonstrar a capacidade de unir conhecimentos de eletrônica, firmware embarcado e desenvolvimento backend em nuvem. A escolha do tema "rastreador veicular" surgir de um feedback de uma entrevista de emprego no Mercado Livre, para demonstrar o conhecimento em API, banco de dados, Golang entre outras tecnologias web como AWS. Criei então com base em um caso real para enviar dados reais para a nuvem e serem acessados de qualquer lugar do mundo. Assim demostrando meus conhecimento em Engenharia eletrônica e software.
</p>

### 2. Hardware e Documentação Oficial

- **LILYGO Official**. *T-A7670E / T-A7670G ESP32 4G/LTE/GPRS/GNSS Development Board*. Disponível em: [https://lilygo.cc/products/t-sim-a7670e](https://lilygo.cc/products/t-sim-a7670e). Acesso em: maio/jun. 2026.

- **Espressif Systems**. *ESP-IDF Programming Guide v5.3.5*. Disponível em: [https://docs.espressif.com/projects/esp-idf/](https://docs.espressif.com/projects/esp-idf/). Acesso em: maio/jun. 2026.

- **Quectel**. *L76K GNSS Module Datasheet*. Disponível em: [https://files.waveshare.com/upload/d/db/Quectel_L76K_Hardware_Design_V1.0.pdf/](https://files.waveshare.com/upload/d/db/Quectel_L76K_Hardware_Design_V1.0.pdf). 

- **Random Nerd Tutorials**. *Getting Started with LILYGO TTGO T-A7670G ESP32 (4G, GPRS, and GPS)*. Disponível em: [https://RandomNerdTutorials.com/lilygo-ttgo-t-a7670g-a7670e-a7670sa-esp32/](https://RandomNerdTutorials.com/lilygo-ttgo-t-a7670g-a7670e-a7670sa-esp32/). Acesso em: jun. 2026.

### 3. Firmware e ESP-IDF

- **Espressif Systems**. *ESP32 Technical Reference Manual*. Disponível em: [https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf). Acesso em: maio/jun. 2026.

- **FreeRTOS**. *FreeRTOS Kernel Quick Start Guide*. Disponível em: [https://www.freertos.org/](https://www.freertos.org/). Acesso em: maio/jun. 2026.

### 4. Docker

- **Docker Inc**. *Docker Engine Documentation*. Disponível em: [https://docs.docker.com/engine/](https://docs.docker.com/engine/). Acesso em: maio/jun. 2026.

- **Docker Inc**. *Docker Compose Documentation*. Disponível em: [https://docs.docker.com/compose/](https://docs.docker.com/compose/). Acesso em: maio/jun. 2026.


### 5. Git e GitHub

- **Chacon, S.; Straub, B**. *Pro Git Book*. Disponível em: [https://git-scm.com/book/en/v2](https://git-scm.com/book/en/v2). Acesso em: maio/jun. 2026.

- **GitHub Docs**. *Getting started with GitHub*. Disponível em: [https://docs.github.com/](https://docs.github.com/). Acesso em: maio/jun. 2026.


### 6. Golang e Backend

- **Donovan, A. A.; Kernighan, B. W**. *The Go Programming Language*. Addison-Wesley Professional, 2015. (Obra de referência sobre a linguagem Go).

- **AWS Official Documentation**. *AWS Lambda Developer Guide*. Disponível em: [https://docs.aws.amazon.com/lambda/](https://docs.aws.amazon.com/lambda/). Acesso em: jun. 2026.

- **AWS Official Documentation**. *Amazon API Gateway Developer Guide*. Disponível em: [https://docs.aws.amazon.com/apigateway/](https://docs.aws.amazon.com/apigateway/). Acesso em: jun. 2026.

- **AWS Official Documentation**. *Amazon DynamoDB Developer Guide*. Disponível em: [https://docs.aws.amazon.com/dynamodb/](https://docs.aws.amazon.com/dynamodb/). Acesso em: jun. 2026.

### 7. Cursos e Capacitação

- **joão Ribeiro**. *Curso Git/Github - Do Zero ao Avançado*. Udemy. Disponível em: [https://www.udemy.com/course/git-github-rapido-no-essencial-ate-profissional/learn/lecture/54087419?start=0#overview](https://www.udemy.com/course/git-github-rapido-no-essencial-ate-profissional/learn/lecture/54087419?start=0#overview).

- **joão Ribeiro**. *Curso Docker - Do Zero ao Avançado*. Udemy. Disponível em: [https://www.udemy.com/course/docker-zero-a-profissional-para-desenvolvimento-web/learn/lecture/47385635?start=0#overview](https://www.udemy.com/course/docker-zero-a-profissional-para-desenvolvimento-web/learn/lecture/47385635?start=0#overview).

- **Diego Fernando de Souza Lima**. *Golang - Do Zero ao Avançado*. Udemy. Disponível em: [https://www.udemy.com/course/golang-dos-fundamentos-ao-desenvolvimento-web-e-llms/learn/lecture/48868035?start=135#overview](https://www.udemy.com/course/golang-dos-fundamentos-ao-desenvolvimento-web-e-llms/learn/lecture/48868035?start=135#overview).

- **Stephany Henrique de Almeida Batista**. *Curso Docker - Do Zero ao Avançado*. Udemy. Disponível em: [https://www.udemy.com/course/golang-do-zero-ao-avancado/learn/lecture/35904446?start=0#overview](https://www.udemy.com/course/golang-do-zero-ao-avancado/learn/lecture/35904446?start=0#overview).

- **Felipe Mafra**. *Curso Banco de dados MySql - Do Zero ao Avançado*. Udemy. Disponível em: [https://www.udemy.com/course/bancos-de-dados-relacionais-basico-avancado/learn/lecture/19042442?start=0#overview](https://www.udemy.com/course/bancos-de-dados-relacionais-basico-avancado/learn/lecture/19042442?start=0#overview).


### 8. Inteligência Artificial (Ferramentas de Apoio ao Desenvolvimento)

- **DeepSeek**. *DeepSeek AI Assistant*. Versão utilizada: maio/jun. 2026. Disponível em: [https://www.deepseek.com/](https://www.deepseek.com/). Utilizado para: consultas técnicas sobre ESP-IDF, Docker, Git, Go, AWS e revisão da documentação.

- **Google Gemini (via Google AI Studio)**. *Gemini Advanced*. Versão utilizada: maio/jun. 2026. Disponível em: [https://gemini.google.com/](https://gemini.google.com/). Utilizado para: auxílio na correção do código firmware (C++), especialmente no `gps_parser.cpp`, e debugging da comunicação UART e envio HTTPS para AWS.

### 9. Agradecimentos e Motivação

<p class="texto-formatado">
    Este projeto não seria possível sem a oportunidade de participar do processo seletivo do <strong>Mercado Livre</strong> e o feedback recebido, que direcionou meus estudos para linguagens como <strong>Golang, microserviços, APIs RESTful, bancos de dados e integração hardware-software</strong>. Agradeço também à comunidade de código aberto, aos criadores de conteúdo técnico no YouTube e aos engenheiros que compartilham conhecimento em fóruns como Stack Overflow e GitHub Issues.
</p>

<p class="texto-formatado">
    Por fim, dedico este projeto a todos que estão em transição de carreira, provando que é possível migrar de áreas técnicas (como eletrônica) para o desenvolvimento de software, aproveitando a base de conhecimento prévio como diferencial competitivo.
</p>

---

