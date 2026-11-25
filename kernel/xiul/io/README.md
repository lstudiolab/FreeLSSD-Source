<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# XIUL I/O Streams

The first executable stream primitive is `stream.c`: a bounded, caller-backed, nonblocking single-producer/single-consumer byte ring.

Contract:
- no allocation in read/write paths;
- caller owns the storage and stream lifetime;
- producer publishes bytes with release ordering and consumer observes them with acquire ordering;
- partial transfers are allowed;
- full writes and empty open reads return `XIUL_E_BUSY`;
- close prevents future writes; buffered bytes remain readable and an empty closed stream returns a successful zero-byte read;
- exactly one producer and one consumer may operate concurrently. Multi-producer/multi-consumer access requires external serialization.

The stream is a XIUL-owned data path primitive. It is not XNU I/O Kit and does not itself provide device discovery, interrupts, DMA or Linux-driver compatibility. Those remain separate adapters.
