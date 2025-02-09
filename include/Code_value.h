#ifndef _CODE_VALUE_H_
#define _CODE_VALUE_H_

class Code_value{
public:
    Code_value() = default;
    Code_value(int);

    void set_code_value(int);
    int get_code_value() const;

    bool operator==(const Code_value& c) const;

private:
    int code;
};

#endif //_CODE_VALUE_H_