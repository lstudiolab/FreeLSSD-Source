#!/usr/bin/env bash
# One-shot repository migration from XIUL to XUL.
# Third-party Mach, Linux and FreeBSD source trees are deliberately left untouched.
# Workflow files are updated separately through repository administration.
set -euo pipefail

if [[ -d kernel/xiul && ! -e kernel/xul ]]; then
  git mv kernel/xiul kernel/xul
fi

if [[ -d kernel/xul/include/xiul && ! -e kernel/xul/include/xul ]]; then
  git mv kernel/xul/include/xiul kernel/xul/include/xul
fi

if [[ -f kernel/xul/xiul.toml && ! -e kernel/xul/xul.toml ]]; then
  git mv kernel/xul/xiul.toml kernel/xul/xul.toml
fi

# Rename XUL-owned identifiers, include paths, diagnostics, build variables and
# documentation. Preserve the upstream Mach, Linux and FreeBSD source content.
# CI workflows are excluded because GitHub requires a separate workflow scope.
git grep -Ilz -e 'XIUL' -e 'xiul' -- . \
  ':!kernel/xul/mach/**' \
  ':!kernel/xul/linux/**' \
  ':!kernel/xul/freebsd/**' \
  ':!.github/workflows/**' \
  ':!scripts/rebrand-xul.sh' |
  xargs -0 -r sed -i -e 's/XIUL/XUL/g' -e 's/xiul/xul/g'

echo "XUL source-tree migration applied."
