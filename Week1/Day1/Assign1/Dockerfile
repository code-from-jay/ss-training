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
WORKDIR /home/development
COPY .profile /home/development/.profile
