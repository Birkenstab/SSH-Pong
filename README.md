# SSH-Pong
Connect to a SSH server and play Pong

Uses Node for the SSH server but the game is written in C

## Demo
Connect to pong.birkenstab.de via SSH:
```
ssh pong.birkenstab.de
```

## Installation
Use Docker: 
```
docker run --name ssh-pong -p 22:10022 birkenstab/ssh-pong
```
The Docker image exposes Port 10022 for SSH

## Todo
* Highscore list
