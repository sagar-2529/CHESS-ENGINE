FROM debian:bookworm-slim AS build

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
       build-essential \
       ca-certificates \
       cmake \
       git \
       libboost-system-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY CMakeLists.txt ./
COPY api ./api
COPY board ./board
COPY enums ./enums
COPY game ./game
COPY pieces ./pieces
COPY rules ./rules
COPY user ./user
COPY utlis ./utlis

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel

FROM debian:bookworm-slim

RUN apt-get update \
    && apt-get install -y --no-install-recommends libboost-system1.74.0 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /app/build/chess_server /usr/local/bin/chess_server

ENV PORT=10000
EXPOSE 10000
CMD ["chess_server"]
