#!/bin/bash

DOCKER="hdp-profiling"

docker stop $(docker ps -a -f name="^$DOCKER-*" -q)
docker rm $(docker ps -a -f name="^$DOCKER-*" -q)
