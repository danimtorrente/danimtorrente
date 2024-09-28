#!/bin/bash
pwd=$(pwd)
# Get your host's UID and GID
export HOST_UID=$(id -u)
export HOST_GID=$(id -g)

if [ "$1" == "build" ]; then
    # Build the Docker image
    docker image build --build-arg UID=$HOST_UID --build-arg GID=$HOST_GID --no-cache -t ac2:1.0 .

elif [ "$1" == "init" ]; then
    # Run the Docker container
    xhost +local:docker
    docker run --net=host --env="DISPLAY" --volume="$HOME/.Xauthority:/home/ac2/.Xauthority:rw" --volume="$pwd/labs:/home/ubuntu/labs"  -it ac2:1.0
    xhost -local:docker

elif [ "$1" == "rm" ]; then
    # Remove the Docker container
    docker image rm -f ac2:1.0
else
    echo "Invalid command. Use './script.sh build' to build or './script.sh init' to initialize."
fi