#pragma once

class IIdentity
{
public:
    virtual ~IIdentity() = default;
    virtual int GetId() const = 0;
};

template <class T>
bool operator ==(T firstElement, T secondElement) {
    return firstElement.GetId() == secondElement.GetId();
}

template <class T>
bool operator ==(T element, int id) {
    return element.GetId() == id;
}

template <class T>
bool operator >(const T& element, const int id) {
    return element.GetId() > id;
}

template <class T>
std::string VectorToString(std::vector<T> &vector) {
    std::string str = "";
    for(auto &item : vector) {
        str += std::to_string(item.GetId());
        if (item == vector.back()) {
            continue;
        } else {
            str += ",";
        }
    }

    return str;
}

template <class T>
void PrintVector(std::vector<T> &vector) {
    std::cout << VectorToString(vector) << std::endl;
}