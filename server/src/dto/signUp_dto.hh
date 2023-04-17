#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

class signUp_dto : public oatpp::DTO {

  DTO_INIT(signUp_dto, DTO)

  DTO_FIELD(oatpp::String, username, "username");
  DTO_FIELD(oatpp::String, password, "password");
  DTO_FIELD(oatpp::String, email,    "email");

};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto