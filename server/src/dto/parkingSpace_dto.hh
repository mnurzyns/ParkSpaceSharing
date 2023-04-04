#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

class parkingSpace_dto : public oatpp::DTO {

  DTO_INIT(parkingSpace_dto, DTO)

  DTO_FIELD(UInt32, user_id,  "owner_id");
  DTO_FIELD(String, location, "location");

};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto