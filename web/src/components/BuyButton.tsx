import { PlaceControllerApi } from "@/client";
import { useRouter } from "next/router";
import { useState } from "react";

const BuyButton = ({ place_id }: { place_id: number }) => {
  const [isButtonDisabled, setIsButtonDisabled] = useState(false);
  const [isLoading, setIsLoading] = useState(false);
  const router = useRouter();

  const fetchDataAndRedirect = async () => {
    // Disable the button
    setIsButtonDisabled(true);

    // Show the loading spinner
    setIsLoading(true);

    try {
      // Fetch data from an API or perform any asynchronous task
      const client: PlaceControllerApi = new PlaceControllerApi();
      const place = await client.getOne(place_id);

      // Redirect the page using Next.js Router
      router.push("/profile/" + place.data?.owner_id);
    } catch (error) {
      // Handle any errors
      console.error("Error:", error);
    }

    // Re-enable the button and hide the loading spinner
    setIsButtonDisabled(false);
    setIsLoading(false);
  };

  return (
    <button
      onClick={fetchDataAndRedirect}
      disabled={isButtonDisabled}
      className={
        "bg-secondary hover:bg-secondary-focus text-white font-semibold py-2 px-4 mt-4 rounded-lg shadow-md transition duration-200" +
        `btn ${isLoading ? "btn-primary" : "btn-secondary"}`
      }
    >
      {isLoading ? "Loading..." : "Buy Now"}
    </button>
  );
};

export default BuyButton;
