#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace server::database::model {

class UserModel : public oatpp::DTO {

    DTO_INIT(UserModel, DTO)

    DTO_FIELD(UInt64, id);
    DTO_FIELD(String, username);
    DTO_FIELD(String, email);
    DTO_FIELD(String, password);
    DTO_FIELD(Boolean, admin);

};

}  // namespace server::database::model

#include OATPP_CODEGEN_END(DTO)
