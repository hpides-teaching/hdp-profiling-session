### HDP Profiling Session

Small code example to show how to use `perf` in our HDP lecture.

To build the Docker container, run:
```shell
# Build
$ DOCKER_BUILDKIT=1 docker build -t hdp-profiling .
```

To start the container once per student, run:
```shell
$ ./start_docker /path/to/users.txt
```

`users.txt` should contain one student per line, consisting of their name and a port.
You can use the task's users.txt and modify it.

This will open the ports from the file on the host for the students to SSH into with user `hdp` and password `123456`.
The code for the session can be found in `/hdp-profiling-session` inside the container.
