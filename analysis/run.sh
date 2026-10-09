#!/bin/sh
# Code/data analysis of Pushy II's !RunImage with SuperSpider
# (~/stuff/superspider): pushy2.json, which recomp/'s "make regen" translates
# to C with superspider gen.
# pushy2.roots adds entry points to the analysis (recomp/: "make roots" merges
# the gap entries a --trace-gaps run logged into it).
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
SUPERSPIDER="${SUPERSPIDER:-$HOME/stuff/superspider}"
IMG="$HERE/../!PushyII/!RunImage"
(cd "$SUPERSPIDER" && dune build)
"$SUPERSPIDER/_build/default/bin/main.exe" analyse "$IMG" 8000 --full --roots "$HERE/pushy2.roots" -o "$HERE/pushy2.json"
