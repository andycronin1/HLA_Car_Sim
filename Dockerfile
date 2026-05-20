# ---- Build Stage ----
FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    libssl-dev \
    zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy source
COPY CMakeLists.txt .
COPY src/ src/
COPY include/ include/
COPY external/ external/

# Build
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    cmake --build . --config Release

# ---- Run Stage ----
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libssl3 \
    zlib1g \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy built binary and map client
COPY --from=builder /app/bin/HLA_Personal_Project ./bin/HLA_Personal_Project
COPY data/ data/

EXPOSE 8080

# Run with stdin enabled for keyboard commands
CMD ["./bin/HLA_Personal_Project"]
