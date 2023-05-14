import Navbar from "@/components/Navbar";
import { useEffect, useState } from "react";
import { signIn, useSession } from "next-auth/react";
import AddPlaceForm from "@/components/AddPlaceForm";
import { OfferDto, OfferControllerApi } from "@/client";

export default function Profile() {
  const { data: session } = useSession();
  const [offers, setOffers] = useState<OfferDto[]>([]);

  
  useEffect(() => {
    const client: OfferControllerApi = new OfferControllerApi();
    client
      .search({limit: 50, offset: 0, owner_id: session?.user?.id})
      .then((res) => {
        if (res.data.items != null) {
          console.log(res.data);
          setOffers(res.data.items);
        }
      })
      .catch((err) => {
        console.log(err.response);
      });
  }, [session]);


  if (session) {
    return (
      <div>
        <Navbar session={session} />
        <div className="container mx-auto py-12">
          <div className="px-4 py-2 text-center">
            <h2 className="text-3xl font-bold">{session.user?.name}</h2>
            <p className="text-gray-500">{session.user?.phone}</p>
          </div>
          <div className="flex flex-wrap justify-around ">
            {offers?.map((offer, index) => (
              <div key={index}>
                <div className="card w-80 m-4 bg-gradient-to-br from-purple-600 via-primary to-primary text-white p-2 shadow-md">
                  <div className="card-body">
                    <h2 className="card-title">Offer {offer.id}</h2>
                    <p>Place ID: {offer.place_id}</p>
                    <p>
                      Date From:{" "}
                      {new Date(offer.date_from!).toLocaleDateString()}
                    </p>
                    <p>
                      Date To: {new Date(offer.date_to!).toLocaleDateString()}
                    </p>
                    <p>Description: {offer.description}</p>
                    <p>Price: ${offer.price}</p>

                    <div className="card-actions justify-end">
                      <button className="bg-secondary hover:bg-secondary-focus text-white font-semibold py-2 px-4 mt-4 rounded-lg shadow-md transition duration-200">
                        Buy Now
                      </button>
                    </div>
                  </div>
                </div>
              </div>
            ))}
          </div>
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
