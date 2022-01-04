FROM centos
RUN dnf -y install https://dl.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm
RUN dnf update -y
RUN dnf -y install htop iftop iotop
RUN dnf -y install vim tree cscope
RUN dnf -y install gdb
RUN dnf -y install clang cmake
RUN dnf -y group install "Development Tools"
RUN dnf -y install bc
WORKDIR /tmp
COPY install_protobuf.sh /tmp/install_protobuf.sh
RUN bash /tmp/install_protobuf.sh
RUN chmod -R a+rx /usr/include/google/
RUN echo "root:Docker!" | chpasswd
RUN useradd -ms /bin/bash development
USER development
WORKDIR /home/development/ss-training
COPY .profile /home/development/.profile
