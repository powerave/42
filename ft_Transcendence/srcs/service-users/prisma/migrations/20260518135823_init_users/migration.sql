-- CreateTable
CREATE TABLE "users" (
    "id" BIGSERIAL NOT NULL,
    "user_uuid" UUID NOT NULL,
    "first_name" TEXT NOT NULL,
    "last_name" TEXT NOT NULL,
    "username" TEXT NOT NULL,
    "email" TEXT NOT NULL,
    "password" TEXT NOT NULL,
    "avatar" TEXT NOT NULL DEFAULT 'AVATAR/default_avatar.jpg',
    "xp" INT DEFAULT 0,

    CONSTRAINT "users_pkey" PRIMARY KEY ("id")
);

-- CreateIndex
CREATE UNIQUE INDEX "users_user_uuid_key" ON "users"("user_uuid");

-- CreateIndex
CREATE UNIQUE INDEX "users_email_key" ON "users"("email");