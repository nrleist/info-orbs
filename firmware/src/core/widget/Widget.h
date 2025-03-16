#ifndef WIDGET_H
#define WIDGET_H

#include "Button.h"
#include "ConfigManager.h"
#include "ScreenManager.h"
#include "Translations.h" // include for use by all Widgets
#include "config_helper.h"

class WidgetTimer;

enum class TimeFrequency : unsigned long {
    Immediate = 0,
    OneHundredMilliseconds = 100,
    OneSecond = 1000,
    TwoSeconds = 2000,
    FiveSeconds = 5000,
    TenSeconds = 10000,
    ThirtySeconds = 30000,
    OneMinute = 60000,
    FiveMinutes = 300000,
    TenMinutes = 600000,
    FifteenMinutes = 900000,
    ThirtyMinutes = 1800000,
    OneHour = 3600000,
    OneDay = 86400000,
    OneWeek = 604800000
};

class Widget {
public:
    Widget(ScreenManager &manager, ConfigManager &config);
    virtual ~Widget() = default;
    virtual void setup() = 0;
    virtual void update(bool force = false) = 0;
    virtual void draw(bool force = false) = 0;
    virtual void buttonPressed(uint8_t buttonId, ButtonState state) = 0;
    virtual String getName() = 0;

    WidgetTimer &addDrawRefreshFrequency(TimeFrequency frequency);
    WidgetTimer &addUpdateRefreshFrequency(TimeFrequency frequency);
    void resetTimer(WidgetTimer &timer);

    void setBusy(bool busy);
    bool isEnabled();

    bool isItTimeToDraw();
    bool isItTimeToUpdate();

    // New methods to get last trigger times
    unsigned long getLastDrawTime() const; // Time of last draw trigger
    unsigned long getLastUpdateTime() const; // Time of last update trigger

protected:
    ScreenManager &m_manager;
    ConfigManager &m_config;
    bool m_enabled = false;

    WidgetTimer *m_drawTimer = nullptr;
    WidgetTimer *m_updateTimer = nullptr;
};

class WidgetTimer {
public:
    WidgetTimer(uint32_t interval) : m_interval(interval), m_previousMillis(0) {}

    bool isDue() {
        unsigned long currentMillis = millis();
        if (currentMillis - m_previousMillis >= m_interval) {
            m_previousMillis = currentMillis;
            return true;
        }
        return false;
    }

    void reset() {
        m_previousMillis = millis();
    }

    uint32_t getInterval() const { return m_interval; }
    void setInterval(uint32_t interval) { m_interval = interval; }

private:
    uint32_t m_interval;
    unsigned long m_previousMillis;
};

#endif // WIDGET_H
