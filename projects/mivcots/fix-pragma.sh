#!/bin/sh

files="$(grep -Rl '#pragma once' ./* | grep -v "$0")"

tmpfile="$(mktemp)"
cleanup () { rm -f $tmpfile; }
trap cleanup EXIT

# NOTE this will not work if there are spaces in any of the file names
for file in $files; do
    f="${file##*/}"
    macro="$(echo "$f" | sed 's/\([a-z]\)\([A-Z]\)/\1_\2/g;s/\./_/g' | tr '[[:lower:]]' '[[:upper:]]')"
    cp "$file" "$tmpfile"
    echo $macro
    sed 's/#pragma once/#ifndef '"$macro"'\n#define '"$macro"'/;$a #endif //'"$macro" $tmpfile > $file
done

