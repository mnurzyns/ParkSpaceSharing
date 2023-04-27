#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace server::dto {

    template<class T>
    class PageDto : public oatpp::DTO {
        DTO_INIT(PageDto, DTO)

        DTO_FIELD(UInt64, offset);

        DTO_FIELD(UInt64, limit);

        DTO_FIELD(UInt64, count);

        DTO_FIELD(Vector<T>, items);
    };

} // namespace server::dto

#include OATPP_CODEGEN_END(DTO)
