# SSH-Pong
Connect to a ssh server and play Pong

Uses Node for the ssh server but the game is written in C

## Demo
Connect to pong.birkenstab.de via ssh:
```
ssh pong.birkenstab.de
```

## Installation
Use Docker: 
```
docker run -p 22:10022 birkenstab/ssh-pong
```
The Docker image exposes Port 10022 for ssh
