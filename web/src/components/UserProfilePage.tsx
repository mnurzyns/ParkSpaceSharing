import { useRouter } from "next/router";
import { useEffect, useState } from "react";
import { User } from "next-auth";
import {
  OfferControllerApi,
  OfferDto,
  UserControllerApi,
  UserDto,
} from "@/client";
import Navbar from "./Navbar";
import { useSession } from "next-auth/react";
import BuyButton from "./BuyButton";

const UserProfilePage = () => {
  const router = useRouter();
  const { id } = router.query;
  const [user, setUser] = useState<UserDto | null>(null);
  const [offers, setOffers] = useState<OfferDto[]>([]);
  const { data: session } = useSession();

  useEffect(() => {
    // Fetch user data based on the ID
    const fetchUser = async () => {
      const client: UserControllerApi = new UserControllerApi();
      const fetchedUser = await client.getOne(Number(id));
      if (fetchedUser.data) {
        setUser(fetchedUser.data);
      }
    };

    const fetchOffers = async () => {
      const client: OfferControllerApi = new OfferControllerApi();
      client
        .search({ limit: 50, offset: 0, owner_id: Number(id) })
        .then((res) => {
          if (res.data.items != null) {
            console.log(res.data);
            setOffers(res.data.items);
          }
        })
        .catch((err) => {
          console.log(err.response);
        });
    };

    if (id) {
      fetchUser();
      fetchOffers();
    }

    // Redirect to /profile if session user ID is the same as id
    if (session?.user?.id === Number(id)) {
      router.push("/profile");
    }
  }, [id, router, session]);

  return (
    <div>
      {user ? (
        <div>
          <Navbar session={session} />
          <div className="container mx-auto py-12">
            <div className="px-4 py-2 text-center">
              <h2 className="text-3xl font-bold">{session?.user?.name}</h2>
              <p className="text-gray-500">{session?.user?.phone}</p>
            </div>
            <div className="flex flex-wrap justify-around ">
              {offers.length > 0 ? (
                offers?.map((offer, index) => (
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
                          Date To:{" "}
                          {new Date(offer.date_to!).toLocaleDateString()}
                        </p>
                        <p>Description: {offer.description}</p>
                        <p>Price: ${offer.price}</p>
                      </div>
                    </div>
                  </div>
                ))
              ) : (
                <p>User has no offers</p>
              )}
            </div>
          </div>
        </div>
      ) : (
        <p>Loading user data...</p>
      )}
    </div>
  );
};

export default UserProfilePage;
