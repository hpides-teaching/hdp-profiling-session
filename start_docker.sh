#!/bin/bash

set -e

if [ "$#" -ne 1 ]; then
    echo "Need to specify /path/to/users.txt"
    exit 1
fi

USER_FILE=$1

if [ ! -f $USER_FILE ]; then
    echo "File $USER_FILE does not exist!"
    exit 1
fi

if [ ! -s $USER_FILE ]; then
    echo "File $USER_FILE is empty!"
    exit 1
fi

while read -r user port; do
    echo "Adding user: $user to port: $port"
    docker_name="hdp-profiling-$user"
    docker run -d -p $port:22 \
            --privileged \
            --restart always \
            --name "$docker_name" \
            $DOCKER
done < $USER_FILE



