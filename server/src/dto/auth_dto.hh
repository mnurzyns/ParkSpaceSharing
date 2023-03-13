#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

class auth_dto : public oatpp::DTO {

  DTO_INIT(auth_dto, DTO)

  DTO_FIELD(::oatpp::String, token);
  //DTO_FIELD(String, expire_date);

};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto