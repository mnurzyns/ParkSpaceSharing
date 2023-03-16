#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

class signUp_dto : public oatpp::DTO {

  DTO_INIT(signUp_dto, DTO)

  DTO_FIELD(String, username, "username");
  DTO_FIELD(String, password, "password");
  DTO_FIELD(String, email,    "email");

};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto