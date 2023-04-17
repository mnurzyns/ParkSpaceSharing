#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

class parkingSpace_dto : public oatpp::DTO {

  DTO_INIT(parkingSpace_dto, DTO)

  DTO_FIELD(oatpp::UInt32, id, "id");
  DTO_FIELD(oatpp::UInt32, owner_id, "owner_id");
  DTO_FIELD(oatpp::String, location, "location");

  DTO_FIELD_INFO(location)
  {
    info->description = "pole with client and server use to sent and read parkingSpace";
  }

  DTO_FIELD_INFO(owner_id)
  {
    info->description = "pole with client and server use to sent and read only in specific endpoits";
  }

  DTO_FIELD_INFO(id)
  {
    info->description = "pole with server use to sent and client read";
  }  
  

};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto