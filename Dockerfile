# Get the base Ubuntu image from Docker Hub
FROM ubuntu:focal-20201008

# Required to get around TZ data asking for geographic data user input
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow

# Update apps on the base image and install GCC, G++ and Make
RUN apt-get -y update && apt-get install build-essential -y
# Update apps on the base image and install CMake
RUN apt-get -y update && apt-get install cmake -y

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY /src /usr/gerald.chau/src/
COPY /CMakeLists.txt /usr/gerald.chau/CMakeLists.txt

# Specify the working directory
WORKDIR /usr/gerald.chau

# Use Cmake to produce project
RUN cmake -Bbuild_64 -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++
 
# Specify the working directory
WORKDIR /usr/gerald.chau/build_64

# Compile project
RUN make

# Run tests - takes up to 20 seconds before console message appears!
# Comment out, and attach to container if you want to see layout.
CMD ["/usr/gerald.chau/build_64/test"]