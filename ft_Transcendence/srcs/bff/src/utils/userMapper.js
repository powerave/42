export function mapUserProfile(user) {

    //adds the minioUrl
    //adapts to what actually exists in the user that has to be mapped, uuid is necessary
    //if (!user|| !user.user_uuid) return null;

    //const minioUrl = `${process.env.MINIO_PUBLIC_URL}:${process.env.PORT_MINIO_INT}/${process.env.MINIO_BUCKET}`;
    const minioUrl = `${process.env.MINIO_PUBLIC_URL}`;
    return {
        user_uuid: user.user_uuid,

        ...(user.username && { username: user.username }),
        ...(user.first_name && { first_name: user.first_name }),
        ...(user.last_name && { last_name: user.last_name }),
        ...(user.email && { email: user.email }),
        ...(user.xp && { xp: user.xp }),

        ...(user.avatar && {
            avatar: `${minioUrl}/${user.avatar}`
        })
    };
}
