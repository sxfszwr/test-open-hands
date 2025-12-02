/**
 * @file main_example.c
 * @brief 超速告警模块使用示例
 * @version 1.0
 * @date 2025-12-02
 * 
 * 演示如何使用OverspeedWarning模块进行超速监控
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "overspeed_warning.h"

/* 模拟10ms定时器 */
#define CYCLE_TIME_MS 10

/* 错误状态码描述 */
const char* get_error_description(uint8_t error_code) {
    switch (error_code) {
        case OSW_ERROR_NONE:
            return "正常";
        case OSW_ERROR_SPEED_NAN:
            return "速度信号为NaN";
        case OSW_ERROR_SPEED_RANGE:
            return "速度超出有效范围";
        case OSW_ERROR_LIMIT_INVALID:
            return "速度阈值无效";
        case OSW_ERROR_LIMIT_RANGE:
            return "速度阈值超出配置范围";
        default:
            return "未知错误";
    }
}

/* 打印模块状态 */
void print_module_status(float vehicle_speed, float speed_limit, int cycle_count) {
    bool alarm_status = OSW_GetAlarmStatus();
    uint8_t error_status = OSW_GetErrorStatus();
    
    printf("周期 %3d: 车速=%.1f km/h, 限速=%.1f km/h, 告警=%s, 错误=%s\n",
           cycle_count,
           vehicle_speed,
           speed_limit,
           alarm_status ? "是" : "否",
           get_error_description(error_status));
}

int main(void) {
    printf("=== 超速告警模块测试示例 ===\n\n");
    
    /* 1. 模块初始化 */
    printf("1. 初始化模块...\n");
    OSW_Init();
    printf("   默认速度阈值: %.1f km/h\n\n", OSW_DEFAULT_LIMIT);
    
    /* 2. 正常工作场景测试 */
    printf("2. 正常工作场景测试:\n");
    float test_speeds[] = {60.0f, 75.0f, 85.0f, 90.0f, 70.0f};
    float speed_limit = 80.0f;
    
    for (int i = 0; i < 5; i++) {
        OSW_MainFunction(test_speeds[i], speed_limit);
        print_module_status(test_speeds[i], speed_limit, i + 1);
        usleep(CYCLE_TIME_MS * 1000); /* 模拟10ms延迟 */
    }
    
    printf("\n");
    
    /* 3. 动态调整速度阈值测试 */
    printf("3. 动态调整速度阈值测试:\n");
    printf("   设置新的速度阈值为100.0 km/h\n");
    if (OSW_SetSpeedLimit(100.0f)) {
        printf("   速度阈值设置成功\n");
    } else {
        printf("   速度阈值设置失败\n");
    }
    
    OSW_MainFunction(90.0f, 100.0f);
    print_module_status(90.0f, 100.0f, 6);
    
    printf("\n");
    
    /* 4. 异常输入测试 */
    printf("4. 异常输入测试:\n");
    
    /* 测试速度为NaN */
    printf("   测试速度为NaN:\n");
    OSW_MainFunction(NAN, 80.0f);
    print_module_status(NAN, 80.0f, 7);
    
    /* 测试速度超出范围 */
    printf("   测试速度超出范围(350 km/h):\n");
    OSW_MainFunction(350.0f, 80.0f);
    print_module_status(350.0f, 80.0f, 8);
    
    /* 测试速度阈值无效 */
    printf("   测试速度阈值无效(0 km/h):\n");
    OSW_MainFunction(60.0f, 0.0f);
    print_module_status(60.0f, 0.0f, 9);
    
    /* 测试速度阈值超出范围 */
    printf("   测试速度阈值超出范围(150 km/h):\n");
    OSW_MainFunction(60.0f, 150.0f);
    print_module_status(60.0f, 150.0f, 10);
    
    printf("\n");
    
    /* 5. 恢复正常输入测试 */
    printf("5. 恢复正常输入测试:\n");
    OSW_MainFunction(75.0f, 80.0f);
    print_module_status(75.0f, 80.0f, 11);
    
    printf("\n=== 测试完成 ===\n");
    
    return 0;
}