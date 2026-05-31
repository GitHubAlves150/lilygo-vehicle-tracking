FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Instala dependências básicas
RUN apt-get update && apt-get install -y \
    git \ 
    wget \ 
    curl \
    flex \
    bison \
    gperf \
    ccache \
    dfu-util \
    cmake \
    libffi-dev \
    libssl-dev \
    python3 \
    python3-pip \
    python3-venv \
    usbutils \
    udev \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

# Instala ESP-IDF
ENV IDF_PATH=/opt/esp/idf
ENV IDF_TOOLS_PATH=/opt/esp/tools

RUN mkdir -p /opt/esp && \
    cd /opt/esp && \
    git clone --recursive --depth 1 --branch release/v5.3 https://github.com/espressif/esp-idf.git

ENV PATH="/opt/esp/tools:${PATH}"

WORKDIR /workspace

CMD ["tail", "-f", "/dev/null"]