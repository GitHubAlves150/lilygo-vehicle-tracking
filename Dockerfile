FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    git \
    wget \
    curl \
    python3 \
    python3-pip \
    python3-venv \
    flex \
    bison \
    gperf \
    ccache \
    dfu-util \
    cmake \
    libffi-dev \
    libssl-dev \
    nano -y \
    usbutils \
    udev \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*
RUN python3 -m pip install --upgrade pip setuptools wheel
RUN python3 -m pip install platformio
RUN echo '. /opt/esp/idf/export.sh' >> ~/.bashrc

WORKDIR /workspace

CMD ["tail", "-f", "/dev/null"]
