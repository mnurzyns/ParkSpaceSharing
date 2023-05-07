import { OfferControllerApi } from "@/client";
import { useState } from "react";

const AddOfferForm = () => {
  const [formData, setFormData] = useState({
    place_id: 0,
    date_from: 0,
    date_to: 0,
    description: "",
    price: 0,
  });

  const handleChange = (e: any) => {
    const { name, value } = e.target;
    setFormData({ ...formData, [name]: value });
  };

  const handleSubmit = (e: any) => {
    e.preventDefault();
    const client: OfferControllerApi = new OfferControllerApi();
    client.createOne(formData);
    console.log(formData);
  };

  return (
    <div>
      <form onSubmit={handleSubmit} className="p-4 space-y-4">
        <div>
          <label htmlFor="place_id" className="block mb-2">
            Place ID:
          </label>
          <input
            type="number"
            name="place_id"
            id="place_id"
            value={formData.place_id}
            onChange={handleChange}
            placeholder="Place ID"
            className="input input-bordered w-full"
          />
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
