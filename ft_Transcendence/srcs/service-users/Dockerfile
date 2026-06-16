FROM node:22-alpine

WORKDIR /app

COPY conf/package.json /app
COPY /prisma /app/prisma/
COPY /src /app

RUN npm install
#RUN apk add --no-cache openssl libc6-compat
RUN apk add --no-cache openssl
RUN apk add --no-cache postgresql-client 
RUN  npx prisma generate --schema=/app/prisma/schema.prisma

COPY /tools/entrypoint.sh /tmp/entrypoint.sh

RUN chmod +x /tmp/entrypoint.sh

ENTRYPOINT ["/tmp/entrypoint.sh"]

CMD [ "node", "server.js" ]