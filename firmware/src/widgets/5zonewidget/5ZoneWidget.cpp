#include "5ZoneWidget.h"
#include "5ZoneTranslations.h"
#include <ArduinoJson.h>

FiveZoneWidget::FiveZoneWidget(ScreenManager &manager, ConfigManager &config) : Widget(manager, config) {

    m_time = GlobalTime::getInstance();

    m_config.addConfigBool("FiveZoneWidget", "5zoEnabled", &m_enabled, t_enableWidget);
    for (int i = 0; i < MAX_ZONES; i++) {
        const char *zoneName = strdup((String("5zoZoneName") + String(i)).c_str());
        const char *zoneDesc = strdup((i18nStr(t_5zoneDesc) + " " + String(i) + ": ").c_str());
        m_config.addConfigString("FiveZoneWidget", zoneName, &m_timeZones[i].locName, 50, zoneDesc, false);

        const char *zoneTZ = strdup((String("5zoZoneInfo") + String(i)).c_str());
        const char *zoneTZDesc = strdup((i18nStr(t_5zoneTZDesc) + " " + String(i) + ": ").c_str());
        m_config.addConfigString("FiveZoneWidget", zoneTZ, &m_timeZones[i].tzInfo, 50, zoneTZDesc, false);

        const char *zoneWorkStart = strdup((String("5zoZoneWstart") + String(i)).c_str());
        const char *zoneWorkStartDesc = strdup((i18nStr(t_5zoneWorkStartDesc) + " " + String(i) + ": ").c_str());
        m_config.addConfigInt("FiveZoneWidget", zoneWorkStart, &m_timeZones[i].m_workStart, zoneWorkStartDesc, false);

        const char *zoneWorkEnd = strdup((String("5zoZoneWend") + String(i)).c_str());
        const char *zoneWorkEndDesc = strdup((i18nStr(t_5zoneWorkEndDesc) + " " + String(i) + ": ").c_str());
        m_config.addConfigInt("FiveZoneWidget", zoneWorkEnd, &m_timeZones[i].m_workEnd, zoneWorkEndDesc, false);
    }
    m_format = m_config.getConfigInt("clockFormat", 0);
    m_type = m_config.getConfigInt("defaultType", 0);
}

void FiveZoneWidget::setup() {
}

void FiveZoneWidget::getTZoneOffset(int8_t zoneIndex) {
    Serial.println("enter updateTZoffset");

    TimeZone &zone = m_timeZones[zoneIndex];
    HTTPClient http;
    http.begin(String(TIMEZONE_API_URL) + "?key=" + TIMEZONE_API_KEY + "&format=json&fields=gmtOffset,zoneEnd&by=zone&zone=" + String(zone.tzInfo.c_str()));

    int httpCode = http.GET();
    if (httpCode > 0) {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, http.getString());
        if (!error) {
            zone.timeZoneOffset = doc["gmtOffset"].as<int>();
            if (doc["zoneEnd"].isNull()) {
                // Timezone does not use DST, no futher updates necessary
                zone.nextTimeZoneUpdate = 0;
            } else {
                // Timezone uses DST, update when necessary
                zone.nextTimeZoneUpdate = doc["zoneEnd"].as<unsigned long>() + random(5 * 60); // Randomize update by 5 minutes to avoid flooding the API
            }
        } else {
            Serial.println("Deserialization error on timezone offset API response");
        }
    } else {
        Serial.println("Failed to get timezone offset from API");
    }
}

void FiveZoneWidget::update(bool force) {
    m_time->updateTime(true);
    int clockStamp = getClockStamp();

    if (clockStamp != m_clockStampU || force) {
        m_clockStampU = clockStamp;
        time_t lv_localEpoch = m_time->getUnixEpoch();

        for (int i = 0; i < MAX_ZONES; i++) {
            TimeZone &zone = m_timeZones[i];
            if (zone.timeZoneOffset == -1 || (zone.nextTimeZoneUpdate > 0 && lv_localEpoch > zone.nextTimeZoneUpdate)) {
                getTZoneOffset(i);
                // small hack because the current TZ API doesn't allow to be called more than once a second
                delay(750);
            }
        }
    }
}

void FiveZoneWidget::changeFormat() {
    GlobalTime *time = GlobalTime::getInstance();
    m_format++;
    if (m_format > 1)
        m_format = 0;
    m_manager.clearAllScreens();
    update(true);
    draw(true);
}

int FiveZoneWidget::getClockStamp() {
    return m_time->getHour24() * 100 + m_time->getMinute();
}

