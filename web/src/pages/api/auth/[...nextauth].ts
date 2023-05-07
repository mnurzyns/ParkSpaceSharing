import NextAuth, { User } from "next-auth";
import DiscordProvider from "next-auth/providers/discord";
import CredentialsProvider from "next-auth/providers/credentials";
import {
  AuthControllerApi,
  SignInDto,
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
    session({ session, token }: { session: any; token: any}) {
      /* Step 2: update the session.user based on the token object */
      if (token && session.user) {
        session.user = token.user;
      }
      return session;
    },
  },
  // session: {
  //   strategy: "jwt",
  // },
  secret: process.env.NEXTAUTH_SECRET,
  providers: [
    DiscordProvider({
      clientId: process.env.DISCORD_CLIENT_ID ?? "",
      clientSecret: process.env.DISCORD_CLIENT_SECRET ?? "",
    }),
    CredentialsProvider({
      // The name to display on the sign in form (e.g. 'Sign in with...')
      name: "Credentials",
      // The credentials is used to generate a suitable form on the sign in page.
      // You can specify whatever fields you are expecting to be submitted.
      // e.g. domain, username, password, 2FA token, etc.
      // You can pass any HTML attribute to the <input> tag through the object.
      credentials: {
        login: {
          label: "Login",
          type: "text",
          placeholder: "jsmith",
        },
        password: { label: "Password", type: "password" },
      },
      async authorize(credentials, req): Promise<User | null> {
        // You need to provide your own logic here that takes the credentials
        // submitted and returns either a object representing a user or value
        // that is false/null if the credentials are invalid.
        // e.g. return { id: 1, name: 'J Smith', email: 'jsmith@example.com' }
        // You can also use the `req` object to obtain additional parameters
        // (i.e., the request IP address)

        const client = new AuthControllerApi();
        const body: SignInDto = {
          login: credentials!.login,
          password: credentials!.password,
        };
        // const user: User = {
        //   id: 1,
        //   name: "usernameskfk",
        //   email: "token.user.email",
        //   admin: false,
        //   token: "rawToken",
        // };
        // console.log("user", user);
        // return user;
        const res = await client.signIn(body).catch((err) => {
          console.log(err.response.data);
          console.error("Invalid credentials");
          return null;
        });
        if (res == null) {
          return null;
        } else{
          var rawToken = res.data.token;
          var token = JSON.parse(
            Buffer.from(res!.data.token!.split(".")[1], "base64").toString()
          );
          console.log("token", token);
          //   "iss": "issuer",
          //   "user": "{\"id\":2,\"username\":null,\"email\":null,\"password\":null,\"admin\":null}"
          // }
          
          const client: UserControllerApi = new UserControllerApi();
          const userFromRequest: UserDto = (await client.getOne(token.user_id)).data;
          if (userFromRequest == null) {
            return null;
          }
          const user = {
            id: token.user_id,
            name: userFromRequest.username!,
            email: userFromRequest.email!,
            role: userFromRequest.role!,
            token: rawToken!,
          };
          return user;
        }

      },
    }),
  ],
};

export default NextAuth(authOptions);
