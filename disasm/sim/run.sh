#!/bin/sh
# usage: ./run.sh [HEX] [seconds] [script] [ENV=val ...]
# runs the headless simulator with node if present, else macOS JavaScriptCore
cd "$(dirname "$0")"
HEX=${1:-../../YA4000S.HEX}; SECS=${2:-3}; SCRIPT=${3:-}; shift 3 2>/dev/null
if command -v node >/dev/null; then env "$@" node run_node.js "$HEX" "$SECS" "$SCRIPT"
else /System/Library/Frameworks/JavaScriptCore.framework/Versions/Current/Helpers/jsc run_node.js -- "$HEX" "$SECS" "$SCRIPT" "$*"; fi
