#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/value_at.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/range_c.hpp>
#include "nlohmann/json.hpp"
#include <string>
#include <vector>
#include <type_traits>
#include <iostream>
#include <stdexcept>

// Поддерживаемые типы
template <typename T>
struct IsAllowedType : boost::mpl::bool_<
    std::is_same<T, int>::value ||
    std::is_same<T, float>::value ||
    std::is_same<T, std::string>::value || 
    boost::fusion::traits::is_sequence<T>::value> {};

// Поддерживаются std::vector<T>
template <typename T>
struct IsAllowedType<std::vector<T>> : IsAllowedType<T> {};

// Сериалзация для элементов
template <typename T>
nlohmann::json serialize_element(const T& elem);

// Сериалиация для std::vector
template <typename T>
nlohmann::json serialize_element(const std::vector<T>& vec);

// Десериализация json-строки
template <typename T>
void deserialize_element(const nlohmann::json& j, T& elem);

template <typename T>
void deserialize_element(const nlohmann::json& j, std::vector<T>& vec);

// Проверка всех типов внутри FusionT на этапе компиляции
template <typename FusionT, std::size_t Index = 0>
constexpr bool ValidateFusionTypes() {
    if constexpr (Index < boost::fusion::result_of::size<FusionT>::value) {
        using FieldType = typename boost::fusion::result_of::value_at_c<FusionT, Index>::type;
        return IsAllowedType<std::decay_t<FieldType>>::value &&
               ValidateFusionTypes<FusionT, Index + 1>();
    }
    return true;
}

// Алгоритм сериализации
template <typename FusionT>
std::enable_if_t<ValidateFusionTypes<FusionT>(), nlohmann::json >
serialize(const FusionT& fusion) {
    nlohmann::json j;
    using indices = boost::mpl::range_c<int, 0, boost::fusion::result_of::size<FusionT>::value>;
    boost::mpl::for_each<indices>([&](auto index_wrapper) {
        constexpr int index = decltype(index_wrapper)::value;
        const char* field_name = boost::fusion::extension::struct_member_name<FusionT, index>::call();
        // std::cout << "Processing field: " << field_name << std::endl; 
        const auto& elem = boost::fusion::at_c<index>(fusion);
        j[field_name] = serialize_element(elem);  
    });
    return j;
}

// Случай для сериализации вектора
template <typename T>
nlohmann::json serialize_element(const std::vector<T>& vec) {
    nlohmann::json json_array = nlohmann::json::array();
    for (const auto& item : vec) {
        json_array.push_back(serialize_element(item)); 
    }
    return json_array;
}

// Сериализация невекторных элементов
template <typename T>
nlohmann::json serialize_element(const T& elem) {
    if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, std::string>) {
        return elem;  
    } else if constexpr (boost::fusion::traits::is_sequence<T>::value) {
        return serialize(elem);  
    } else if constexpr (IsAllowedType<T>::value) {
        return serialize_element(elem);  
    } else {
        throw std::runtime_error("Unsupported type for serialization");
    }
}

// Аналогично с десериализацией
template <typename FusionT>
std::enable_if_t<ValidateFusionTypes<FusionT>(), FusionT>
Deserialize(std::string_view json_string) {
    nlohmann::json j = nlohmann::json::parse(json_string);  
    FusionT fusion_obj;
    using indices = boost::mpl::range_c<int, 0, boost::fusion::result_of::size<FusionT>::value>;

    for (auto it = j.begin(); it != j.end(); ++it) {
        const std::string& key = it.key();

        bool key_exists_in_fusion = false;
        boost::mpl::for_each<indices>([&](auto index_wrapper) {
            constexpr int index = decltype(index_wrapper)::value;
            const char* field_name = boost::fusion::extension::struct_member_name<FusionT, index>::call();

            if (key == field_name) {
                key_exists_in_fusion = true;
            }
        });

        if (!key_exists_in_fusion) {
            throw std::runtime_error("Unknown key in JSON: " + key);
        }
    }

    boost::mpl::for_each<indices>([&](auto index_wrapper) {
        constexpr int index = decltype(index_wrapper)::value;
        const char* field_name = boost::fusion::extension::struct_member_name<FusionT, index>::call();

        // std::cout << field_name << std::endl;
        
        // Если нет такого ключа в структуре BOOST_FUSION, то это ошибка
        if (!j.contains(field_name)) {
            throw std::runtime_error(std::string("Missing required field: ") + field_name);
        }

        auto& elem = boost::fusion::at_c<index>(fusion_obj);
        try {
            deserialize_element(j[field_name], elem);  
        } catch (const std::exception& e) {
            throw std::runtime_error("Error deserializing field " + std::string(field_name) + ": " + e.what());
        }
    });
    return fusion_obj;
}

template <typename T>
void deserialize_element(const nlohmann::json& j, T& elem) {
    try {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, std::string>) {
            if (!j.is_primitive()) {
                throw std::runtime_error("JSON type mismatch for primitive field");
            }
            elem = j.get<T>();
        } else if constexpr (boost::fusion::traits::is_sequence<T>::value) {
            elem = Deserialize<T>(j.dump());  // Рекурсивная обработка вложенного Fusion
        } else {
            throw std::runtime_error("Unsupported type for deserialization");
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error deserializing element: " + std::string(e.what()));
    }
}

template <typename T>
void deserialize_element(const nlohmann::json& j, std::vector<T>& vec) {
    if (!j.is_array()) {
        throw std::runtime_error("JSON type mismatch for vector field");
    }
    vec.clear();
    for (const auto& item : j) {
        T elem;
        deserialize_element(item, elem);  
        vec.push_back(elem);
    }
}

