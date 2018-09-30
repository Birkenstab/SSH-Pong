# SSH-Pong
Connect to a SSH server and play Pong

Uses Node for the SSH server but the game is written in C

## Demo
Connect to pong.brk.st via SSH:
```
ssh pong.brk.st
```

## Installation
Use Docker: 
```
docker run --name ssh-pong -p 22:22 birkenstab/ssh-pong
```
The Docker image exposes Port 22 for SSH

### Example docker-compose file
```Dockerfile
version: "2.1"

 services:
   web:
     image: 'birkenstab/ssh-pong'
     restart: always
     ports:
       - '22:22'
```

## Todo
* Highscore list
