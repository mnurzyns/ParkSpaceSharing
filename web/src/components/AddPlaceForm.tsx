import { Configuration, PlaceControllerApi, UserDto } from '@/client';
import { User } from 'next-auth';
import { useState } from 'react';

const AddPlaceForm = ({user}: {user: User})  => {

  const [formData, setFormData] = useState({
    owner_id: user.id,
    address: '',
    latitude: 0,
    longitude: 0,
  });

  const handleChange = (e: any) => {
    const { name, value } = e.target;
    setFormData({ ...formData, [name]: value });
  };

  const handleSubmit = (e:any) => {
    e.preventDefault();
    const client:PlaceControllerApi = new PlaceControllerApi(new Configuration({ accessToken: user.token }));
    client.createOne(formData);
  };

  return (
    <form onSubmit={handleSubmit} className="p-4 space-y-4">
      <div>
        <label htmlFor="address" className="block mb-2">Address:</label>
        <input
          type="text"
          name="address"
          id="address"
          value={formData.address}
          onChange={handleChange}
          placeholder="Address"
          className="input input-bordered w-full"
        />
      </div>
      <div>
        <label htmlFor="latitude" className="block mb-2">Latitude:</label>
        <input disabled
          type="number"
          name="latitude"
          id="latitude"
          value={formData.latitude}
          onChange={handleChange}
          placeholder="Latitude"
          className="input input-bordered w-full"
          step="0.000001"
        />
      </div>
      <div>
        <label htmlFor="longitude" className="block mb-2">Longitude:</label>
        <input disabled
          type="number"
          name="longitude"
          id="longitude"
          value={formData.longitude}
          onChange={handleChange}
          placeholder="Longitude"
          className="input input-bordered w-full"
          step="0.000001"
        />
      </div>
      <button type="submit" className="btn btn-primary w-full">
        Submit
      </button>
    </form>
  );
};

export default AddPlaceForm;