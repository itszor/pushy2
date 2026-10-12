#!/bin/sh
# Build Pushy II from your own copy of the game:
#     ./build.sh PUSHY2      (the RISC OS zip of !PushyII; or a directory holding !PushyII)
# then: cd recomp && ./pushy2
# The steps are recipe.json's "build", run by SuperSpider's tools/weave.py.
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
SUPERSPIDER="${SUPERSPIDER:-$HOME/stuff/superspider}"
[ $# -gt 0 ] || { echo "usage: $0 PUSHY2.zip | DIRECTORY" >&2; exit 2; }
(cd "$SUPERSPIDER" && dune build)
WEAVE_RECIPES="$HERE" exec python3 -I "$SUPERSPIDER/tools/weave.py" build "$@"
