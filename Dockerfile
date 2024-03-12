FROM ubuntu:22.04

RUN DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get -y install build-essential \
    valgrind \
    libgtest-dev \
    cmake \
    && cd /usr/src/gtest && cmake CMakeLists.txt && make && cp lib/*.a /usr/lib \
    && cd /usr/local/lib && mkdir gtest/ \
    && cp /usr/lib/*.a /usr/local/lib/gtest/ 


COPY . /project

WORKDIR "/project"

ENTRYPOINT ["/bin/bash"]

