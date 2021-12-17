#include "Options.hpp"

#include <fstream>

Config g_Options;
ConfigScheme g_ConfigScheme{ std::map<std::string, Config>{
    {"<Default>", g_Options}
} };
bool g_Unload = false;

// These 2 functions are from here:
// http://stackoverflow.com/questions/277772/avoid-trailing-zeroes-in-printf
void morphNumericString(char *s, int n)
{
    char *p;
    int count;

    p = strchr(s, '.');         // Find decimal point, if any.
    if(p != NULL) {
        count = n;              // Adjust for more or less decimals.
        while(count >= 0) {    // Maximum decimals allowed.
            count--;
            if(*p == '\0')    // If there's less than desired.
                break;
            p++;               // Next character.
        }

        *p-- = '\0';            // Truncate string.
        while(*p == '0')       // Remove trailing zeros.
            *p-- = '\0';

        if(*p == '.') {        // If all decimals were zeros, remove ".".
            *p = '\0';
        }
    }
}
template<size_t N>
void nDecimals(char(&s)[N], double d, int n)
{
    int sz; double d2;

    // Allow for negative.

    d2 = (d >= 0) ? d : -d;
    sz = (d >= 0) ? 0 : 1;

    // Add one for each whole digit (0.xx special case).

    if(d2 < 1) sz++;
    while(d2 >= 1) { d2 /= 10.0; sz++; }

    // Adjust for decimal point and fractionals.

    sz += 1 + n;

    // Create format string then use it.

    sprintf_s(s, N, "%*.*f", sz, n, d);
}
// ---------------------------------------------

// ---------------------------------------------
Option::Option(std::string name, std::string value)
    : m_szName(name)
{    
    if(value[0] == '#') {
        set_color(Color(strtoul(value.data() + 1, nullptr, 16)));
    } else {
        set_float((float)atof(value.data()));
    }
}
// ---------------------------------------------
Option::~Option()
{
}
// ---------------------------------------------
void Option::set_float(float val)
{
    char tempVal[32] = "0";

    snprintf(tempVal, sizeof(tempVal), "%f", val);

    set_string(tempVal);
}
// ---------------------------------------------
void Option::set_int(int val)
{
    char tempVal[32];

    snprintf(tempVal, sizeof(tempVal), "%d", val);

    set_string(tempVal);
}
// ---------------------------------------------
void Option::set_bool(bool val)
{
    set_int((int)val);
}
// ---------------------------------------------
void Option::set_string(const char* val)
{
    if(val[0] == '#' && strlen(val) == 9) {
        m_Value.szString = val;
        m_Value.nValue = (int)strtoul(m_Value.szString.data() + 1, nullptr, 16);
        m_Value.clrValue = Color(m_Value.nValue);
        m_Value.fValue = std::numeric_limits<float>::max();
    } else {
        char temp[32];
        m_Value.fValue = (float)atof(val);
        m_Value.nValue = (int)m_Value.fValue;
        m_Value.clrValue = Color(m_Value.nValue);

        nDecimals(temp, m_Value.fValue, 4);
        morphNumericString(temp, 3);
        m_Value.szString = temp;
    }
}
// ---------------------------------------------
void Option::set_color(const Color& val)
{
    char tempVal[32];

    snprintf(tempVal,
        sizeof(tempVal),
        "#%02X%02X%02X%02X",
        static_cast<int>(val.a()),
        static_cast<int>(val.r()),
        static_cast<int>(val.g()),
        static_cast<int>(val.b()));

    set_string(tempVal);
}
// ---------------------------------------------

ConfigScheme::ConfigScheme(std::map<std::string, Config> default)
{
    current_config_name = "<Default>";
    config_list = std::move(default);
    config_names.push_back(current_config_name);
    current_config_index = 0;
}

// ---------------------------------------------
bool ConfigScheme::save_config()
{
    std::ofstream stream{ json_file };

    if(!stream.is_open() || !stream.good() || stream.eof())
        return false;
    try {
        nlohmann::json root;

        for(const auto& pair : config_list) {
            nlohmann::json json = pair.second;
            root["configs"][pair.first] = json;
        }

        root["default_config_name"] = current_config_name;

        stream << root.dump(2) << std::endl;

        return true;
    } catch(...) {
        return false;
    }
}
// ---------------------------------------------
bool ConfigScheme::load_config(int idx)
{
    std::ifstream stream{ json_file };
    
    if(!stream.is_open() || !stream.good() || stream.eof())
        return false;

    try {
        nlohmann::json root;

        stream >> root;

        const auto& configs = root.at("configs");
        if(idx == -1) {
            config_list.clear();
            config_names.clear();

            for(auto it = configs.begin(); it != configs.end(); ++it) {
                config_list.emplace(it.key(), it->get<Config>());
                config_names.emplace_back(it.key());
            }

            current_config_name = root.at("default_config_name").get<std::string>();

            auto it = std::find(config_names.begin(), config_names.end(), current_config_name);
            if(it != config_names.end()) {
                current_config_index = std::distance(config_names.begin(), it);
            }

            set_active_config(current_config_index);

        } else {
            if(config_exists(idx)) {
                const auto& name = config_names[idx];
                for(auto it = configs.begin(); it != configs.end(); ++it) {
                    if(name == it.key()) {
                        config_list[name] = it->get<Config>();
                    }
                }
            }
        }
        return true;
    } catch(...) {
        return false;
    }
}
// ---------------------------------------------
bool ConfigScheme::config_exists(int idx)
{
    return (int)config_names.size() > idx;
}
// ---------------------------------------------
void ConfigScheme::set_active_config(int idx)
{
    if(!config_exists(idx))
        throw std::runtime_error{ "Profile does not exists" };

    current_config_name = config_names[idx];
    g_Options = config_list[current_config_name];
}
// ---------------------------------------------
bool ConfigScheme::save_current_config()
{
    config_list[config_names[current_config_index]] = g_Options;

    return true;
}
// ---------------------------------------------
int ConfigScheme::add_config(const std::string& name)
{
    config_names.push_back(name);
    config_list[name] = g_Options;

    return (int)config_names.size() - 1;
}
// ---------------------------------------------
void ConfigScheme::remove_config(const std::string& name)
{
    if(name == "<Default>")
        return;

    config_names.erase(std::find(config_names.begin(), config_names.end(), name));
    config_list.erase(name);
}
// ---------------------------------------------
bool ConfigScheme::item_getter(void* data, int i, const char** name)
{
    ConfigScheme* p = (ConfigScheme*)data;

    if(i >= 0 && i < (int)p->config_names.size()) {
        *name = p->config_names[i].data();
        return true;
    }

    return false;
}
// ---------------------------------------------

// ---------------------------------------------
void to_json(nlohmann::json& json, const Config& p)
{
    Option* cur = (Option*)&p;
    Option* end = (Option*)((uint8_t*)&p + sizeof(Config));

    while((uint8_t*)cur < (uint8_t*)end) {
        json[cur->get_name()] = cur->get_string();

        cur++;
    }
}
// ---------------------------------------------
void from_json(const nlohmann::json& json, Config& p)
{
    Option* cur = (Option*)&p;
    Option* end = (Option*)((uint8_t*)&p + sizeof(Config));

    while((uint8_t*)cur < (uint8_t*)end) {
        auto name = cur->get_name();
        try {
            auto value = json.at(name).get<std::string>();
            if(value[0] == '#') {
                cur->set_color(Color(strtoul(value.data() + 1, nullptr, 16)));
            } else {
                cur->set_float((float)atof(value.data()));
            }
        } catch(const std::out_of_range&) {

        }
        cur++;
    }
}
// ---------------------------------------------