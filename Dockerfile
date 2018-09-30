FROM node:carbon

ADD https://github.com/Yelp/dumb-init/releases/download/v1.1.1/dumb-init_1.1.1_amd64 /usr/local/bin/dumb-init
RUN chmod +x /usr/local/bin/dumb-init

RUN mkdir -p /home/nodejs/app
WORKDIR /home/nodejs/app

COPY package*.json ./
RUN npm install --production

RUN mkdir settings
COPY sshServer.js .

COPY main.c .
RUN gcc -lncurses -std=c11 main.c -o pong

ENV TERM xterm

EXPOSE 22

CMD ["/usr/local/bin/dumb-init", "npm", "start"]
