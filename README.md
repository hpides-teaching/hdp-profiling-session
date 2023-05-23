### HDP Profiling Session

Small code example to show how to use `perf` in our HDP lecture.

To build and start the Docker container, run:
```shell
DOCKER_BUILDKIT=1 docker build -t hdp-profiling .
docker run -d \
    -v $(pwd):/hdp-profiling-session \
    --privileged \
    -p 8000:22 \
    --restart always \
    --name "hdp-profiling" \
    hdp-profiling
```

This will open port 8000 on the host for SSH.
