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

    DTO_FIELD(oatpp::UInt32, id, "id");
    DTO_FIELD(oatpp::UInt32, parking_space_id, "parking_space_id");
    DTO_FIELD(oatpp::String, description, "description");
    DTO_FIELD(oatpp::String, location, "location");

    DTO_FIELD_INFO(parking_space_id)
    {
        info->description = "pole with client use to sent and server read";
    }

    DTO_FIELD_INFO(description)
    {
        info->description = "pole with client and server use to sent and read";
    }   

    DTO_FIELD_INFO(id)
    {
        info->description = "pole with server use to sent and client read";
    }  

    DTO_FIELD_INFO(location)
    {
        info->description = "pole with server use to sent and client read";
    }   

};


#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto
