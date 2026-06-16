import bcrypt from 'bcryptjs';
import userRepository from "../repositories/userRepository.js"
import { checkUserData } from "../utils.js"
import { v7 as uuidv7 } from "uuid";

class UsersService {
    async getHealth() {
        return {
            status: 200,
            message: 'Users service OK'
        }
    }

    async getMyProfile(userUuid) {

        const me = await userRepository.getUser(userUuid);

        if (!me) {
            throw {
				status: 404,
				message: "User not found."
            }
        }
        return me;
    }

    async patchMyProfile(userUuid, bodyData) {//TO DO AND TO BE TESTED
        const updatedData = {};
            
        //avoid overwrite if the fields are empty, for each field
        if (bodyData.username) updatedData.username = bodyData.username;
        if (bodyData.first_name) updatedData.first_name = bodyData.first_name;
        if (bodyData.last_name) updatedData.last_name = bodyData.last_name;
        if (bodyData.email) updatedData.email = bodyData.email;
        if (bodyData.avatar) updatedData.avatar = bodyData.avatar; //The path string from the gallery view selection
            
        // password separately with hashing (as in Audrey's signup)
        if (bodyData.password) {
            updatedData.password = await bcrypt.hash(bodyData.password, 10);
        }
        const updatedUser = await userRepository.updateUser(userUuid, updatedData);

        if (!updatedUser) {
            throw {
				status: 404,
				message: "User not found."
            }
        }        
        return updatedUser;
    }

    async getCreateUser(newUserData) {
        // Check if user data is valid
        const check = await checkUserData(newUserData); //function utils

        if (check.valid === false)
            throw {
                status: 400,
                message: check.error
        }
        // Check if user doesn't already exist
        const exists = await userRepository.checkEmail(newUserData.email);

        if (exists > 0) {
            throw {
                status: 409,
		    	message: "User already exists. Please login."
            }
        }

        // Hash pwd for database
        const hashedPwd = await bcrypt.hash(newUserData.password, 10);

        // Create uuid for this user
        const user_id = uuidv7();

		// Save it to the database
		const userDataForDb = {
			user_uuid: user_id,
			first_name: newUserData.first_name,
			last_name: newUserData.last_name,
			username: newUserData.username,
			email: newUserData.email,
			password: hashedPwd,
		};

		await userRepository.create(userDataForDb);

		return {
			status: 201,
			message: "User created"
		}
    }

    async getGetUser(identifiers) {
		const { email, password } = identifiers;

		if (!email || !password) {
			throw {
				success: false,
				status: 400,
				message: "All fields are required"
			};
		}

		const user = await userRepository.get(email);

        if (!user) {
            throw {
				success: false,
				status: 401,
				message: "Invalid email or password"
			}
        }

		const isMatch = await bcrypt.compare(password, user.password);

		if (!isMatch) {
			throw {
				success: false,
				status: 401,
				message: "Invalid email or password"
			}
		}

		return {
			status: 200,
			message: "User found",
			user_id: user.user_uuid,
            avatar: user.avatar
		}
    }

    async getBatchUserInfo(batch_user) {

        const users = await userRepository.getMultiple(batch_user);

        return Object.fromEntries(
            users.map(user => [
                user.user_uuid,
                {
                    uuid: user.user_uuid,
                    username: user.username,
                    xp: user.xp,
                    avatar: user.avatar
                }
            ])
        );
    }

    async getUserXP(user_uuid) {

        const user = await userRepository.getByUUID(user_uuid);

        return {
            status: 200,
            message: "User XP is coming",
            user_uuid: user.user_uuid,
            user_xp: user.xp
        }
    }

    async getUpdateXP(usersScores) {
        
        await userRepository.incrementMultipleXP(usersScores);

        return {
            status: 200,
            message: "Users XP updated"
        };
    }
}

export default new UsersService();
