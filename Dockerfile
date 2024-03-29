FROM alpine:3.17.2 AS deps

ENV PYTHONUNBUFFERED=1
RUN \
    apk add --no-cache cmake gcc g++ make samurai python3 perl linux-headers && \
    ln -sf python3 /usr/bin/python

RUN python3 -m ensurepip
RUN pip3 install --no-cache pip setuptools conan==1.59.0

FROM deps AS server

COPY . /build
WORKDIR /build

RUN \
    --mount=type=cache,target=/root/.conan/data \
    --mount=type=cache,target=/build/build \
    cmake -S. -Bbuild -G Ninja -DCMAKE_BUILD_TYPE:STRING=Release -DCONAN:BOOL=ON && \
    cmake --build   build && \
    cmake --install build

ENV XDG_CONFIG_HOME=/app_config
RUN mkdir -p $XDG_CONFIG_HOME

CMD ["pss-server"]
