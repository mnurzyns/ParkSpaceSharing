import {
  Configuration,
  OfferControllerApi,
  OfferDto,
  PlaceControllerApi,
} from "@/client";
import { Session } from "next-auth";
import { useRouter } from "next/router";
import { off } from "process";
import { Dispatch, SetStateAction, useState } from "react";

const RemoveOfferButton = ({
  offer_id,
  session,
  offers,
  setOffers,
}: {
  offer_id: number;
  session: Session;
  offers: OfferDto[];
  setOffers: Dispatch<SetStateAction<OfferDto[]>>;
}) => {
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
      const client: OfferControllerApi = new OfferControllerApi(
        new Configuration({ accessToken: session.user?.token })
      );
      const place = await client.deleteOne(offer_id);
      setOffers(offers.filter((offer) => offer.id !== offer_id));
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
