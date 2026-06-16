import jwt from "jsonwebtoken";


export function errorHandler(err, req, res, next) {
    console.log(err);
    res.status(err.status || 500).json({
        error: err.message || "Internal server error"
    });
}

export function decodeJWT(req, res, next) {
	const authHeader = req.headers['authorization'] || req.headers['Authorization'];
    
	if (!authHeader || !authHeader.startsWith('Bearer ')) {
		return res.status(401).json({ error: 'JWT access token missing or malformed' });
	}

	const token = authHeader.split(' ')[1];

	try {
		const decoded = jwt.verify(token, process.env.JWT_ACCESS_SECRET, {
			algorithms: [process.env.ENCRYPTION_ALGO] 
		});
		req.userUuid = decoded.userId; 
		
		return next();
	} catch (err) {
		return res.status(401).json({ error: 'Invalid or expired token' });
	}
}