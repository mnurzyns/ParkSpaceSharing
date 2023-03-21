#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

class status_dto :
    public ::oatpp::DTO
{
    DTO_INIT(status_dto, DTO)

    DTO_FIELD_INFO(status)
    {
        info->description = "Short status info";
    }
    DTO_FIELD(String, status);

    DTO_FIELD_INFO(code)
    {
        info->description = "Status code";
    }
    DTO_FIELD(Int32, code);

    DTO_FIELD_INFO(message)
    {
        info->description = "Verbose message";
    }
    DTO_FIELD(String, message);
};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto

