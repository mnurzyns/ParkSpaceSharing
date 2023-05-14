import NextAuth, { User } from "next-auth";
import DiscordProvider from "next-auth/providers/discord";
import CredentialsProvider from "next-auth/providers/credentials";
import {
  AuthControllerApi,
  SignInDto,
  SignUpDto,
  UserControllerApi,
  UserDto,
} from "@/client";

export const authOptions = {
  callbacks: {
    async jwt({ token, user }: { token: any; user: any }) {
      /* Step 1: update the token based on the user object */
      if (user) {
        token.user = user;
      }
      return token;
    },
    session({ session, token }: { session: any; token: any }) {
      /* Step 2: update the session.user based on the token object */
      if (token && session.user) {
        session.user = token.user;
      }
      return session;
    },
  },
  secret: process.env.NEXTAUTH_SECRET,
  providers: [
    // DiscordProvider({
    //   clientId: process.env.DISCORD_CLIENT_ID ?? "",
    //   clientSecret: process.env.DISCORD_CLIENT_SECRET ?? "",
    // }),
    CredentialsProvider({
      id: "register",
      name: "Register",
      credentials: {
        username: {
          label: "Username",
          type: "text",
          placeholder: "jsmith",
        },
        phone: {
          label: "Phone",
          type: "tel",
          placeholder: "+48 123 123 123",
        },
        email: {
          label: "Email",
          type: "email",
          placeholder: "jsmith@example.com",
        },
        password: {
          label: "Password",
          type: "password",
        },
      },
      async authorize(credentials, req): Promise<User | null> {

        const client = new AuthControllerApi();
        const body: SignUpDto = {
          username: credentials!.username,
          phone: credentials!.phone,
          email: credentials!.email,
          password: credentials!.password,
        };
    
        const res = await client.signUp(body).catch((err) => {
          console.log(err.response.data);
          console.error("Registration error");
          return null;
        });
    
        if (res == null) {
          return null;
        } else {
          const body: SignInDto = {
            login: credentials!.username,
            password: credentials!.password,
          };
          const res = await client.signIn(body).catch((err) => {
            console.log(err.response.data);
            console.error("Invalid credentials");
            return null;
          });
          if (res == null) {
            return null;
          } else {
            var rawToken = res.data.token;
            var token = JSON.parse(
              Buffer.from(res!.data.token!.split(".")[1], "base64").toString()
            );
            console.log("token", token);
            const client: UserControllerApi = new UserControllerApi();
            const userFromRequest: UserDto = (await client.getOne(token.user_id))
              .data;
            if (userFromRequest == null) {
              return null;
            }
            const user: User = {
              id: token.user_id,
              name: userFromRequest.username!,
              email: userFromRequest.email!,
              role: userFromRequest.role!,
              phone: userFromRequest.phone!,
              token: rawToken!,
            };
            return user;
          }
        }
      },
    }),
    CredentialsProvider({
      id: "login",
      name: "Credentials",
      credentials: {
        loginLogin: {
          label: "Login",
          type: "text",
          placeholder: "jsmith",
        },
        loginPassword: { label: "Password", type: "password" },
      },
      async authorize(credentials, req): Promise<User | null> {
        const client = new AuthControllerApi();
        const body: SignInDto = {
          login: credentials!.loginLogin,
          password: credentials!.loginPassword,
        };
        const res = await client.signIn(body).catch((err) => {
          console.log(err.response.data);
          console.error("Invalid credentials");
          return null;
        });
        if (res == null) {
          return null;
        } else {
          var rawToken = res.data.token;
          var token = JSON.parse(
            Buffer.from(res!.data.token!.split(".")[1], "base64").toString()
          );
          console.log("token", token);
          const client: UserControllerApi = new UserControllerApi();
          const userFromRequest: UserDto = (await client.getOne(token.user_id))
            .data;
          if (userFromRequest == null) {
            return null;
          }
          const user = {
            id: token.user_id,
            name: userFromRequest.username!,
            email: userFromRequest.email!,
            role: userFromRequest.role!,
            phone: userFromRequest.phone!,
            token: rawToken!,
          };
          return user;
        }
      },
    }),
  ],
};

export default NextAuth(authOptions);
