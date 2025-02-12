#include "Code_value.h"

#include <string>

Code_value::Code_value(int code) : code(code) {}

void Code_value::set_code_value(int code)
{
    this->code = code;
}

int Code_value::get_code_value() const
{
    return code;
}

bool Code_value::operator==(const Code_value& c) const
{
    return code == c.code;
}