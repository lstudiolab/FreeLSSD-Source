# BSD Networking Adapter

XIUL-owned network contracts now provide a bounded network-interface registry, a packet receive handoff to the selected stack input callback, and transmit dispatch to a registered device driver.

The current code is a narrow integration boundary, not a port of FreeBSD's networking stack. Selected FreeBSD protocol and interface sources under `freebsd/sys/net` and `freebsd/sys/netinet` still depend on mbufs, epochs, VNET, netisr, taskqueues, UMA, sysctl and FreeBSD locking. Those dependencies must be adapted before those sources can be linked.

Run `make -C kernel/xiul test` for registry and packet handoff tests.
