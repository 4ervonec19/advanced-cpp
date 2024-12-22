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

template <typename T>
struct IsAllowedType : boost::mpl::bool_<
    std::is_same<T, int>::value ||
    std::is_same<T, float>::value ||
    std::is_same<T, std::string>::value || 
    boost::fusion::traits::is_sequence<T>::value> {};

template <typename T>
struct IsAllowedType<std::vector<T>> : IsAllowedType<T> {};

// struct TypeChecker {
//     bool& is_valid;  // Ссылка на флаг, который будет изменяться

//     TypeChecker(bool& valid) : is_valid(valid) {}

//     template <typename T>
//     void operator()(const T&) const {
//         is_valid &= IsAllowedType<std::decay_t<T>>::value;
//         // std::cout << is_valid << std::endl;
//     }
// };

// Провекра всех типов на Compile Time
template <typename FusionT, std::size_t Index = 0>
constexpr bool ValidateFusionTypes() {
    if constexpr (Index < boost::fusion::result_of::size<FusionT>::value) {
        using FieldType = typename boost::fusion::result_of::value_at_c<FusionT, Index>::type;
        return IsAllowedType<std::decay_t<FieldType>>::value &&
               ValidateFusionTypes<FusionT, Index + 1>();
    }
    return true;
}

template <typename T>
nlohmann::json serialize_element(const T& elem);

template <typename T>
nlohmann::json serialize_element(const std::vector<T>& vec);

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

template <typename T>
nlohmann::json serialize_element(const std::vector<T>& vec) {
    nlohmann::json json_array = nlohmann::json::array();
    for (const auto& item : vec) {
        json_array.push_back(serialize_element(item));  
    }
    return json_array;
}

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

template <typename T>
void deserialize_element(const nlohmann::json& j, T& elem);

template <typename T>
void deserialize_element(const nlohmann::json& j, std::vector<T>& vec);

template <typename FusionT>
std::enable_if_t<ValidateFusionTypes<FusionT>(), FusionT>
Deserialize(std::string_view json_string) {
    nlohmann::json j = nlohmann::json::parse(json_string);  
    FusionT fusion_obj;
    using indices = boost::mpl::range_c<int, 0, boost::fusion::result_of::size<FusionT>::value>;
    boost::mpl::for_each<indices>([&](auto index_wrapper) {
        constexpr int index = decltype(index_wrapper)::value;
        const char* field_name = boost::fusion::extension::struct_member_name<FusionT, index>::call();
        
        // Если нет такого ключа в структуре BOOST_FUSION, то это ошибка
        if (!j.contains(field_name)) {
            throw std::runtime_error(std::string("Missing required field: ") + field_name);
        }

        auto& elem = boost::fusion::at_c<index>(fusion_obj);
        deserialize_element(j[field_name], elem);  
    });
    return fusion_obj;
}

template <typename T>
void deserialize_element(const nlohmann::json& j, T& elem) {
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


// Пример структуры с разрешёнными типами
BOOST_FUSION_DEFINE_STRUCT((), 
    struct_fusion1,
    (int, id)
    (float, value)
    // (double, doub)
)

// Пример структуры с недопустимыми типами
BOOST_FUSION_DEFINE_STRUCT((), 
    struct_fusion2,
    (int, id)
    (std::string, name)
    (double, double_value) // double не поддерживается
)

// Пример структуры с вложением
BOOST_FUSION_DEFINE_STRUCT((), 
    struct_fusion3,
    (int, id3)
    (std::string, name3)
    (struct_fusion1, s1_instance) 
    (std::vector<struct_fusion1>, s1_vec)
    (int, atacking_potential)
    (std::vector<float>, float_vector)
)

int main() {
    struct_fusion1 s1;
    s1.id = 2;
    s1.value = 2.5;

    struct_fusion1 s1_1{1, 2.2};
    struct_fusion1 s1_2{2, 2.5};

    struct_fusion3 s3;
    s3.id3 = 1;
    s3.name3 = "Chervonikov";
    s3.s1_instance = s1;
    s3.s1_vec = {s1_1, s1_2};
    s3.float_vector = {1, 2, 3, 4};
    s3.atacking_potential = 99;

    auto result = serialize(s3);  
    std::cout << result.dump(4) << std::endl;  

    // std::cout << std::endl;
    // auto result1 = serialize(s1);
    // std::cout << result1.dump(4) << std::endl;

    // std::string res_json = result.dump();
    // // std::cout << res_json << std::endl;

    // struct_fusion3 s3_instance = Deserialize<struct_fusion3>(res_json);

    // std::cout << "id3: " << s3_instance.id3 << std::endl;
    // std::cout << "name3: " << s3_instance.name3 << std::endl;
    // for (const auto& value: s3_instance.float_vector) {
    //     std::cout << value << std::endl;
    // }

    // std::cout << "id from s1: " << s3_instance.s1_instance.id << std::endl;

    // Попробуем json с невалидными ключами
    // "id4 - fail expected"
    std::string json_data = R"({
        "id3": 1, 
        "name3": "Chervonikov",
        "s1_instance": {
            "id": 2,
            "value": 2.5
        },
        "s1_vec": [
            {"id": 1, "value": 2.2},
            {"id": 2, "value": 2.5}
        ],
        "atacking_potential": 99,
        "float_vector": [1.0, 2.0, 3.0, 4.0]
    })";

    auto s3_from_json = Deserialize<struct_fusion3>(json_data);
    // correct runtime error

    auto result_new = serialize(s3_from_json);
    std::cout << result_new.dump(4) << std::endl;
    return 0;
}