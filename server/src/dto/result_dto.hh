#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

class result_dto : public oatpp::DTO {

  DTO_INIT(result_dto, DTO)

  DTO_FIELD(oatpp::UInt32, id_result, "id_result");

};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto