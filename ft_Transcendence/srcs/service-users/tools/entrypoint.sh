#!/bin/sh

#for debug
#echo "Seeding test user into database..."
#npx prisma db seed
echo "Waiting for database..."
until pg_isready -h "$DB_HOST" -p "$PORT_DB" -U "$POSTGRES_USER"; do
  echo "Waiting for database to be ready..."
  sleep 2
done

echo "Database is up. Migrating prisma schema..."

cd /app && npx prisma migrate deploy
#cd /app && npx prisma db push --skip-generate 

echo "Schema synced."

npx prisma db seed

echo "Test users seeded. Starting app"

#commands are defined in the Dockerfile
exec "$@"