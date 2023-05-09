import Navbar from "@/components/Navbar";
import { useEffect } from "react";
import { signIn, useSession } from "next-auth/react";
import AddPlaceForm from "@/components/AddPlaceForm";

export default function AddOfferPage() {
  const { data: session } = useSession();

  if (session) {
    return (
      <div>
        <Navbar session={session} />
        <div className="container mx-auto py-12">
      <h1 className="text-2xl font-bold mb-4">Add place form</h1>
      <AddPlaceForm user={session!.user!} />
    </div>
      </div>
    );
  }
  return (
    <>
      {" "}
      Not signed in <br /> <button onClick={() => signIn()}>
        Sign in
      </button>{" "}
    </>
  );
}
