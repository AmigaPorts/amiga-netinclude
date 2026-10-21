#!/bin/sh
# Generate the proposed vector bindings from their SFD.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
SFDC=${SFDC:-sfdc}
CHECK=0
case "${1-}" in
    '') ;;
    --check) CHECK=1 ;;
    *) echo 'usage: gen-ifindex.sh [--check]' >&2; exit 2 ;;
esac
WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT HUP INT TERM
emit() {
    target=$1 mode=$2 relative=$3
    mkdir -p "$WORK/$(dirname "$relative")"
    # Omit attribution and rights boilerplate injected by the generator.
    "$SFDC" --quiet --target="$target" --mode="$mode" \
        "$ROOT/sfd/bsdsocket_ifindex_lib.sfd" > "$WORK/raw"
    sed -e '/^\*\*   Copyright /d' -e '/^\*\*       All Rights Reserved$/d' "$WORK/raw" > "$WORK/$relative"
    if [ "$CHECK" -eq 1 ]; then
        diff -u "$ROOT/$relative" "$WORK/$relative"
    else
        mkdir -p "$ROOT/$(dirname "$relative")"
        cp "$WORK/$relative" "$ROOT/$relative"
    fi
}
emit m68k-amigaos macros include/inline/bsdsocket_ifindex.h
emit m68kvbcc-unknown-amigaos macros include/inline/bsdsocket_ifindex_protos.h
emit m68k-amigaos pragmas include/pragmas/bsdsocket_ifindex_pragmas.h
emit m68k-amigaos lvo include/lvo/bsdsocket_ifindex_lib.i
