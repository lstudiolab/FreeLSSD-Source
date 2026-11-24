# FreeLSSD

FreeLSSD is the operating-system project. Its kernel is **XIUL**.

## XIUL source layout

The upstream source repositories are mounted directly inside `kernel/xiul/` as Git submodules:

- `kernel/xiul/xnu/` — XNU source, including its Mach implementation.
- `kernel/xiul/linux/` — Linux kernel source, used as the source for selected driver ports.
- `kernel/xiul/freebsd/` — FreeBSD source, used for selected networking and filesystem components.

These are real upstream source trees when checked out recursively. The parent repository records each upstream commit as a Git submodule pointer, preserving upstream history and licensing rather than copying millions of files into FreeLSSD's own Git history.

## Checkout

```sh
git clone --recurse-submodules https://github.com/lstudiolab/FreeLSSD-Source.git
cd FreeLSSD-Source
```

For an existing clone:

```sh
git submodule update --init --recursive
```

## Status

This layout places the three upstream source trees under the XIUL directory. It does not mean their kernels have already been merged into one bootable kernel. XIUL's integration adapters, boot path, target platform and subsystem implementations remain ongoing work.

## Licensing

Third-party source retains its original license and notices. Review the license of every file and component before importing, linking, modifying or redistributing it.
