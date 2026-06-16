FROM node:22-alpine

WORKDIR /app

# Install openssl for prisma
RUN apk add --no-cache openssl
#RUN apk add --no-cache openssl libc6-compat


COPY conf/package.json /app
RUN npm install
COPY /src /app

EXPOSE 3042

CMD ["npm", "start"]