/**
 * @file unit_test_en.c
 * @brief Unit tests for OverspeedWarning module (English version)
 * @version 1.0
 * @date 2025-12-02
 * 
 * Comprehensive unit tests for all sub-modules
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "overspeed_warning.h"

/* Test counters */
static int test_count = 0;
static int test_passed = 0;

/* Test macro */
#define TEST_ASSERT(condition, description) \
    do { \
        test_count++; \
        if (condition) { \
            test_passed++; \
            printf("✓ Test %d: %s - PASSED\n", test_count, description); \
        } else { \
            printf("✗ Test %d: %s - FAILED\n", test_count, description); \
        } \
    } while(0)

/* Test InputValidation sub-module */
void test_input_validation(void) {
    printf("\n=== InputValidation Sub-module Tests ===\n");
    
    bool inputs_valid;
    uint8_t validation_error;
    
    /* Test normal input */
    InputValidation_CheckInputs(60.0f, 80.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == true && validation_error == OSW_ERROR_NONE, 
                "Normal input validation");
    
    /* Test speed NaN */
    InputValidation_CheckInputs(NAN, 80.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_SPEED_NAN, 
                "Speed NaN detection");
    
    /* Test negative speed */
    InputValidation_CheckInputs(-10.0f, 80.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_SPEED_RANGE, 
                "Negative speed detection");
    
    /* Test speed over limit */
    InputValidation_CheckInputs(350.0f, 80.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_SPEED_RANGE, 
                "Speed over limit detection");
    
    /* Test speed limit NaN */
    InputValidation_CheckInputs(60.0f, NAN, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_LIMIT_INVALID, 
                "Speed limit NaN detection");
    
    /* Test speed limit zero */
    InputValidation_CheckInputs(60.0f, 0.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_LIMIT_INVALID, 
                "Speed limit zero detection");
    
    /* Test negative speed limit */
    InputValidation_CheckInputs(60.0f, -20.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_LIMIT_INVALID, 
                "Negative speed limit detection");
    
    /* Test speed limit below minimum */
    InputValidation_CheckInputs(60.0f, 20.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_LIMIT_RANGE, 
                "Speed limit below minimum detection");
    
    /* Test speed limit above maximum */
    InputValidation_CheckInputs(60.0f, 150.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_LIMIT_RANGE, 
                "Speed limit above maximum detection");
    
    /* Test boundary values */
    InputValidation_CheckInputs(0.0f, 30.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == true && validation_error == OSW_ERROR_NONE, 
                "Speed lower boundary test");
    
    InputValidation_CheckInputs(300.0f, 120.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == true && validation_error == OSW_ERROR_NONE, 
                "Speed upper boundary test");
}

/* Test SpeedComparison sub-module */
void test_speed_comparison(void) {
    printf("\n=== SpeedComparison Sub-module Tests ===\n");
    
    bool speed_exceeded;
    
    /* Test speed below limit */
    SpeedComparison_CompareSpeed(70.0f, 80.0f, true, &speed_exceeded);
    TEST_ASSERT(speed_exceeded == false, "Speed below limit");
    
    /* Test speed equals limit */
    SpeedComparison_CompareSpeed(80.0f, 80.0f, true, &speed_exceeded);
    TEST_ASSERT(speed_exceeded == false, "Speed equals limit");
    
    /* Test speed above limit */
    SpeedComparison_CompareSpeed(90.0f, 80.0f, true, &speed_exceeded);
    TEST_ASSERT(speed_exceeded == true, "Speed above limit");
    
    /* Test invalid input handling */
    SpeedComparison_CompareSpeed(90.0f, 80.0f, false, &speed_exceeded);
    TEST_ASSERT(speed_exceeded == false, "Invalid input handling");
    
    /* Test boundary case */
    SpeedComparison_CompareSpeed(80.1f, 80.0f, true, &speed_exceeded);
    TEST_ASSERT(speed_exceeded == true, "Slight overspeed detection");
}

/* Test AlarmGeneration sub-module */
void test_alarm_generation(void) {
    printf("\n=== AlarmGeneration Sub-module Tests ===\n");
    
    bool alarm_status;
    
    /* Test alarm generation on overspeed */
    AlarmGeneration_UpdateAlarm(true, true, &alarm_status);
    TEST_ASSERT(alarm_status == true, "Alarm on overspeed");
    
    /* Test no alarm when not overspeeding */
    AlarmGeneration_UpdateAlarm(false, true, &alarm_status);
    TEST_ASSERT(alarm_status == false, "No alarm when not overspeeding");
    
    /* Test forced no alarm on invalid input */
    AlarmGeneration_UpdateAlarm(true, false, &alarm_status);
    TEST_ASSERT(alarm_status == false, "Forced no alarm on invalid input");
    
    /* Test no alarm on invalid input and no overspeed */
    AlarmGeneration_UpdateAlarm(false, false, &alarm_status);
    TEST_ASSERT(alarm_status == false, "No alarm on invalid input and no overspeed");
}

