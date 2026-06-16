import jwt from "jsonwebtoken";
import authService from "../services/authService.js";

export async function socketAuthMiddleware(socket, next) {
  const session = socket.request.session;

  if (!session?.auth) {
    return next(new Error("Not logged"));
  }

  try {
    const verified = jwt.verify(session.auth.accessToken, process.env.JWT_ACCESS_SECRET);
    socket.userUuid = verified.userId;
    return next();

  } catch (err) {
    if (err.name === 'TokenExpiredError') {
      try {
        const data = await authService.getRefresh(session.auth.refreshToken);

        session.auth = {
          user: data.user_id,
          accessToken: data.accessToken,
          refreshToken: data.refreshToken
        };

        socket.userUuid = jwt.decode(data.accessToken).userId;
        return next();

      } catch (refreshErr) {
        session.destroy(() => {});
        return next(new Error("Session expired"));
      }
    }

    return next(new Error("Invalid token"));
  }
}
