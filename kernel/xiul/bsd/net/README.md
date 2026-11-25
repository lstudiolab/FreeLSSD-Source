<!--
This file has been modified by L,Studio.
Copyright (c) 2026 L,Studio. All rights reserved.
-->

# BSD Networking Adapter

XIUL-owned network contracts now provide:
- a bounded network-interface registry;
- packet receive handoff to an optional stack input callback;
- a protocol-handler registry with registration, duplicate detection, dispatch and unregister;
- transmit dispatch to a registered device driver;
- packet length checks against the interface MTU on transmit.

This is an integration boundary, not a port of FreeBSD's networking stack. FreeBSD protocol and interface sources under `freebsd/sys/net` and `freebsd/sys/netinet` still depend on mbufs, epochs, VNET, netisr, taskqueues, UMA, sysctl and FreeBSD locking. Those dependencies must be adapted before those sources can be linked.

The current registry is fixed-capacity and externally serialized. It does not yet provide mbuf allocation/reference counting, socket buffers, protocol implementations, routing, ARP/ND, TCP/UDP/IP, packet queues, interrupt/NAPI processing or network namespace support.

Run `make -C kernel/xiul test` for interface and protocol dispatch tests.
