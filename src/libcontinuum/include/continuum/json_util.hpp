#ifndef CONTINUUM_LIB_JSON_UTIL_H
#define CONTINUUM_LIB_JSON_UTIL_H

#include <rapidjson/document.h>
#include <continuum/datatypes.hpp>
#include <stdexcept>
#include <unordered_map>

using continuum::Input;
using continuum::InputType;
using continuum::Output;
using continuum::VersionedModelId;
using rapidjson::Type;

// RapidJSON value types
/*
enum Type {
  kNullType = 0,      // null
  kFalseType = 1,     // false
  kTrueType = 2,      // true
  kObjectType = 3,    // object
  kArrayType = 4,     // array
  kStringType = 5,    // string
  kNumberType = 6     // number
}; */
static std::vector<std::string> kTypeNames = {
    "Null", "False", "True", "Object", "Array", "String", "Number"};

namespace continuum {
namespace json {

class json_parse_error : public std::runtime_error {
 public:
  json_parse_error(const std::string& what);
  ~json_parse_error() throw();
};

class json_semantic_error : public std::runtime_error {
 public:
  json_semantic_error(const std::string& what);
  ~json_semantic_error() throw();
};

/* Check for matching types else throw exception */
rapidjson::Value& check_kv_type_and_return(rapidjson::Value& d,
                                           const char* key_name,
                                           Type expected_type);
/**
 * This method is needed because checking boolean fields requires
 * matching against multiple types: rapidjson::kFalseType and
 * rapidjson::kTrueType.
 */
rapidjson::Value& check_kv_type_is_bool_and_return(rapidjson::Value& d,
                                                   const char* key_name);

/* Getters with error handling for bool, double, float, long, int, string */
bool get_bool(rapidjson::Value& d, const char* key_name);

double get_double(rapidjson::Value& d, const char* key_name);

float get_float(rapidjson::Value& d, const char* key_name);

long get_long(rapidjson::Value& d, const char* key_name);

int get_int(rapidjson::Value& d, const char* key_name);

double try_get_double(rapidjson::Value& d, const char* key_name,
                      double default_value);

std::string try_get_string(rapidjson::Value& d, const char* key_name);

std::string get_string(rapidjson::Value& d, const char* key_name);

/* Getters with error handling for arrays of byte, double, float, int, string */

/**
 * Obtains the decoded byte array corresponding to a base64-encoded string
 * representation
 */
std::vector<uint8_t> get_base64_encoded_byte_array(rapidjson::Value& d,
                                                   const char* key_name);

std::vector<double> get_double_array(rapidjson::Value& d, const char* key_name);

std::vector<float> get_float_array(rapidjson::Value& d, const char* key_name);

std::vector<int> get_int_array(rapidjson::Value& d, const char* key_name);

std::vector<std::string> get_string_array(rapidjson::Value& d,
                                          const char* key_name);

std::vector<std::vector<double>> get_double_arrays(rapidjson::Value& d,
                                                   const char* key_name);

std::vector<std::vector<float>> get_float_arrays(rapidjson::Value& d,
                                                 const char* key_name);

std::vector<std::vector<int>> get_int_arrays(rapidjson::Value& d,
                                             const char* key_name);

std::vector<std::vector<uint8_t>> get_base64_encoded_byte_arrays(
    rapidjson::Value& d, const char* key_name);

std::vector<VersionedModelId> get_candidate_models(rapidjson::Value& d,
                                                   const char* key_name);

rapidjson::Value& get_object(rapidjson::Value& d, const char* key_name);

void parse_json(const std::string& json_content, rapidjson::Document& d);

std::vector<std::shared_ptr<Input>> parse_input(InputType input_type,
                                                rapidjson::Value& d);

std::shared_ptr<Input> parse_single_input(InputType input_type,
                                          rapidjson::Value& d);

std::vector<std::shared_ptr<Input>> parse_input_batch(InputType input_type,
                                                      rapidjson::Value& d);

/* Utilities for serialization into JSON */
void add_kv_pair(rapidjson::Document& d, const char* key_name,
                 rapidjson::Value& value_to_add);

void add_bool(rapidjson::Document& d, const char* key_name, bool value_to_add);

void add_double_array(rapidjson::Document& d, const char* key_name,
                      std::vector<double>& values_to_add);

void add_float_array(rapidjson::Document& d, const char* key_name,
                     std::vector<float>& values_to_add);

void add_int_array(rapidjson::Document& d, const char* key_name,
                   std::vector<int>& values_to_add);

void add_string_array(rapidjson::Document& d, const char* key_name,
                      std::vector<std::string>& values_to_add);

void add_json_array(rapidjson::Document& d, const char* key_name,
                    std::vector<std::string>& values_to_add);

void add_double(rapidjson::Document& d, const char* key_name, double val);

void add_float(rapidjson::Document& d, const char* key_name, float val);

void add_int(rapidjson::Document& d, const char* key_name, int val);

void add_long(rapidjson::Document& d, const char* key_name, long val);

void add_string(rapidjson::Document& d, const char* key_name,
                const std::string& val);

void add_object(rapidjson::Document& d, const char* key_name,
                rapidjson::Document& to_add);

std::string to_json_string(rapidjson::Document& d);

/**
 * Sets `d` to an Array type document containing the elements
 * in `string_array`.
 */
void set_string_array(rapidjson::Document& d,
                      const std::vector<std::string>& str_array);
/**
 * Extracts the string vector from the document `d`. Requires
 * that `d` represents an array with only string elements.
 */
std::vector<std::string> to_string_array(rapidjson::Document& d);

/**
 * Sets `d` to the publicly-facing representation of a given Continuum backend.
 * Backend data, provided in `backend_metadata`, is assumed to be pulled from
 * redis and therefore may need to be transformed to comply with desired
 * formatting.
 */
void redis_backend_metadata_to_json(
    rapidjson::Document& d,
    const std::unordered_map<std::string, std::string>& backend_metadata);

/**
 * Sets `d` to the publicly-facing representation of a given Continuum app.
 * App data, provided in `app_metadata`, is assumed to be pulled from redis
 * and therefore may need to be transformed to comply with desired formatting.
 */
void redis_app_metadata_to_json(
    rapidjson::Document& d,
    const std::unordered_map<std::string, std::string>& app_metadata);

/**
 * Sets `d` to the publicly-facing representation of a given Continuum model.
 * Model data, provided in `model_metadata`, is assumed to be pulled from redis
 * and therefore may need to be transformed to comply with desired formatting.
 */
void redis_model_metadata_to_json(
    rapidjson::Document& d,
    const std::unordered_map<std::string, std::string>& model_metadata);

/**
 * Sets `d` to the publicly-facing representation of a given Continuum container.
 * Container data, provided in `container_metadata`, is assumed to be pulled
 * from redis and therefore may need to be transformed to comply with desired
 * formatting.
 */
void redis_container_metadata_to_json(
    rapidjson::Document& d,
    const std::unordered_map<std::string, std::string>& container_metadata);

}  // namespace json
}  // namespace continuum
#endif  // CONTINUUM_LIB_JSON_UTIL_H