/* Test ErrorHandler sub-module */
void test_error_handler(void) {
    printf("\n=== ErrorHandler Sub-module Tests ===\n");
    
    uint8_t error_status;
    
    /* Test error clearing on valid input */
    ErrorHandler_UpdateStatus(OSW_ERROR_SPEED_NAN, true, &error_status);
    TEST_ASSERT(error_status == OSW_ERROR_NONE, "Error clearing on valid input");
    
    /* Test error status setting on invalid input */
    ErrorHandler_UpdateStatus(OSW_ERROR_SPEED_NAN, false, &error_status);
    TEST_ASSERT(error_status == OSW_ERROR_SPEED_NAN, "Speed NaN error status");
    
    ErrorHandler_UpdateStatus(OSW_ERROR_SPEED_RANGE, false, &error_status);
    TEST_ASSERT(error_status == OSW_ERROR_SPEED_RANGE, "Speed range error status");
    
    ErrorHandler_UpdateStatus(OSW_ERROR_LIMIT_INVALID, false, &error_status);
    TEST_ASSERT(error_status == OSW_ERROR_LIMIT_INVALID, "Limit invalid error status");
    
    ErrorHandler_UpdateStatus(OSW_ERROR_LIMIT_RANGE, false, &error_status);
    TEST_ASSERT(error_status == OSW_ERROR_LIMIT_RANGE, "Limit range error status");
}

/* Test main module integration */
void test_main_module(void) {
    printf("\n=== Main Module Integration Tests ===\n");
    
    /* Initialization test */
    OSW_Init();
    TEST_ASSERT(OSW_GetAlarmStatus() == false, "Initialization alarm status false");
    TEST_ASSERT(OSW_GetErrorStatus() == OSW_ERROR_NONE, "Initialization error status normal");
    
    /* Normal operation test */
    OSW_MainFunction(70.0f, 80.0f);
    TEST_ASSERT(OSW_GetAlarmStatus() == false, "Normal speed no alarm");
    TEST_ASSERT(OSW_GetErrorStatus() == OSW_ERROR_NONE, "Normal input no error");
    
    /* Overspeed alarm test */
    OSW_MainFunction(90.0f, 80.0f);
    TEST_ASSERT(OSW_GetAlarmStatus() == true, "Overspeed alarm generation");
    TEST_ASSERT(OSW_GetErrorStatus() == OSW_ERROR_NONE, "Overspeed no error status");
    
    /* Invalid input test */
    OSW_MainFunction(NAN, 80.0f);
    TEST_ASSERT(OSW_GetAlarmStatus() == false, "Invalid input no alarm");
    TEST_ASSERT(OSW_GetErrorStatus() == OSW_ERROR_SPEED_NAN, "Speed NaN error detection");
    
    /* Recovery test */
    OSW_MainFunction(75.0f, 80.0f);
    TEST_ASSERT(OSW_GetAlarmStatus() == false, "Recovery no alarm");
    TEST_ASSERT(OSW_GetErrorStatus() == OSW_ERROR_NONE, "Recovery no error");
    
    /* Dynamic configuration test */
    TEST_ASSERT(OSW_SetSpeedLimit(100.0f) == true, "Valid speed limit setting success");
    TEST_ASSERT(OSW_SetSpeedLimit(200.0f) == false, "Invalid speed limit setting failure");
    TEST_ASSERT(OSW_SetSpeedLimit(NAN) == false, "NaN speed limit setting failure");
}

int main(void) {
    printf("=== OverspeedWarning Module Unit Tests ===\n");
    
    /* Run all tests */
    test_input_validation();
    test_speed_comparison();
    test_alarm_generation();
    test_error_handler();
    test_main_module();
    
    /* Output test results */
    printf("\n=== Test Results Summary ===\n");
    printf("Total tests: %d\n", test_count);
    printf("Passed tests: %d\n", test_passed);
    printf("Failed tests: %d\n", test_count - test_passed);
    printf("Pass rate: %.1f%%\n", (float)test_passed / test_count * 100.0f);
    
    if (test_passed == test_count) {
        printf("\n🎉 All tests passed! Module functionality is correct.\n");
        return 0;
    } else {
        printf("\n❌ Some tests failed, please check module implementation.\n");
        return 1;
    }
}