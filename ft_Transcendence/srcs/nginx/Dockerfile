FROM node:20-alpine AS frontend-builder
WORKDIR /app

COPY frontend/package*.json ./
RUN npm install
COPY frontend/ ./

# Build frontend - ensure index.html exists
RUN if [ ! -f index.html ]; then echo "ERROR: index.html not found!"; ls -la; exit 1; fi && npm run build

FROM docker.io/library/nginx:alpine
COPY nginx/nginx.conf /etc/nginx/nginx.conf
COPY --from=frontend-builder /app/dist /usr/share/nginx/html
