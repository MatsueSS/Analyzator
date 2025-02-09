#include "Code_value.h"

Code_value::Code_value(int code) : code(code) {}

void Code_value::set_code_value(int code)
{
    this->code = code;
}

int Code_value::get_code_value() const
{
    return code;
}