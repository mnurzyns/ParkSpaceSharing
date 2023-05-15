import { Configuration, OfferControllerApi, PlaceControllerApi } from "@/client";
import { useRouter } from "next/router";
import { useState } from "react";

const RemoveOfferButton = ({ offer_id, token}: { offer_id: number, token: string }) => {
  const [isButtonDisabled, setIsButtonDisabled] = useState(false);
  const [isLoading, setIsLoading] = useState(false);
  const router = useRouter();

  const fetchDataAndRedirect = async () => {
    // Disable the button
    setIsButtonDisabled(true);

    // Show the loading
    setIsLoading(true);

    try {
      // Fetch data from an API or perform any asynchronous task
      const client: OfferControllerApi = new OfferControllerApi(new Configuration({ accessToken: token }));
      const place = await client.deleteOne(offer_id);
      router.push("/profile");
    } catch (error) {
      // Handle any errors
      console.error("Error:", error);
    }

    // Re-enable the button and hide the loading
    setIsButtonDisabled(false);
    setIsLoading(false);
  };

  return (
    <button
      onClick={fetchDataAndRedirect}
      disabled={isButtonDisabled}
      className={
        "bg-red-600 hover:bg-red-700 text-white font-semibold py-2 px-4 mt-4 rounded-lg shadow-md transition duration-200"
      }
    >
      {isLoading ? "Loading..." : "Remove"}
    </button>
  );
};

export default RemoveOfferButton;
