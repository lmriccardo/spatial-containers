FROM ubuntu:latest

ARG DEBIAN_FRONTEND=noninteractive

# Install required dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    libssl-dev \
    gdb \
    gdbserver \
    libtool \
    automake \
    autoconf \
    locales \
    && rm -rf /var/lib/apt/lists/*

# Wide-char compatibility
RUN locale-gen en_US.UTF-8
RUN update-locale LANG=en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LC_ALL=en_US.UTF-8