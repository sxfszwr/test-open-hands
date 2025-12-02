/**
 * @file unit_test.c
 * @brief 超速告警模块单元测试
 * @version 1.0
 * @date 2025-12-02
 * 
 * 对各个子模块进行详细的单元测试
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "overspeed_warning.h"

/* 测试计数器 */
static int test_count = 0;
static int test_passed = 0;

/* 测试宏 */
#define TEST_ASSERT(condition, description) \
    do { \
        test_count++; \
        if (condition) { \
            test_passed++; \
            printf("✓ 测试 %d: %s - 通过\n", test_count, description); \
        } else { \
            printf("✗ 测试 %d: %s - 失败\n", test_count, description); \
        } \
    } while(0)

/* 测试InputValidation子模块 */
void test_input_validation(void) {
    printf("\n=== InputValidation子模块测试 ===\n");
    
    bool inputs_valid;
    uint8_t validation_error;
    
    /* 测试正常输入 */
    InputValidation_CheckInputs(60.0f, 80.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == true && validation_error == OSW_ERROR_NONE, 
                "正常输入验证");
    
    /* 测试速度为NaN */
    InputValidation_CheckInputs(NAN, 80.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_SPEED_NAN, 
                "速度为NaN检测");
    
    /* 测试速度为负值 */
    InputValidation_CheckInputs(-10.0f, 80.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_SPEED_RANGE, 
                "速度为负值检测");
    
    /* 测试速度超出上限 */
    InputValidation_CheckInputs(350.0f, 80.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_SPEED_RANGE, 
                "速度超出上限检测");
    
    /* 测试速度阈值为NaN */
    InputValidation_CheckInputs(60.0f, NAN, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_LIMIT_INVALID, 
                "速度阈值为NaN检测");
    
    /* 测试速度阈值为0 */
    InputValidation_CheckInputs(60.0f, 0.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_LIMIT_INVALID, 
                "速度阈值为0检测");
    
    /* 测试速度阈值为负值 */
    InputValidation_CheckInputs(60.0f, -20.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_LIMIT_INVALID, 
                "速度阈值为负值检测");
    
    /* 测试速度阈值低于下限 */
    InputValidation_CheckInputs(60.0f, 20.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_LIMIT_RANGE, 
                "速度阈值低于下限检测");
    
    /* 测试速度阈值高于上限 */
    InputValidation_CheckInputs(60.0f, 150.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == false && validation_error == OSW_ERROR_LIMIT_RANGE, 
                "速度阈值高于上限检测");
    
    /* 测试边界值 */
    InputValidation_CheckInputs(0.0f, 30.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == true && validation_error == OSW_ERROR_NONE, 
                "速度下边界值测试");
    
    InputValidation_CheckInputs(300.0f, 120.0f, &inputs_valid, &validation_error);
    TEST_ASSERT(inputs_valid == true && validation_error == OSW_ERROR_NONE, 
                "速度上边界值测试");
}

/* 测试SpeedComparison子模块 */
void test_speed_comparison(void) {
    printf("\n=== SpeedComparison子模块测试 ===\n");
    
    bool speed_exceeded;
    
    /* 测试速度未超过阈值 */
    SpeedComparison_CompareSpeed(70.0f, 80.0f, true, &speed_exceeded);
    TEST_ASSERT(speed_exceeded == false, "速度未超过阈值");
    
    /* 测试速度等于阈值 */
    SpeedComparison_CompareSpeed(80.0f, 80.0f, true, &speed_exceeded);
    TEST_ASSERT(speed_exceeded == false, "速度等于阈值");
    
    /* 测试速度超过阈值 */
    SpeedComparison_CompareSpeed(90.0f, 80.0f, true, &speed_exceeded);
    TEST_ASSERT(speed_exceeded == true, "速度超过阈值");
    
    /* 测试输入无效时不进行比较 */
    SpeedComparison_CompareSpeed(90.0f, 80.0f, false, &speed_exceeded);
    TEST_ASSERT(speed_exceeded == false, "输入无效时不进行比较");
    
    /* 测试边界情况 */
    SpeedComparison_CompareSpeed(80.1f, 80.0f, true, &speed_exceeded);
    TEST_ASSERT(speed_exceeded == true, "微小超速检测");
}

/* 测试AlarmGeneration子模块 */
void test_alarm_generation(void) {
    printf("\n=== AlarmGeneration子模块测试 ===\n");
    
    bool alarm_status;
    
    /* 测试正常告警生成 */
    AlarmGeneration_UpdateAlarm(true, true, &alarm_status);
    TEST_ASSERT(alarm_status == true, "超速时生成告警");
    
    /* 测试无超速时无告警 */
    AlarmGeneration_UpdateAlarm(false, true, &alarm_status);
    TEST_ASSERT(alarm_status == false, "无超速时无告警");
    
    /* 测试输入无效时强制无告警 */
    AlarmGeneration_UpdateAlarm(true, false, &alarm_status);
    TEST_ASSERT(alarm_status == false, "输入无效时强制无告警");
    
    /* 测试输入无效且无超速 */
    AlarmGeneration_UpdateAlarm(false, false, &alarm_status);
    TEST_ASSERT(alarm_status == false, "输入无效且无超速时无告警");
}

