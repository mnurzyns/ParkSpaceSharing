#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace server::dto
{

class SignInDto : public oatpp::DTO {

  DTO_INIT(SignInDto, DTO)

  DTO_FIELD(String, login, "login");
  DTO_FIELD(String, password, "password");

};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto