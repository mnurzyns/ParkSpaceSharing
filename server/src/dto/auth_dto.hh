#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

class auth_dto : public oatpp::DTO {

  DTO_INIT(auth_dto, DTO)

  DTO_FIELD(::oatpp::String, token, "token");
  DTO_FIELD(::oatpp::Boolean, admin, "admin");


};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto