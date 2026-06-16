FROM node:22-alpine

WORKDIR /app

# Install openssl for prisma
RUN apk add --no-cache openssl python3 build-base

COPY /tools/entrypoint.sh /tmp/entrypoint.sh
COPY prisma /app/prisma/

RUN chmod +x /tmp/entrypoint.sh

RUN apk add --no-cache postgresql-client

COPY conf/package.json /app
RUN npm install
COPY /src /app

RUN npx prisma generate --schema=/app/prisma/schema.prisma

EXPOSE 3002

CMD ["npm", "start"]

ENTRYPOINT ["/tmp/entrypoint.sh"]