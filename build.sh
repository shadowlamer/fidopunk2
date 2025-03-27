#!/bin/bash

PROJECT="fidopunk"
TAPE="${PWD}/${PROJECT}.tap"
ROM="${PWD}/bin/main.c.rom"
CODE="${PWD}/main.bin"
INTRO="${PWD}/echo.scr"
SKIP=536

END=$(grep -oba $'\xde\xad\xbe\xef' ${ROM} | cut -d: -f1)
dd if=${ROM} of=${CODE} bs=1 skip=${SKIP} count=$((0x${END}-${SKIP}))

zmakebas -i 1 -a 10 -l -o ${TAPE} -n ${PROJECT} loader.bas

# Install dependencies
[ -d node_modules ] || npm install jsbin2tap

node_modules/.bin/jsbin2tap -p -o ${TAPE} ${INTRO} ${CODE}