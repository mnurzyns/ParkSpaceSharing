#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

class parking_space :
    public ::oatpp::DTO
{
    DTO_INIT(parking_space, DTO)

    DTO_FIELD(UInt32, id);

    DTO_FIELD_INFO(user_id)
    {
        info->description = "user_id of the owner";
    }
    DTO_FIELD(UInt32, user_id);

    DTO_FIELD(String, location);
};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto
