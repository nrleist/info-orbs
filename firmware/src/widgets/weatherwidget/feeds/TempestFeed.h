#ifndef TEMPESTFEED_H
#define TEMPESTFEED_H

#include "../WeatherDataModel.h"
#include "../WeatherFeed.h"
#include <ArduinoJson.h>
#include <TaskManager.h>
#include <string> // Include for std::string

#ifndef TEMPEST_PROXY_URL
    #define TEMPEST_PROXY_URL "https://tempestproxy.lzrk.net/proxy"
#endif

class TempestFeed : public WeatherFeed {
public:
    TempestFeed(const String &apiKey, int units); // Remove stationId and stationName from constructor
    bool getWeatherData(WeatherDataModel &model) override;
    void setupConfig(ConfigManager &config) override; // Implement setupConfig
    void processResponse(int httpCode, const String &response, WeatherDataModel &model);
    void preProcessResponse(int httpCode, String &response);
    String translateIcon(const std::string &icon);

private:
    String apiKey;
    std::string stationId; // Changed to std::string
    std::string stationName; // Changed to std::string
    std::string m_proxyUrl = TEMPEST_PROXY_URL;
    int units;

#ifdef WEATHER_UNITS_METRIC
    int m_weatherUnits = 0;
#else
    int m_weatherUnits = 1;
#endif
};
#endif
