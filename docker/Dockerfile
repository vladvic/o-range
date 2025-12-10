FROM ubuntu:24.04

ARG USER_ID=1000
ARG GROUP_ID=1000

RUN apt update && apt install -y \
    gcc-14 \
    g++-14 \
    gdb \
    make \
    cmake \
    git \
    ninja-build \
    pkg-config \
    autoconf \
    automake \
    libtool \
    libssl-dev \
    libpq-dev \
    libpcre3-dev \
    libc-ares-dev \
    libdb++-dev \
    libsrtp2-dev \
    libgeoip-dev \
    libpopt-dev \
    libcppunit-dev \
    libncurses-dev \
    libmagicenum-dev \
    libbenchmark-dev \
    sox \
    xxd \
    wget \
    sudo \
    libc6-dev \
    dpkg-dev \
    && rm -rf /var/lib/apt/lists/*
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 100 \
    && update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-14 100

RUN userdel -r ubuntu && \
    groupadd -g ${GROUP_ID} developer 2>/dev/null || true && \
    useradd -m -u ${USER_ID} -g ${GROUP_ID} -s /bin/bash developer 2>/dev/null || true && \
    echo "developer ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

RUN cd /tmp && \
    wget -q https://archives.boost.io/release/1.89.0/source/boost_1_89_0.tar.bz2 && \
    tar -xf boost_1_89_0.tar.bz2 && \
    cd boost_1_89_0 && \
    ./bootstrap.sh --prefix=/usr/local --with-libraries=system,thread,interprocess,container,exception,date_time && \
    ./b2 -j$(nproc) install && \
    cd .. && \
    rm -rf boost_1_89_0.tar.bz2 boost_1_89_0

RUN cd /tmp && \
    git clone https://github.com/chriskohlhoff/asio.git && \
    cd asio && \
    git checkout asio-1-31-0 && \
    cd asio && \
    ./autogen.sh && \
    ./configure --prefix=/usr/local && \
    make -j$(nproc) && \
    make install && \
    cd ../.. && \
    rm -rf asio

RUN cd /tmp && \
    git clone https://github.com/resiprocate/resiprocate.git && \
    cd resiprocate && \
    git checkout resiprocate-1.13.2 && \
    mkdir cmake_build && \
    cd cmake_build && \
    cmake .. -DBUILD_TFM=OFF -DBUILD_QPID_PROTON=OFF && \
    make -j$(nproc) && \
    make install && \
    cd .. && \
    rm -rf resiprocate

RUN cd /tmp && \
    git clone https://github.com/SIPp/sipp.git && \
    cd sipp && \
    git checkout v3.7.5 && \
    mkdir cmake_build && cd cmake_build && \
    cmake .. -DUSE_SSL=1 && \
    make && \
    make install && \
    cd .. && \
    rm -rf sipp

USER developer
WORKDIR /home/developer

RUN mkdir -p /home/developer/work

WORKDIR /home/developer/work

CMD ["/bin/bash"]
