# Developer Guide for Using ArduinoLog: Log Methods and LOG_LEVEL

When using the `ArduinoLog` library, the logging methods correspond to different log levels, which help you control the verbosity of your logging output. The log levels are hierarchical, meaning that enabling a specific log level will also enable all log levels above it.

## Log Levels and Methods

1. **`LOG_LEVEL_SILENT`**:

   - No logging output.
   - No corresponding `Log.` method.

2. **`LOG_LEVEL_FATAL`**:

   - Only the most critical errors.
   - Corresponds to `Log.fatal()`.
   - Example: `Log.fatal("This is a fatal error");`

3. **`LOG_LEVEL_ERROR`**:

   - Critical errors.
   - Corresponds to `Log.error()`.
   - Example: `Log.error("This is an error");`

4. **`LOG_LEVEL_WARNING`**:

   - Warnings and errors.
   - Corresponds to `Log.warning()`.
   - Example: `Log.warning("This is a warning");`

5. **`LOG_LEVEL_INFO`**:

   - Informational messages, warnings, and errors.
   - Corresponds to `Log.info()`.
   - Example: `Log.info("This is an info message");`

6. **`LOG_LEVEL_NOTICE`**:

   - Notice messages, informational messages, warnings, and errors.
   - Corresponds to `Log.notice()`.
   - Example: `Log.notice("This is a notice");`

7. **`LOG_LEVEL_TRACE`**:

   - Detailed tracing information, notices, informational messages, warnings, and errors.
   - Corresponds to `Log.trace()`.
   - Example: `Log.trace("This is a trace message");`

8. **`LOG_LEVEL_VERBOSE`**:
   - Very detailed tracing information, traces, notices, informational messages, warnings, and errors.
   - Corresponds to `Log.verbose()`.
   - Example: `Log.verbose("This is a verbose message");`

## Setting the Log Level

You can set the log level globally in your `config.h` file. For example:

```cpp
#define LOG_LEVEL LOG_LEVEL_INFO
```

To use logging in your code, include the following:

```cpp
#include <ArduinoLog.h>
```

### Examples

```cpp
Log.info("This is an info message.");
Log.warning("Warning: Device is overheating!");
Log.error("Error: Sensor not found.");
Log.fatal("Fatal: System crash imminent!");
Log.info("Sensor value: " + String(sensorValue));

Log.info("Count: %d", count);
Log.info("Status: %s, Value: %d", status, value);
Log.warning("Temperature: %.2f°C", temperature);
Log.error("Error %d: %s", errorCode, errorMessage);
Log.trace("Loop iteration: %d", loopCount);

Log.info("Device: " + String(deviceName) + ", ID: %d", deviceId);
Log.warning("Battery level: %d%%, Status: %s", batteryLevel, status);
```

## General Guidance

1. Opt for using `Log` instead of `Serial.print` where possible.
2. Keep `INFO` log messages limited and concise to avoid cluttering the serial monitor with messages that may not be meaningful.
3. Use `NOTICE` log messages for events that occur less frequently but are still important.
4. Use `WARNING` log messages for unexpected occurrences during execution that may not cause immediate issues (e.g., "Only 4 stock symbols defined").
5. Use `TRACE` log messages for debugging purposes, such as tracking execution flow or dumping JSON returned from an API call.
6. Use `WARNING` for less critical errors, `ERROR` for more critical errors, and `FATAL` only for the most critical errors.
