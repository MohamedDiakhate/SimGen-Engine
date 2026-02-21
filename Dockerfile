
FROM ubuntu:22.04


ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/* 
WORKDIR /app


COPY . .


RUN mkdir build && cd build && \
    cmake .. && \
    make -j4


CMD ["./build/simgen"]            