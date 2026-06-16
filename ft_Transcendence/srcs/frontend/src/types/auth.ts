export interface LoginPayload {
  email: string;
  password: string;
}

export interface SignupPayload {
  first_name: string;
  last_name: string;
  username: string;
  email: string;
  password: string;
}

export interface ApiAuthResponse {
  success?: boolean;
  message?: string;
  token?: string;
  [key: string]: unknown;
}
