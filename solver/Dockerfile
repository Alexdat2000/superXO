# Use an official GCC image for compilation
FROM ubuntu:latest

# Install Boost
RUN apt update && apt install -y build-essential libboost-all-dev

# Copy the source code into the container
COPY . /app/

# Change the working directory to /app
WORKDIR /app

# Compile the C++ program
RUN g++ -O3 -o run main.cpp -lboost_system -lboost_thread -DCROW_USE_BOOST

# Run the compiled binary
CMD ["./run"]
