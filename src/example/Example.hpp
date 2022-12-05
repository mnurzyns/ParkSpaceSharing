#include <string>

namespace example {

class Example {
public:
    const std::string& getName() const;
private:
    const std::string _name {"Example"};
};

}