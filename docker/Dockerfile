# Linux toolbox for litcode. Main reason: on Linux, ASan includes LeakSanitizer and
# Valgrind works — real leak reports with stack traces, which macOS can't give you.
# C is compiled with gcc here (Debian's gcc ships the ASan/LSan/UBSan runtimes; its clang doesn't).
#   lc box build              build this image
#   lc box test 103 c         run any lc command inside the container
#   lc box valgrind 103       C tests under valgrind
#   lc box shell              bash inside the container
FROM rust:1.90-bookworm

RUN apt-get update && apt-get install -y --no-install-recommends \
        gcc valgrind gdb make postgresql-client unzip curl ca-certificates less \
    && rm -rf /var/lib/apt/lists/*

ENV BUN_INSTALL=/usr/local
RUN curl -fsSL https://bun.sh/install | bash -s "bun-v1.3.13"

# Keep Linux build artifacts away from the macOS ones in the mounted repo.
ENV CARGO_TARGET_DIR=/work/.box/target \
    CC=gcc \
    ASAN_OPTIONS=detect_leaks=1

WORKDIR /work
ENTRYPOINT ["/work/lc"]
CMD ["help"]
