#!/bin/sh
# Code/data classification for Pushy II's !RunImage with SuperSpider
# (~/stuff/superspider). Produces what recomp/gen_c.py reads:
#   pushy2.json(.pickle)  via SuperSpider's phase 6 adapter
#   out/functions.tsv
# spider.py / export_ghidra.py / gen_asm.py are copies of the Lemmings
# project's (gen_c.py imports them; the pickle refers to spider.Spider).
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
SUPERSPIDER="${SUPERSPIDER:-$HOME/stuff/superspider}"
IMG="$HERE/../!PushyII/!RunImage"
(cd "$SUPERSPIDER" && dune build)
"$SUPERSPIDER/_build/default/bin/main.exe" s1b "$IMG" 8000 -o "$HERE/ss.json"
python3 "$SUPERSPIDER/phase6/ss_to_spider.py" "$HERE/ss.json" "$IMG" 8000 "$HERE/pushy2.json" "$HERE/out" "$HERE"
