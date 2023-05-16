import { useState } from "react";
import axios from "axios";
import { Configuration, PlaceControllerApi, PlaceDto } from "@/client";
import { Session } from "next-auth";

const PlacesList = ({ session }: { session: Session }) => {
  const [places, setPlaces] = useState<PlaceDto[]>([]);

  const fetchPlaces = async () => {
    const client = new PlaceControllerApi(
      new Configuration({ accessToken: session.user?.token })
    );
    await client
      .search({
        owner_id: session.user?.id,
        limit: 100,
      })
      .then((res) => {
        if (res.data.items != null) {
          setPlaces(res.data.items);
        } else {
          setPlaces([]);
        }
      })
      .catch((err) => {
        console.log(err.response);
        setPlaces([]);
        console.log("no more places^");
      });
  };

  const handleRemove = async (place_id: number) => {
    const client = new PlaceControllerApi(
      new Configuration({ accessToken: session.user?.token })
    );
    await client.deleteOne(place_id);
    fetchPlaces();
  };

  return (
    <div>
      <div
        className="collapse collapse-arrow border border-base-300 bg-base-100 rounded-box w-full"
        onClick={() => fetchPlaces()}
      >
        <input type="checkbox" />
        <div className="collapse-title text-xl font-medium">Your places</div>
        <div className="collapse-content">
          {places.length > 0 ? (
            <table className="mt-4 w-full">
              <thead>
                <tr>
                  <th className="py-2 px-4 bg-primary w-2/5">Name</th>
                  <th className="py-2 px-4 bg-primary w-2/5">Address</th>
                  <th className="py-2 px-4 bg-primary w-1/5">Action</th>
                </tr>
              </thead>
              <tbody>
                {places.map((place) => (
                  <tr key={place.id}>
                    <td className="py-2 px-4">{place.id}</td>
                    <td className="py-2 px-4">{place.address}</td>
                    <td className="py-2 px-4">
                      <button
                        className="p-2 bg-red-500 text-white rounded"
                        onClick={() => handleRemove(place.id!)}
                      >
                        Remove
                      </button>
                    </td>
                  </tr>
                ))}
              </tbody>
            </table>
          ) : (
            <tr>
              <td className="py-2 px-4">No places</td>
            </tr>
          )}
        </div>
      </div>
    </div>
  );
};

export default PlacesList;
