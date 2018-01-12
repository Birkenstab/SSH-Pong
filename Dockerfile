FROM node:carbon

RUN groupadd -r nodejs \
   && useradd -m -r -g nodejs nodejs

ADD https://github.com/Yelp/dumb-init/releases/download/v1.1.1/dumb-init_1.1.1_amd64 /usr/local/bin/dumb-init
RUN chmod +x /usr/local/bin/dumb-init

USER nodejs

RUN mkdir -p /home/nodejs/app
WORKDIR /home/nodejs/app

COPY package*.json ./
RUN npm install --production

COPY host.key .
COPY sshServer.js .

COPY main.c .
RUN gcc -lncurses -std=c11 main.c -o pong

ENV TERM xterm

EXPOSE 10022

CMD ["/usr/local/bin/dumb-init", "npm", "start"]
