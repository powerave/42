#!/bin/sh

#npx prisma generate --schema=/app/prisma/schema.prisma

#set up upload folders according to the categories in the enum in the prisma schema
node scripts/init-minio.js

echo "Waiting for database..."
until pg_isready -h "$DB_HOST" -p "$PORT_DB" -U "$POSTGRES_USER"; do
  echo "Waiting for database to be ready..."
  sleep 2
done

echo "Database is up. Migrating prisma schema..."

#cd /app && npx prisma db push --skip-generate

cd /app && npx prisma migrate deploy && npx prisma db seed 

echo "Schema synced. Starting app"

#commands are defined in the Dockerfile
exec "$@"