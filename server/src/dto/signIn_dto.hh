#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

class signIn_dto : public oatpp::DTO {

  DTO_INIT(signIn_dto, DTO)

  DTO_FIELD(String, username, "username");
  DTO_FIELD(String, password, "password");

};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto