# Modified by L,Studio. Copyright (c) 2026 L,Studio.
#!/bin/sh
set -eu

root=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
cd "$root"

fail() {
    printf 'XIUL source check: %s\n' "$1" >&2
    exit 1
}

[ -f .gitmodules ] || fail "missing .gitmodules"

for path in vendor/xnu vendor/linux vendor/freebsd-src; do
    [ -f "$path/README" ] || [ -f "$path/README.md" ] || [ -f "$path/Makefile" ] || \
        fail "$path is not initialized (run: git submodule update --init --recursive)"
done

[ -d vendor/xnu/osfmk ] || fail "XNU Mach source directory is missing"
[ -d vendor/linux/drivers ] || fail "Linux driver source directory is missing"
[ -d vendor/freebsd-src/sys ] || fail "FreeBSD kernel source directory is missing"

printf '%s\n' "XIUL upstream source trees are present."
printf '%s\n' "This check validates source availability only; it does not claim that XIUL builds or boots."
