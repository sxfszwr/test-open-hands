/**
 * @file main_example_en.c
 * @brief OverspeedWarning module usage example (English version)
 * @version 1.0
 * @date 2025-12-02
 * 
 * Demonstrates how to use the OverspeedWarning module for speed monitoring
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "overspeed_warning.h"

/* Simulate 10ms timer */
#define CYCLE_TIME_MS 10

/* Error status code descriptions */
const char* get_error_description(uint8_t error_code) {
    switch (error_code) {
        case OSW_ERROR_NONE:
            return "Normal";
        case OSW_ERROR_SPEED_NAN:
            return "Speed signal is NaN";
        case OSW_ERROR_SPEED_RANGE:
            return "Speed out of valid range";
        case OSW_ERROR_LIMIT_INVALID:
            return "Speed limit invalid";
        case OSW_ERROR_LIMIT_RANGE:
            return "Speed limit out of config range";
        default:
            return "Unknown error";
    }
}

/* Print module status */
void print_module_status(float vehicle_speed, float speed_limit, int cycle_count) {
    bool alarm_status = OSW_GetAlarmStatus();
    uint8_t error_status = OSW_GetErrorStatus();
    
    printf("Cycle %3d: Speed=%.1f km/h, Limit=%.1f km/h, Alarm=%s, Error=%s\n",
           cycle_count,
           vehicle_speed,
           speed_limit,
           alarm_status ? "YES" : "NO",
           get_error_description(error_status));
}

int main(void) {
    printf("=== OverspeedWarning Module Test Example ===\n\n");
    
    /* 1. Module initialization */
    printf("1. Initializing module...\n");
    OSW_Init();
    printf("   Default speed limit: %.1f km/h\n\n", OSW_DEFAULT_LIMIT);
    
    /* 2. Normal operation scenario test */
    printf("2. Normal operation scenario test:\n");
    float test_speeds[] = {60.0f, 75.0f, 85.0f, 90.0f, 70.0f};
    float speed_limit = 80.0f;
    
    for (int i = 0; i < 5; i++) {
        OSW_MainFunction(test_speeds[i], speed_limit);
        print_module_status(test_speeds[i], speed_limit, i + 1);
        usleep(CYCLE_TIME_MS * 1000); /* Simulate 10ms delay */
    }
    
    printf("\n");
    
    /* 3. Dynamic speed limit adjustment test */
    printf("3. Dynamic speed limit adjustment test:\n");
    printf("   Setting new speed limit to 100.0 km/h\n");
    if (OSW_SetSpeedLimit(100.0f)) {
        printf("   Speed limit setting successful\n");
    } else {
        printf("   Speed limit setting failed\n");
    }
    
    OSW_MainFunction(90.0f, 100.0f);
    print_module_status(90.0f, 100.0f, 6);
    
    printf("\n");
    
    /* 4. Abnormal input test */
    printf("4. Abnormal input test:\n");
    
    /* Test speed NaN */
    printf("   Testing speed NaN:\n");
    OSW_MainFunction(NAN, 80.0f);
    print_module_status(NAN, 80.0f, 7);
    
    /* Test speed out of range */
    printf("   Testing speed out of range (350 km/h):\n");
    OSW_MainFunction(350.0f, 80.0f);
    print_module_status(350.0f, 80.0f, 8);
    
    /* Test invalid speed limit */
    printf("   Testing invalid speed limit (0 km/h):\n");
    OSW_MainFunction(60.0f, 0.0f);
    print_module_status(60.0f, 0.0f, 9);
    
    /* Test speed limit out of range */
    printf("   Testing speed limit out of range (150 km/h):\n");
    OSW_MainFunction(60.0f, 150.0f);
    print_module_status(60.0f, 150.0f, 10);
    
    printf("\n");
    
    /* 5. Recovery to normal input test */
    printf("5. Recovery to normal input test:\n");
    OSW_MainFunction(75.0f, 80.0f);
    print_module_status(75.0f, 80.0f, 11);
    
    printf("\n=== Test Complete ===\n");
    
    return 0;
}