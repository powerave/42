export function errorHandler(err, req, res, next) {

	if (err.response) {
		return res.status(err.response.status).json({
			error: err.response.data.error
		});
	}
		
	console.log(err);

	res.status(500).json({
		error: "Gateway internal error"
	});
}
