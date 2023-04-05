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
    DTO_FIELD(UInt32, id_parkingSpace, "id_parkingSpace");
    DTO_FIELD(String, description,     "description"    );

};


#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto
