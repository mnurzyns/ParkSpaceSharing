#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

class offer_dto :
    public ::oatpp::DTO
{
    DTO_INIT(offer_dto, DTO)

    DTO_FIELD(UInt32, id);
    DTO_FIELD(UInt32, id_parking_space, "id_parkingSpace");
    DTO_FIELD(String, description,      "description"    );
    DTO_FIELD(UInt32, user_id ,         "user_id"        );

    DTO_FIELD_INFO(user_id)
    {
        info->description = "id of the offer's owner";
    }
};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto
