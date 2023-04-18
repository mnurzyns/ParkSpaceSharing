import Head from "next/head";
import Image from "next/image";
import { Inter } from "@next/font/google";
import Navbar from "@/components/Navbar";
import { BaseAPI } from "@/client/base";
import { OfferDto, ParkingSpaceControllerApi } from "@/client/api";
import { useEffect, useState } from "react";
import { off } from "process";

export default function AddOfferPage() {


  return (
    <div>
      <Navbar />
      <div>
        
      </div>
      <div className="flex justify-center">No more offers!</div>
    </div>
  );
}
