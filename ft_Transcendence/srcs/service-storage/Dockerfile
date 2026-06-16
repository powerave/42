FROM node:22-alpine

WORKDIR /app

COPY conf/package.json /app
COPY /prisma /app/prisma/
COPY /src /app
COPY /scripts /app/scripts
COPY /public /app/public

#RUN apk add --no-cache openssl libc6-compat
RUN apk add --no-cache openssl
RUN npm install --omit=dev 
RUN apk add --no-cache postgresql-client

COPY /tools/entrypoint.sh /tmp/entrypoint.sh

RUN chmod +x /tmp/entrypoint.sh

RUN npx prisma generate --schema=/app/prisma/schema.prisma
#RUN npm run build
#no root user for security
#RUN chown -R node:node /app && chown -R node:node /tmp

#USER node

ENTRYPOINT ["/tmp/entrypoint.sh"]

CMD [ "node", "server.js" ]