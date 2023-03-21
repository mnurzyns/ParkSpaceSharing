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
RUN cmake -S. -Bbuild -G Ninja -DCMAKE_BUILD_TYPE:STRING=Release -DCONAN:BOOL=ON
RUN cmake --build build

CMD ["/build/build/bin/server"]
