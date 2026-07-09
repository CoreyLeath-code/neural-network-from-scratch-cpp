FROM ubuntu:24.04 AS build

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        ca-certificates \
        cmake \
        g++ \
        libgtest-dev \
        ninja-build \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . .

RUN cmake -S src -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_TESTING=OFF \
    && cmake --build build --parallel \
    && cmake --install build --prefix /opt/neuralnet

FROM ubuntu:24.04 AS runtime

RUN useradd --create-home --uid 10001 appuser
COPY --from=build /opt/neuralnet/bin/nn_main /usr/local/bin/nn_main

USER 10001
ENTRYPOINT ["/usr/local/bin/nn_main"]
