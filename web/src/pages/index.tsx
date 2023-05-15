import Head from "next/head";
import Image from "next/image";
import { Inter } from "@next/font/google";
import Navbar from "@/components/Navbar";
import { BaseAPI } from "@/client/base";
import { OfferControllerApi, OfferDto } from "@/client/api";
import { useEffect, useState } from "react";
import { off } from "process";
import { useSession } from "next-auth/react";
import BuyButton from "@/components/BuyButton";

export default function Home() {
  const [offers, setOffers] = useState<OfferDto[]>([]);
  const [currentPage, setCurrentPage] = useState(0);
  const [totalPages, setTotalPages] = useState(1);
  const itemsPerPage = 12;

  const { data: session } = useSession();

  useEffect(() => {

    const client: OfferControllerApi = new OfferControllerApi();
    client
      .search({limit: itemsPerPage, offset: currentPage * itemsPerPage})
      .then((res) => {
        if (res.data.items != null) {
          console.log(res.data);
          setOffers(res.data.items);
          setTotalPages(Math.ceil(res.data.count! / itemsPerPage));
        }
      })
      .catch((err) => {
        console.log(err.response);
      });
  }, [currentPage]);

  const goToNextPage = () => {
    setCurrentPage((prevPage) => prevPage + 1);
  };

  const goToPreviousPage = () => {
    setCurrentPage((prevPage) => prevPage - 1);
  };



  return (
    <div>
      <Navbar session={session} />
      <div className="flex flex-wrap justify-around ">
        {offers?.map((offer, index) => (
          <div key={index}>
            <div className="card w-80 m-4 bg-gradient-to-br from-purple-600 via-primary to-primary text-white p-2 shadow-md">
              <div className="card-body">
                <h2 className="card-title">Offer {offer.id}</h2>
                <p>Place ID: {offer.place_id}</p>
                <p>
                  Date From: {new Date(offer.date_from!).toLocaleDateString()}
                </p>
                <p>Date To: {new Date(offer.date_to!).toLocaleDateString()}</p>
                <p>Description: {offer.description}</p>
                <p>Price: ${offer.price}</p>

                <div className="card-actions justify-end">
                  <BuyButton place_id={offer.place_id!} />
                </div>
              </div>
            </div>
          </div>
        ))}
      </div>
      {currentPage === totalPages - 1 && (
        <div className="flex justify-center">No more offers!</div>
      )}
      {/* pagination */}
      <div className="btn-group grid grid-cols-2">
        <button
          className="btn btn-outline"
          onClick={goToPreviousPage}
          disabled={currentPage === 0}
        >
          Previous page
        </button>
        <button
          className="btn btn-outline"
          onClick={goToNextPage}
          disabled={currentPage >= totalPages - 1}
        >
          Next
        </button>
      </div>
    </div>
  );
}
