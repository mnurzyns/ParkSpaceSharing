#include <string>

namespace example {

class proxy {
public:
    const std::string& getName() const;
private:
    const std::string _name {"Proxy"};
};

}