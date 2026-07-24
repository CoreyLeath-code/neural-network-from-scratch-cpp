FROM ubuntu:26.04 AS build

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
      ca-certificates cmake g++ ninja-build \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /workspace
COPY . .
RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF \
    && cmake --build build --parallel \
    && cmake --install build --prefix /opt/neuralnet

FROM ubuntu:26.04 AS runtime
RUN useradd --create-home --uid 10001 appuser
COPY --from=build /opt/neuralnet/bin/neural_network /usr/local/bin/neural_network
USER 10001
ENTRYPOINT ["/usr/local/bin/neural_network"]
