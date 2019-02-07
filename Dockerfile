FROM sshambir/compiler-builder:0.0.1 AS builder

COPY src /app/src
RUN mkdir -p /app/build
WORKDIR /app/build
RUN cmake /app/src && cmake --build .

FROM sshambir/compiler-base:0.0.1
COPY --from=builder /app/build/pythonishc /usr/bin/pythonishc
COPY src/bin/pythonish /usr/bin/pythonish

ENTRYPOINT [ "/usr/bin/pythonish" ]
