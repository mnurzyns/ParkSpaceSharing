#include <string>

namespace poxy
{  

class Proxy {
    public:
        const std::string& getName() const;
    private:
        const std::string _name {"Proxy"};
};

} // namespace poxy