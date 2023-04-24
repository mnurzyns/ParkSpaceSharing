import Head from "next/head";
import Image from "next/image";
import { Inter } from "@next/font/google";
import Navbar from "@/components/Navbar";
import { BaseAPI } from "@/client/base";
import { OfferControllerApi, OfferDto, ParkingSpaceControllerApi } from "@/client/api";
import { useEffect, useState } from "react";
import { off } from "process";

export default function Home() {
  //useEffect(() => {
  //  const base = new BaseAPI()

  const [offers, setOffers] = useState<OfferDto[]>([]);

  useEffect(() => {
    const client: OfferControllerApi = new OfferControllerApi();
    client.getOffers()
      .then((res) => {
        if (res.data.items[0] != null) {
          console.log(res.data);
          setOffers(res.data.items);
        }
      })
      .catch((err) => {
        console.log(err.response);
      });
  },[]);

  useEffect(() => {
    console.log(offers);
  }, [offers]);




  return (
    <div>
      <Navbar />
      <div className="flex flex-wrap justify-around">
        <div className="card w-96 bg-primary shadow-xl">
          <div className="card-body">
            <h2 className="card-title">Test offer!</h2>
            <p>Not loaded using api</p>
            <div className="card-actions justify-end">
              <button className="btn btn-primary">Buy Now</button>
            </div>
          </div>
        </div>
        {offers?.map((offer, index) =>  (
          <div key={index}>
            <div className="card w-96 bg-primary shadow-xl">
              <div className="card-body">
                <h2 className="card-title">{offer.parking_space_id}</h2>
                <p>{offer.description}</p>
                <div className="card-actions justify-end">
                  <button className="btn btn-primary">Buy Now</button>
                </div>
              </div>
            </div>
          </div>
        ))}
      </div>
      <div className="flex justify-center">No more offers!</div>
    </div>
  );
}
