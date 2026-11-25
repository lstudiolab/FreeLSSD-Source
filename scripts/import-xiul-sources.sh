undefined# Modified by L,Studio. Copyright (c) 2026 L,Studio.
#!/usr/bin/env bash
set -euo pipefail

ROOT="$(git rev-parse --show-toplevel)"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

import_tree() {
  local name="$1" url="$2" branch="$3"
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

import_mach() {
  local url="https://github.com/lstudiolab/darwin-xnu.git"
  local branch="main" destination="$ROOT/kernel/xiul/mach"
  echo "::group::Importing Mach (XNU osfmk only)"
  git clone --depth 1 --single-branch --branch "$branch" "$url" "$TMP/xnu"
  local source_commit
  source_commit="$(git -C "$TMP/xnu" rev-parse HEAD)"
  test -d "$TMP/xnu/osfmk" || { echo "Expected osfmk/ was not found." >&2; exit 1; }
  mkdir -p "$destination"
  find "$destination" -mindepth 1 -maxdepth 1 -exec rm -rf {} +
  tar -C "$TMP/xnu/osfmk" -cf - . | tar -C "$destination" -xf -
  printf 'mach-osfmk %s %s\n' "$url" "$source_commit" >> "$ROOT/kernel/xiul/UPSTREAM-SOURCES.txt"
  echo "Mach source commit: $source_commit"
  echo "::endgroup::"
}

mkdir -p "$ROOT/kernel/xiul"
: > "$ROOT/kernel/xiul/UPSTREAM-SOURCES.txt"
import_mach
import_tree linux https://github.com/lstudiolab/linux.git master
import_tree freebsd https://github.com/lstudiolab/freebsd-src.git main

# Keep imported upstream trees as ordinary tracked source directories.
git add -A -- kernel/xiul
if git ls-files --error-unmatch .gitmodules >/dev/null 2>&1; then
  rm -f "$ROOT/.gitmodules"
  git add -u -- .gitmodules
fi

if git diff --cached --quiet; then
  echo "XIUL source trees are already up to date."
  exit 0
fi

git -c user.name="github-actions[bot]" \
    -c user.email="41898282+github-actions[bot]@users.noreply.github.com" \
    commit -m "Import Mach osfmk, Linux and FreeBSD sources into XIUL"
git fetch origin main
git config user.name "github-actions[bot]"
git config user.email "41898282+users.noreply.github.com"
git rebase origin/main
git push origin "HEAD:main"
