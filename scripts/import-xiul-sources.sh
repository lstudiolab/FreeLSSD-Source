#!/usr/bin/env bash
set -euo pipefail

ROOT="$(git rev-parse --show-toplevel)"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

import_tree() {
  local name="$1"
  local url="$2"
  local branch="$3"
  local destination="$ROOT/kernel/xiul/$name"

  echo "::group::Importing $name source"
  git clone --depth 1 --single-branch --branch "$branch" "$url" "$TMP/$name"
  local source_commit
  source_commit="$(git -C "$TMP/$name" rev-parse HEAD)"
  echo "$name source commit: $source_commit"

  mkdir -p "$destination"
  find "$destination" -mindepth 1 -maxdepth 1 -exec rm -rf {} +
  tar -C "$TMP/$name" --exclude='./.git' -cf - . | tar -C "$destination" -xf -

  printf '%s %s %s\n' "$name" "$url" "$source_commit" >> "$ROOT/kernel/xiul/UPSTREAM-SOURCES.txt"
  echo "::endgroup::"
}

mkdir -p "$ROOT/kernel/xiul"
: > "$ROOT/kernel/xiul/UPSTREAM-SOURCES.txt"

import_tree xnu https://github.com/lstudiolab/darwin-xnu.git main
import_tree linux https://github.com/lstudiolab/linux.git master
import_tree freebsd https://github.com/lstudiolab/freebsd-src.git main

# The source trees are now ordinary directories tracked by FreeLSSD, not submodules.
rm -f "$ROOT/.gitmodules"
git rm --cached -f kernel/xiul/xnu kernel/xiul/linux kernel/xiul/freebsd 2>/dev/null || true
git add -A kernel/xiul .gitmodules

if git diff --cached --quiet; then
  echo "XIUL source trees are already up to date."
  exit 0
fi

git -c user.name="github-actions[bot]" \
    -c user.email="41898282+github-actions[bot]@users.noreply.github.com" \
    commit -m "Import full XNU, Linux and FreeBSD source into XIUL"
git push origin "HEAD:main"
