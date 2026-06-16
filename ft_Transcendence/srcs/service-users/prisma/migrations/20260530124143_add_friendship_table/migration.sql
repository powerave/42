-- CreateTable
CREATE TABLE "friendships" (
    "id" SERIAL NOT NULL,
    "requester_uuid" UUID NOT NULL,
    "receiver_uuid" UUID NOT NULL,
    "status" TEXT NOT NULL DEFAULT 'PENDING',
    "created_at" TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT "friendships_pkey" PRIMARY KEY ("id")
);

-- CreateIndex
CREATE UNIQUE INDEX "friendships_requester_uuid_receiver_uuid_key" ON "friendships"("requester_uuid", "receiver_uuid");

-- AddForeignKey
ALTER TABLE "friendships" ADD CONSTRAINT "friendships_requester_uuid_fkey" FOREIGN KEY ("requester_uuid") REFERENCES "users"("user_uuid") ON DELETE CASCADE ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "friendships" ADD CONSTRAINT "friendships_receiver_uuid_fkey" FOREIGN KEY ("receiver_uuid") REFERENCES "users"("user_uuid") ON DELETE CASCADE ON UPDATE CASCADE;
