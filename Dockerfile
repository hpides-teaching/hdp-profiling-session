FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
    software-properties-common \
    libssl-dev openssh-server rsyslog \
    less nano neovim vim emacs htop tree git rsync \
    gcc-11 g++-11 gdb cmake libc6-dbg \
    linux-tools-common linux-tools-generic

# perf. This needs to be tied to the host we are running on.
# Replace "5.15.0-58-generic" with the output of `uname -r`.
RUN apt update && apt install -y linux-tools-5.15.0-58-generic

# VTune (https://www.intel.com/content/www/us/en/docs/vtune-profiler/installation-guide/2023-0/package-managers.html)
RUN wget https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB && \
    apt-key add GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB && \
    echo "deb https://apt.repos.intel.com/oneapi all main" > /etc/apt/sources.list.d/oneAPI.list && \
    add-apt-repository "deb https://apt.repos.intel.com/oneapi all main" && \
    apt update && \
    apt install -y intel-oneapi-vtune

RUN useradd --create-home --shell /bin/bash hdp && \
    echo "hdp:123456" | chpasswd

ENTRYPOINT service ssh restart && tail -f /dev/null
