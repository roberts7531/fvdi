#!/bin/bash -eux
# -e: Exit immediately if a command exits with a non-zero status.
# -u: Treat unset variables as an error when substituting.
# -x: Display expanded script commands

cd fvdi
make CPU="${CPU_TARGET}" V=1
make CPU="${CPU_TARGET}" DESTDIR="${INSTALL_DIR}" install
