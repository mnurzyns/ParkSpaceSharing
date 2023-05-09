import { Configuration, OfferControllerApi, PlaceControllerApi, PlaceDto } from "@/client";
import { User } from "next-auth";
import { useEffect, useState } from "react";

const AddOfferForm = ({user}: {user: User})  => {
  const [formData, setFormData] = useState({
    place_id: 0,
    date_from: 0,
    date_to: 0,
    description: "",
    price: 0,
  });
  const [places, setPlaces] = useState<PlaceDto[]>([]);

  const handleChange = (e: any) => {
    const { name, value } = e.target;
    setFormData({ ...formData, [name]: value });
  };

  const handleSubmit = (e: any) => {
    e.preventDefault();
    const client: OfferControllerApi = new OfferControllerApi(new Configuration({ accessToken: user.token }));
    const dateFromTimestamp = new Date(formData.date_from).getTime() / 1000;
    const dateToTimestamp = new Date(formData.date_to).getTime() / 1000;
    const priceAsNumber = Number(formData.price);
    const place_idAsNumber = Number(formData.place_id);
    if (place_idAsNumber === 0) {
      return;
    }

    const submittedData = {
      ...formData,
      place_id: place_idAsNumber,
      price: priceAsNumber,
      date_from: dateFromTimestamp,
      date_to: dateToTimestamp,
    };
    client.createOne(submittedData);
  };

  useEffect(() => {
    const placeApi = new PlaceControllerApi(new Configuration({ accessToken: user.token }));
    placeApi.search({owner_id: user.id })
      .then((res) => {
        if (res.data.items != null) {
          console.log(res.data);
          setPlaces(res.data.items);
        }
      })
      .catch((err) => {
        console.log(err.response);
      });
  }, [user.id, user.token]);

  return (
    <div>
      <form onSubmit={handleSubmit} className="p-4 space-y-4">
      <div>
          <label htmlFor="place_id" className="block mb-2">
            Address:
          </label>
          <select
            name="place_id"
            id="place_id"
            value={formData.place_id}
            onChange={handleChange}
            className="select select-bordered w-full"
          >
            <option selected={true} value={0}>Select Address</option>
            {places.map((place) => (
              <option key={place.id} value={place.id}>
                {place.address} {place.id}
              </option>
            ))}
          </select>
        </div>
        <div>
          <label htmlFor="date_from" className="block mb-2">
            Date From:
          </label>
          <input
            type="date"
            name="date_from"
            id="date_from"
            value={formData.date_from}
            onChange={handleChange}
            className="input input-bordered w-full"
          />
        </div>
        <div>
          <label htmlFor="date_to" className="block mb-2">
            Date To:
          </label>
          <input
            type="date"
            name="date_to"
            id="date_to"
            value={formData.date_to}
            onChange={handleChange}
            className="input input-bordered w-full"
          />
        </div>
        <div>
          <label htmlFor="description" className="block mb-2">
            Description:
          </label>
          <textarea
            name="description"
            id="description"
            value={formData.description}
            onChange={handleChange}
            placeholder="Description"
            className="textarea textarea-bordered w-full"
          />
        </div>
        <div>
        <label htmlFor="price" className="block mb-2">Price:</label>
        <div className="relative">
          <span className="absolute inset-y-0 left-0 flex items-center pl-3">
            $
          </span>
          <input
            type="number"
            name="price"
            id="price"
            value={formData.price}
            onChange={handleChange}
            placeholder="Price"
            className="input input-bordered w-full pl-8"
          />
        </div>
      </div>
        <button type="submit" className="btn btn-primary w-full">
          Submit
        </button>
      </form>
    </div>
  );
};

export default AddOfferForm;
