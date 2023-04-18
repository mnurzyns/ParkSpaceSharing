import NextAuth, { User } from "next-auth";
import DiscordProvider from "next-auth/providers/discord";
import CredentialsProvider from "next-auth/providers/credentials";
import {
  AuthControllerApi,
  ParkingSpaceControllerApi,
  SignInDto,
} from "@/client";


export const authOptions = {
  callbacks: {
    async jwt({ token, user }) {
      /* Step 1: update the token based on the user object */
      if (user) {
        token.user = user;
      }
      console.log("jwt token:", token);
      return token;
    },
    session({ session, token }) {
      /* Step 2: update the session.user based on the token object */
      if (token && session.user) {
        session.user = token.user;
      }
      console.log("session", session);
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
        email: {
          label: "Email",
          type: "text",
          placeholder: "jsmith@gmail.com",
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
          email: credentials!.email,
          password: credentials!.password,
        };
        const user: User = {
          id: 1,
          name: "usernameskfk",
          email: "token.user.email",
          admin: false,
          token: "rawToken",
        };
        console.log("user", user);
        return user;
        const res = await client.signIn(body).catch((err) => {
          console.log(err.response.data);
          console.error("Invalid credentials");

        });
        console .log("res", res);
        if (res === undefined) {

          return null;
        } else{
          var rawToken = res!.data.token;
          var token = JSON.parse(
            Buffer.from(res!.data.token!.split(".")[1], "base64").toString()
          );
          //   "iss": "issuer",
          //   "user": "{\"id\":2,\"username\":null,\"email\":null,\"password\":null,\"admin\":null}"
          // }
          token.user = JSON.parse(token.user);
          const user = {
            id: token.user.id,
            name: token.user.username,
            email: token.user.email,
            admin: token.user.admin,
            token: rawToken,
          };
          //return user;
        }

      },
    }),
  ],
};

export default NextAuth(authOptions);
