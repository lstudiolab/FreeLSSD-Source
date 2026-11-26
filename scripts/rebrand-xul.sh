#!/usr/bin/env bash
# One-shot repository migration from XIUL to XUL.
# Third-party Mach, Linux and FreeBSD source trees are deliberately left untouched.
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

rename_if_present() {
  local old_path="$1"
  local new_path="$2"
  if [[ -e "$old_path" && ! -e "$new_path" ]]; then
    git mv "$old_path" "$new_path"
  fi
}

rename_if_present .github/workflows/xiul.yml .github/workflows/xul.yml
rename_if_present .github/workflows/xiul-source-audit.yml .github/workflows/xul-source-audit.yml
rename_if_present .github/workflows/import-xiul-sources.yml .github/workflows/import-xul-sources.yml
rename_if_present scripts/import-xiul-sources.sh scripts/import-xul-sources.sh

# Rename XUL-owned identifiers, include paths, diagnostics, build variables and
# documentation. Keep upstream source trees byte-for-byte unchanged.
git grep -Ilz -e 'XIUL' -e 'xiul' -- . \
  ':!kernel/xul/mach/**' \
  ':!kernel/xul/linux/**' \
  ':!kernel/xul/freebsd/**' \
  ':!scripts/rebrand-xul.sh' |
  xargs -0 -r sed -i -e 's/XIUL/XUL/g' -e 's/xiul/xul/g'

echo "XUL source-tree migration applied."
