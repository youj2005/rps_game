FROM ubuntu:latest

RUN apt-get update && apt-get install -y g++ \
    vim \
    gdb \
    nano \
    make \
    libssl-dev \
    libcurl4-openssl-dev \
    nlohmann-json3-dev \
    --no-install-recommends

# quiets some warnings
ENV GIT_PYTHON_REFRESH=quiet

EXPOSE 8080
EXPOSE 8090

# Clean up unnecessary files
# RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*


