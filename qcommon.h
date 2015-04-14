#ifndef QCOMMON
#define QCOMMON

struct Interface{
    virtual ~Interface(){}
};

template <typename T>
struct ICloneable:public Interface
{
    virtual T* Clone() = 0;
};

#endif // QCOMMON

