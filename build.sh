#!/bin/bash

#gcc -lncurses main.c -o pong

PONG_VERSION=$(node -e "console.log(JSON.parse(require('fs').readFileSync('package.json', 'utf-8')).version)")

echo "Building version ${PONG_VERSION}"

docker build -t birkenstab/ssh-pong:${PONG_VERSION} .

# Update latest tag
docker tag birkenstab/ssh-pong:1.0.0 birkenstab/ssh-pong