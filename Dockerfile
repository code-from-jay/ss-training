FROM centos
RUN dnf -y install https://dl.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm
RUN dnf update -y
RUN dnf -y install htop iftop iotop
RUN dnf -y install vim tree cscope
RUN dnf -y install gdb
RUN dnf -y install clang
RUN dnf -y group install "Development Tools"
RUN useradd -ms /bin/bash development
USER development
WORKDIR /home/development/ss-training
COPY .profile /home/development/.profile
WORKDIR /tmp
RUN mkdir -p /home/development/valgrind
RUN curl -o valgrind-3.18.1.tar.bz2 https://sourceware.org/pub/valgrind/valgrind-3.18.1.tar.bz2
RUN bzip2 -d valgrind-3.18.1.tar.bz2
RUN tar xvf valgrind-3.18.1.tar
WORKDIR /tmp/valgrind-3.18.1
RUN ./configure --prefix=/home/development/valgrind
RUN make
RUN make install
WORKDIR /home/development/ss-training