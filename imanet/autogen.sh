#!/bin/sh

echo "Generating ChangeLog ..."
git log > ChangeLog

aclocal \
&& autoheader \
&& automake --add-missing \
&& autoconf
