# Docker Command line
#   -- docker image build -- https://docs.docker.com/reference/cli/docker/image/build/
#   Create image => docker build -t embtl-dev-img .
#   -- docker container create -- https://docs.docker.com/reference/cli/docker/container/create/
#   Create container (cli) => docker create -i -t --entrypoint="bin/bash" --name embtl-dev-cli embtl-dev-img
#   -- docker container run -- https://docs.docker.com/reference/cli/docker/container/run/
#   Run Container with volume and cli => docker run -it -v <host_src_path>:/home/embtl --name embtl-vol-test --entrypoint="/bin/bash" embtl-dev-img
#   Run Container with volume and clean up when done => docker run --rm -v <host_src_path>:/home/embtl --name embtl-vol-test --entrypoint="/bin/bash" embtl-dev-img
#
#   docker start -i embtl-dev-cli
#
FROM ubuntu:latest

LABEL author="Robert Morley"
LABEL description="Used for development of the embedded template library."
ARG DEV_DIR=/home/embtl
# Make directory for install scripts
RUN mkdir tmp_scripts && mkdir $DEV_DIR

# Copy project file.
COPY ./ $DEV_DIR
# Copy install scripts
COPY ./tools/docker/linux/arm_gnu_toolchain.sh /tmp_scripts/arm_gnu_toolchain.sh
COPY ./tools/docker/linux/cmake_install.sh /tmp_scripts/cmake_install.sh

# remove build directories
RUN rm -r $DEV_DIR/cmake-*

# Preinstall before running script; curl, xz-utils, dos2unix, and git.
RUN apt update && apt-get install -y curl xz-utils dos2unix git

# Install ARM GNU toolchain and CMake
RUN dos2unix /tmp_scripts/arm_gnu_toolchain.sh \
    && dos2unix /tmp_scripts/cmake_install.sh \
    && chmod u+x /tmp_scripts/arm_gnu_toolchain.sh \
    && chmod u+x /tmp_scripts/cmake_install.sh \
    && ./tmp_scripts/arm_gnu_toolchain.sh \
    && ./tmp_scripts/cmake_install.sh

# Make CMake and ARM GNU Toolchain global.
ENV PATH="${PATH}:/opt/cmake/bin:/opt/gcc-arm-none-eabi/bin"
# Install Python, Clang, Ninja and GNU GCC
RUN apt-get install -y  \
    ssh python3  \
    build-essential \
    ninja-build \
    clang \
    && apt-get clean
WORKDIR $DEV_DIR
# Command that will be invoked when the container starts
# Use cmake to configure and build project. Use ctest to run unit test.
RUN cmake --preset unit-test-config
RUN cmake --build --preset unit-test-build
RUN ctest --preset unit-test

ENTRYPOINT ["bin/bash"]
