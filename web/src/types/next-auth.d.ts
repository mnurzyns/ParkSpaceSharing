import NextAuth, { DefaultUser } from "next-auth"
import { DefaultSession } from "next-auth";

declare module "next-auth" {
  /**
   * Returned by `useSession`, `getSession` and received as a prop on the `SessionProvider` React Context
   */
  interface User {
    id: number,
    name: string,
    email: string,
    admin: boolean,
    token: string,
  }
  interface Session extends DefaultSession {
    user?: User;
  }
}

import { DefaultJWT, JWT } from "next-auth/jwt"

declare module "next-auth/jwt" {
  /** Returned by the `jwt` callback and `getToken`, when using JWT sessions */
  interface JWT extends DefaultJWT {
    /** OpenID ID Token */
    user: User,
  }
}