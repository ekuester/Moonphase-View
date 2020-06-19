#! /bin/sh -e
test -n "$srcdir" || srcdir=`dirname "$0"`
test -n "$srcdir" || srcdir=.

echo "Regenerating autotools files"
autoreconf --force --install --verbose --warnings=all "$srcdir"
echo "Setting up Intltool"
intltoolize --copy --force --automake || exit 1
test -n "$NOCONFIGURE" || "$srcdir/configure" "$@"