/* 测试ErrorHandler子模块 */
void test_error_handler(void) {
    printf("\n=== ErrorHandler子模块测试 ===\n");
    
    uint8_t error_status;
    
    /* 测试输入有效时清除错误 */
    ErrorHandler_UpdateStatus(OSW_ERROR_SPEED_NAN, true, &error_status);
    TEST_ASSERT(error_status == OSW_ERROR_NONE, "输入有效时清除错误");
    
    /* 测试输入无效时设置错误状态 */
    ErrorHandler_UpdateStatus(OSW_ERROR_SPEED_NAN, false, &error_status);
    TEST_ASSERT(error_status == OSW_ERROR_SPEED_NAN, "设置速度NaN错误状态");
    
    ErrorHandler_UpdateStatus(OSW_ERROR_SPEED_RANGE, false, &error_status);
    TEST_ASSERT(error_status == OSW_ERROR_SPEED_RANGE, "设置速度范围错误状态");
    
    ErrorHandler_UpdateStatus(OSW_ERROR_LIMIT_INVALID, false, &error_status);
    TEST_ASSERT(error_status == OSW_ERROR_LIMIT_INVALID, "设置阈值无效错误状态");
    
    ErrorHandler_UpdateStatus(OSW_ERROR_LIMIT_RANGE, false, &error_status);
    TEST_ASSERT(error_status == OSW_ERROR_LIMIT_RANGE, "设置阈值范围错误状态");
}

/* 测试主模块集成功能 */
void test_main_module(void) {
    printf("\n=== 主模块集成测试 ===\n");
    
    /* 初始化测试 */
    OSW_Init();
    TEST_ASSERT(OSW_GetAlarmStatus() == false, "初始化后告警状态为false");
    TEST_ASSERT(OSW_GetErrorStatus() == OSW_ERROR_NONE, "初始化后错误状态为正常");
    
    /* 正常工作测试 */
    OSW_MainFunction(70.0f, 80.0f);
    TEST_ASSERT(OSW_GetAlarmStatus() == false, "正常速度无告警");
    TEST_ASSERT(OSW_GetErrorStatus() == OSW_ERROR_NONE, "正常输入无错误");
    
    /* 超速告警测试 */
    OSW_MainFunction(90.0f, 80.0f);
    TEST_ASSERT(OSW_GetAlarmStatus() == true, "超速时产生告警");
    TEST_ASSERT(OSW_GetErrorStatus() == OSW_ERROR_NONE, "超速时无错误状态");
    
    /* 异常输入测试 */
    OSW_MainFunction(NAN, 80.0f);
    TEST_ASSERT(OSW_GetAlarmStatus() == false, "异常输入时无告警");
    TEST_ASSERT(OSW_GetErrorStatus() == OSW_ERROR_SPEED_NAN, "检测到速度NaN错误");
    
    /* 恢复正常测试 */
    OSW_MainFunction(75.0f, 80.0f);
    TEST_ASSERT(OSW_GetAlarmStatus() == false, "恢复正常后无告警");
    TEST_ASSERT(OSW_GetErrorStatus() == OSW_ERROR_NONE, "恢复正常后无错误");
    
    /* 动态配置测试 */
    TEST_ASSERT(OSW_SetSpeedLimit(100.0f) == true, "设置有效速度阈值成功");
    TEST_ASSERT(OSW_SetSpeedLimit(200.0f) == false, "设置无效速度阈值失败");
    TEST_ASSERT(OSW_SetSpeedLimit(NAN) == false, "设置NaN速度阈值失败");
}

int main(void) {
    printf("=== 超速告警模块单元测试 ===\n");
    
    /* 运行所有测试 */
    test_input_validation();
    test_speed_comparison();
    test_alarm_generation();
    test_error_handler();
    test_main_module();
    
    /* 输出测试结果 */
    printf("\n=== 测试结果汇总 ===\n");
    printf("总测试数: %d\n", test_count);
    printf("通过测试: %d\n", test_passed);
    printf("失败测试: %d\n", test_count - test_passed);
    printf("通过率: %.1f%%\n", (float)test_passed / test_count * 100.0f);
    
    if (test_passed == test_count) {
        printf("\n🎉 所有测试通过！模块功能正常。\n");
        return 0;
    } else {
        printf("\n❌ 存在测试失败，请检查模块实现。\n");
        return 1;
    }
}