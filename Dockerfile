FROM node:carbon

RUN groupadd -r nodejs \
   && useradd -m -r -g nodejs nodejs

USER nodejs

RUN mkdir -p /home/nodejs/app
WORKDIR /home/nodejs/app

COPY package*.json ./
RUN npm install --production

COPY . .
RUN gcc -lncurses -std=c11 main.c -o pong

ENV TERM xterm

ADD https://github.com/Yelp/dumb-init/releases/download/v1.1.1/dumb-init_1.1.1_amd64 /usr/local/bin/dumb-init

EXPOSE 22

CMD ["dump-init", "npm", "start"]