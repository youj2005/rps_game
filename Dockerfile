FROM ubuntu
RUN apt-get update && apt-get install -y g++ \
    vim \
    nano \
    make \
    libssl-dev \
    --no-install-recommends

# quiets some warnings
ENV GIT_PYTHON_REFRESH=quiet

EXPOSE 8080
EXPOSE 8090

# Clean up unnecessary files
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*


