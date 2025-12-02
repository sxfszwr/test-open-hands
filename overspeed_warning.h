/**
 * @file overspeed_warning.h
 * @brief 超速告警模块头文件
 * @version 1.0
 * @date 2025-12-02
 * 
 * 模块标识：OSW_001
 * 需求追溯：V1.0
 */

#ifndef OVERSPEED_WARNING_H
#define OVERSPEED_WARNING_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* 错误状态码定义 */
#define OSW_ERROR_NONE        0x00
#define OSW_ERROR_SPEED_NAN   0x01
#define OSW_ERROR_SPEED_RANGE 0x02
#define OSW_ERROR_LIMIT_INVALID 0x03
#define OSW_ERROR_LIMIT_RANGE 0x04

/* 合理速度范围定义 */
#define OSW_MIN_VALID_SPEED   0.0f
#define OSW_MAX_VALID_SPEED   300.0f
#define OSW_MIN_SPEED_LIMIT   30.0f
#define OSW_MAX_SPEED_LIMIT   120.0f
#define OSW_DEFAULT_LIMIT     80.0f

/* 模块内部数据结构 */
typedef struct {
    float vehicle_speed;        /* 当前车辆速度 (km/h) */
    float speed_limit;          /* 速度阈值 (km/h) */
    bool overspeed_alarm;       /* 超速告警状态 */
    uint8_t error_status;       /* 错误状态码 */
    bool inputs_valid;          /* 输入有效性标志 */
    bool speed_exceeded;        /* 速度超过阈值标志 */
    uint8_t validation_error;   /* 输入验证错误码 */
} OSW_ModuleData_t;

/* 上层调用接口 */

/**
 * @brief 模块初始化函数
 * @param None
 * @return None
 */
void OSW_Init(void);

/**
 * @brief 10ms周期主函数
 * @param vehicle_speed 当前车辆速度 (km/h)
 * @param speed_limit 速度阈值 (km/h)
 * @return None
 */
void OSW_MainFunction(float vehicle_speed, float speed_limit);

/**
 * @brief 获取当前告警状态
 * @param None
 * @return bool 告警状态 (true: 超速告警, false: 无告警)
 */
bool OSW_GetAlarmStatus(void);

/**
 * @brief 获取错误状态
 * @param None
 * @return uint8_t 错误状态码
 */
uint8_t OSW_GetErrorStatus(void);

/**
 * @brief 设置速度阈值
 * @param speed_limit 新的速度阈值 (km/h)
 * @return bool 设置是否成功
 */
bool OSW_SetSpeedLimit(float speed_limit);

/* 子模块接口 */

/**
 * @brief 输入验证子模块
 * @param vehicle_speed 车辆速度
 * @param speed_limit 速度阈值
 * @param inputs_valid 输入有效性标志 (输出)
 * @param validation_error 验证错误码 (输出)
 * @return None
 */
void InputValidation_CheckInputs(float vehicle_speed, float speed_limit, 
                                bool *inputs_valid, uint8_t *validation_error);

/**
 * @brief 速度比较子模块
 * @param vehicle_speed 车辆速度
 * @param speed_limit 速度阈值
 * @param inputs_valid 输入有效性标志
 * @param speed_exceeded 速度超过阈值标志 (输出)
 * @return None
 */
void SpeedComparison_CompareSpeed(float vehicle_speed, float speed_limit, 
                                 bool inputs_valid, bool *speed_exceeded);

/**
 * @brief 告警生成子模块
 * @param speed_exceeded 速度超过阈值标志
 * @param inputs_valid 输入有效性标志
 * @param alarm_status 告警状态 (输出)
 * @return None
 */
void AlarmGeneration_UpdateAlarm(bool speed_exceeded, bool inputs_valid, 
                                bool *alarm_status);

/**
 * @brief 错误处理子模块
 * @param validation_error 验证错误码
 * @param inputs_valid 输入有效性标志
 * @param error_status 错误状态码 (输出)
 * @return None
 */
void ErrorHandler_UpdateStatus(uint8_t validation_error, bool inputs_valid, 
                              uint8_t *error_status);

#endif /* OVERSPEED_WARNING_H */