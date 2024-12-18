//
// Created by ginko on 16/04/24.
//
#pragma once
// TODO This should be in resources dir

constexpr char const *configPath{"config.toml"};
constexpr size_t preStoredValues{20};

class Config {
    static toml::table config;
    static std::array<int, preStoredValues> storedInts;
    static std::array<float, preStoredValues> storedFloats;
    static std::array<bool, preStoredValues> storedBools;
    static int storedIntsIndex;
    static int storedFloatsIndex;
    static int storedBoolsIndex;
public:
    static std::unordered_map<std::string, int> indexDictInts; // TODO make them private and use getters to const ref
    static std::unordered_map<std::string, int> indexDictFloats;
    static std::unordered_map<std::string, int> indexDictBools;

    static toml::table writeTable();

    static void SaveAttributeParameters();

    static void LoadAttributeParameters();

    static int GetInt(const std::string &name);

    static int *GetIntPtr(const std::string &name);

    static void addFloat(const std::string &name);

    static float GetFloat(const std::string &name);

    static float *GetFloatPtr(const std::string &name);

    static void addBool(const std::string &name);

    static bool GetBool(const std::string &name);

    static bool *GetBoolPtr(const std::string &name);

    static void addInt(const std::string &name);


    static void Instantiate();


    //    ~Parameters destroy unloads all
};
