#include "boost_fusion.hpp"
#include <cassert>
#include <cmath>

const float epsilon = 1e-6;

// Пример структуры с разрешёнными типами
BOOST_FUSION_DEFINE_STRUCT((), 
    struct_fusion1,
    (int, id)
    (float, value)
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

void BoostFusionSerializeDeserializeTests() {

    // Test 1: Классическая сериализация - десериализация
    struct_fusion1 s1_instance{1, 2.5};

    // Сериализуем в json-строку
    auto result_test1 = serialize(s1_instance);

    // Десериализуем из json-строки
    auto result_from_json_test1 = Deserialize<struct_fusion1>(result_test1.dump());

    // Поля id и value не должны поменяться
    assert(result_from_json_test1.id == s1_instance.id);
    assert(result_from_json_test1.value == s1_instance.value);

    // Test 2: Получение ошибки на Compile-time, связанной с неподдерживаемым типом данных
    static_assert(!ValidateFusionTypes<struct_fusion2>());
    static_assert(ValidateFusionTypes<struct_fusion3>());

    struct_fusion2 s2_instance;
    s2_instance.id = 1;
    s2_instance.name = "cpp-boost-fusion";
    s2_instance.double_value = 2.5;

    // auto result_compile_time_error = serialize(s2_instance); // Ожидается ошибка компиляции 
                                                            // ввиду наличия double

    // Test 3: Сериализация сложной структуры с вложенными структурами и векторами
    struct_fusion3 s3_instance;

    struct_fusion1 s1_1{1, 2.2};
    struct_fusion1 s1_2{2, 2.5};

    s3_instance.id3 = 1;
    s3_instance.name3 = "Chervonikov";
    s3_instance.s1_instance = s1_instance;
    s3_instance.s1_vec = {s1_1, s1_2};
    s3_instance.float_vector = {1, 2, 3, 4};
    s3_instance.atacking_potential = 99;

    auto result_test3 = serialize(s3_instance);
    auto result_from_json_test3 = Deserialize<struct_fusion3>(result_test3.dump());

    // Check. Аналогично первому ожидаем, что ничего не поменяется
    assert(result_from_json_test3.id3 == 1);
    assert(result_from_json_test3.name3 == "Chervonikov");
    assert(result_from_json_test3.atacking_potential == 99);
    assert(result_from_json_test3.s1_instance.id == 1);
    assert(result_from_json_test3.s1_instance.value == 2.5);
    assert(result_from_json_test3.s1_vec[0].id == 1);
    assert(result_from_json_test3.s1_vec[1].id == 2);
    assert(std::fabs(result_from_json_test3.s1_vec[0].value - 2.2) < epsilon);  
    assert(result_from_json_test3.s1_vec[1].value == 2.5);
    assert(result_from_json_test3.float_vector[0] == 1);
    assert(result_from_json_test3.float_vector[1]  == 2);
    assert(result_from_json_test3.float_vector[2]  == 3);
    assert(result_from_json_test3.float_vector[3] == 4);

    // Test 4: Десериализация из строки
    std::string json_data = R"({
        "id3": 12, 
        "name3": "Chervonikov Alexey",
        "s1_instance": {
            "id": 20,
            "value": 2.5
        },
        "s1_vec": [
            {"id": 99, "value": 1.9},
            {"id": 101, "value": 2.5}
        ],
        "atacking_potential": 77,
        "float_vector": [2.0, -2.0, 3.0, 64.0]
    })";

    auto result_from_json_test4 = Deserialize<struct_fusion3>(json_data);

    assert(result_from_json_test4.id3 == 12);
    assert(result_from_json_test4.name3 == "Chervonikov Alexey");
    assert(result_from_json_test4.atacking_potential == 77);
    assert(result_from_json_test4.s1_instance.id == 20);
    assert(result_from_json_test4.s1_instance.value == 2.5);
    assert(result_from_json_test4.s1_vec[0].id == 99);
    assert(result_from_json_test4.s1_vec[1].id == 101);
    assert(std::fabs(result_from_json_test4.s1_vec[0].value - 1.9) < epsilon);  
    assert(result_from_json_test4.s1_vec[1].value == 2.5);
    assert(result_from_json_test4.float_vector[0] == 2.0);
    assert(result_from_json_test4.float_vector[1]  == -2.0);
    assert(result_from_json_test4.float_vector[2]  == 3.0);
    assert(result_from_json_test4.float_vector[3] == 64.0);

    // Test 5: Получение ошибки отсутвия поля при десериализации
    // Пусть отсутствует поле id3
    std::string json_data_fault = R"({
        "name3": "Chervonikov Alexey",
        "s1_instance": {
            "id": 20,
            "value": 2.5
        },
        "s1_vec": [
            {"id": 99, "value": 1.9},
            {"id": 101, "value": 2.5}
        ],
        "atacking_potential": 77,
        "float_vector": [2.0, -2.0, 3.0, 64.0]
    })";

    try {
        auto result_from_json_test5 = Deserialize<struct_fusion3>(json_data_fault);
    } catch (const std::runtime_error& e) {
        assert(std::string(e.what()) == "Missing required field: id3");
        // std::cout << "Exception caught: " << e.what() << std::endl;
    }

    // Test6: Получение ошибки обработки несуществующего поля при десериализации
    // поле float_vecto 
    std::string json_data_fault_new = R"({
        "id3": 12, 
        "name3": "Chervonikov Alexey",
        "s1_instance": {
            "id": 20,
            "value": 2.5
        },
        "s1_vec": [
            {"id": 99, "value": 1.9},
            {"id": 101, "value": 2.5}
        ],
        "atacking_potential": 77,
        "float_vector": [2.0, -2.0, 3.0, 64.0],
        "float_vecto": [2.0, -2.0, 3.0, 64.0]
    })";

    try {
        auto result_from_json_test6 = Deserialize<struct_fusion3>(json_data_fault_new);
    } catch (const std::runtime_error& e) {
        assert(std::string(e.what()) == "Unknown key in JSON: float_vecto");
        // std::cout << "Exception caught: " << e.what() << std::endl;
    }

    std::cout << "All tests passed succesfully!" << std::endl;

}

int main() {
    // Тестируем...
    BoostFusionSerializeDeserializeTests();
    return 0;
}