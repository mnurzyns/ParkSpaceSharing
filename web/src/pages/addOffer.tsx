import Head from "next/head";
import Image from "next/image";
import { Inter } from "@next/font/google";
import Navbar from "@/components/Navbar";
import { BaseAPI } from "@/client/base";
import { useEffect, useState } from "react";
import { off } from "process";
import { signIn, useSession } from "next-auth/react";
import AddOfferForm from "@/components/AddOfferForm";

export default function AddOfferPage() {
  const { data: session } = useSession();
  useEffect(() => {
    console.log(session);
  }, [session]);
  if (session) {
    return (
      <div>
        <Navbar />
        <div className="container mx-auto py-12">
      <h1 className="text-2xl font-bold mb-4">Add offer form</h1>
      <AddOfferForm />
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