void FiveZoneWidget::draw(bool force) {
    int clockStamp = getClockStamp();

    if (clockStamp != m_clockStampD || force) {
        m_clockStampD = clockStamp;
        for (int i = 0; i < MAX_ZONES; i++) {
            displayZone(i);
        }
    }
}

void FiveZoneWidget::displayZone(int8_t displayIndex) {
    const int nameY = 50; // Zone name at top
    const int dateY = 75; // Date indicator below name
    const int clockY = 115; // Time in middle
    const int ampmY = 175; // AM/PM indicator
    const int offsetY = 200; // Offset at bottom

    m_manager.setFont(DEFAULT_FONT);
    m_manager.selectScreen(displayIndex);

    m_backgroundColor = BG_COLOR;
    m_manager.fillScreen(m_backgroundColor);
    m_foregroundColor = WORK_FG_COLOR;
    m_manager.setFontColor(m_foregroundColor);

    TimeZone &zone = m_timeZones[displayIndex];
    if (zone.locName != "") {
        m_localTimeZone.locName = "Local Time";
        m_localTimeZone.timeZoneOffset = m_time->getTimeZoneOffset();
        m_unixEpoch = m_time->getUnixEpoch();

        time_t lv_unixEpoch = m_unixEpoch + zone.timeZoneOffset - m_localTimeZone.timeZoneOffset;
        int lv_hour = hour(lv_unixEpoch);
        int lv_minute = minute(lv_unixEpoch);
        int lv_day = day(lv_unixEpoch);
        int lv_weekday = weekday(lv_unixEpoch);

        // Calculate offset from local time
        int zoneDiff = zone.timeZoneOffset - m_localTimeZone.timeZoneOffset; // Difference between target UTC offset and local UTC offset
        int hourD = zoneDiff / 3600;
        int minuteD = (zoneDiff / 60) % 60;
        String offsetStr = " ";
        int lv_ringColor;

        if (zoneDiff > 0) {
            offsetStr = "+";
            lv_ringColor = TFT_GREEN;
        } else if (zoneDiff < 0) {
            offsetStr = "-";
            hourD = hourD * -1;
            lv_ringColor = TFT_RED;
        } else
            lv_ringColor = m_backgroundColor;

        offsetStr = offsetStr + ((hourD < 10) ? "0" : "") + String(hourD) +
                    ":" + ((minuteD < 10) ? "0" : "") + String(minuteD);

        m_manager.drawArc(120, 120, 120, 115, 0, 360, lv_ringColor, m_backgroundColor);
        m_manager.drawString(zone.locName.c_str(), ScreenCenterX, nameY, 18, Align::MiddleCenter);

        // Display date indicator if crossing date line
        String dateIndicator = "";
        int lv_localDay = m_time->getDay();
        if (lv_localDay != lv_day) {
            if (lv_unixEpoch > m_unixEpoch)
                dateIndicator = "+1d";
            else
                dateIndicator = "-1d";

            m_manager.drawString(dateIndicator, ScreenCenterX - 60, ampmY, 16, Align::MiddleCenter);
        }
        String lv_displayHour = "";
        String lv_displayAM = "";
        if (m_format == 0)
            lv_displayHour = ((lv_hour < 10) ? "0" : "") + String(lv_hour);
        else {
            lv_displayHour = String(hourFormat12(lv_unixEpoch));
            lv_displayAM = (isAM(lv_unixEpoch)) ? "AM" : "PM";
        }
        m_manager.drawString(lv_displayAM, ScreenCenterX + 60, ampmY, 14, Align::MiddleCenter);
        m_manager.drawString(offsetStr, ScreenCenterX, offsetY, 16, Align::MiddleCenter);

        // if it's a weekend, then use a YELLOW font colour
        // if not, if out of work hours then RED otherwise WHITE
        if (isWeekend(lv_weekday)) {
            m_foregroundColor = WEEKEND_FG_COLOR;
            m_manager.setFontColor(m_foregroundColor);
        } else {
            if (lv_hour < zone.m_workStart || lv_hour >= zone.m_workEnd) {
                m_foregroundColor = AFTER_FG_COLOR;
                m_manager.setFontColor(m_foregroundColor);
            }
        }

        String minuteStr = (lv_minute < 10) ? "0" + String(lv_minute) : String(lv_minute);
        String lv_displayTime = lv_displayHour + ":" + minuteStr;
        m_manager.drawString(lv_displayTime, ScreenCenterX, clockY, 64, Align::MiddleCenter);
    }
}

void FiveZoneWidget ::buttonPressed(uint8_t buttonId, ButtonState state) {
    if (buttonId == BUTTON_OK && state == BTN_MEDIUM) {
        changeFormat();
    }
}

String FiveZoneWidget ::getName() {
    return "5 Zone Clock";
}