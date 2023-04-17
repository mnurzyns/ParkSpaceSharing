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

    DTO_FIELD(oatpp::String, description, "description");
    DTO_FIELD(oatpp::String, location, "location");

    DTO_FIELD_INFO(description)
    {
        info->description = "pole with client and server use to sent and read offer";
    }   

    DTO_FIELD_INFO(location)
    {
        info->description = "pole with server use to sent offer and client read";
    }   

};


#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto
