function isValidEmail(email) {
	if (typeof(email) !== "string") {
		return false;
	}

	email = email.trim().toLowerCase();

	const regex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;

	return regex.test(email);
}

function isValidName(name) {
	if (typeof(name) !== "string") {
		return false;
	}

	name = name.trim();

	if (name.length < 1 || name.length > 100) {
		return false;
	}

	const regex = /^[a-zA-ZÀ-ÿ '-]+$/;

	return regex.test(name);
}

function isValidPassword(pwd) {
	if (typeof(pwd) !== "string") {
		return false;
	}

	if (pwd.length < 8) {
		return false;
	}

	return true;
}

export async function checkUserData(userData) {
	const { first_name, last_name, username, email, password } = userData;

	if (!first_name || !last_name || !username || !email || !password) {
		return {
			valid: false,
			error: "All fields are required"
		}
	}

	if (isValidEmail(email) === false) {
		return {
			valid: false,
			error: "Invalid email format"
		}
	}

	if (isValidName(first_name) === false) {
		return {
			valid: false,
			error: "Invalid first name"
		}
	}

	if (isValidName(last_name) === false) {
		return {
			valid: false,
			error: "Invalid last name"
		}
	}

	if (isValidName(username) === false) {
		return {
			valid: false,
			error: "Invalid username"
		}
	}

	if (isValidPassword(password) === false) {
		return {
			valid: false,
			error: "Invalid password"
		}
	}

	return {
		valid: true
	}
}