#include "Serializer.hpp"
#include "Data.hpp"
#include <iostream>

int main() {
    Data data;
    data.value = 42;
    
    std::cout << "Original pointer: " << &data << std::endl;
    std::cout << "Original value: " << data.value << std::endl;
    
    uintptr_t raw = Serializer::serialize(&data);
    std::cout << "Serialized: " << raw << std::endl;
    
    Data* ptr = Serializer::deserialize(raw);
    std::cout << "Deserialized pointer: " << ptr << std::endl;
    std::cout << "Deserialized value: " << ptr->value << std::endl;
    
    if (ptr == &data)
        std::cout << "Pointers are equal!" << std::endl;
    else
        std::cout << "Pointers are NOT equal!" << std::endl;
    
    return 0;
}
