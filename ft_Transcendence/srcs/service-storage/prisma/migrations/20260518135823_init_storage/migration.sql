-- CreateTable
CREATE TABLE "uploads" (
    "id" SERIAL NOT NULL,
    "owner_uuid" TEXT NOT NULL,
    "file_path" TEXT NOT NULL,
    "original_name" TEXT NOT NULL,
    "file_type" TEXT NOT NULL,
    "created_at" TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT "uploads_pkey" PRIMARY KEY ("id")
);

-- CreateIndex
CREATE UNIQUE INDEX "uploads_file_path_key" ON "uploads"("file_path");