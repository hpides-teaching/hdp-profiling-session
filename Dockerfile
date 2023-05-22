FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
    software-properties-common \
    libssl-dev openssh-server rsyslog \
    less nano neovim vim emacs htop tree git rsync \
    gcc-11 g++-11 gdb cmake libc6-dbg \
    linux-tools-common linux-tools-generic

# perf. This is a bit hacky because of the host and docker kernel mismatch.
# If this fails when building the docker, check which path you have in the container.
RUN ln -sf /usr/lib/linux-tools/5.15.0-72-generic/perf /usr/bin/perf && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 11 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 11 && \
    useradd --create-home --shell /bin/bash hdp && echo "hdp:123456" | chpasswd

ENTRYPOINT service ssh restart && tail -f /dev/null
