import Head from "next/head";
import Image from "next/image";
import { Inter } from "@next/font/google";
import Navbar from "@/components/Navbar";
import { BaseAPI } from "@/client/base";
import { OfferControllerApi, OfferDto } from "@/client/api";
import { useEffect, useState } from "react";
import { off } from "process";

export default function Home() {
  //useEffect(() => {
  //  const base = new BaseAPI()

  const [offers, setOffers] = useState<OfferDto[]>();

  useEffect(() => {
    const offerClient = new OfferControllerApi();
    offerClient
      .getOffers()
      .then((res) => {
        console.log(res);
      })
      .catch((err) => {
        console.log(err.response);
      });
  });

  return (
    <div>
      <Navbar />
      <div>
        <div className="card w-96 bg-primary shadow-xl">
          <div className="card-body">
            <h2 className="card-title">Card title!</h2>
            <p>If a dog chews shoes whose shoes does he choose?</p>
            <div className="card-actions justify-end">
              <button className="btn btn-primary">Buy Now</button>
            </div>
          </div>
        </div>
        {offers?.forEach((offer) => (
          <div>
            <div className="card w-96 bg-primary shadow-xl">
              <div className="card-body">
                <h2 className="card-title">{offer.location}</h2>
                <p>{offer.location}</p>
                <div className="card-actions justify-end">
                  <button className="btn btn-primary">Buy Now</button>
                </div>
              </div>
            </div>
          </div>
        ))}
      </div>
      hello world
    </div>
  );
}
