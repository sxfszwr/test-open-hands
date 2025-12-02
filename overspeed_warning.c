/**
 * @file overspeed_warning.c
 * @brief 超速告警模块实现
 * @version 1.0
 * @date 2025-12-02
 * 
 * 模块标识：OSW_001
 * 需求追溯：V1.0
 */

#include "overspeed_warning.h"

/* 模块内部数据 */
static OSW_ModuleData_t g_osw_data;

/**
 * @brief 输入验证子模块实现
 * 职责：验证输入信号的完整性和有效性
 */
void InputValidation_CheckInputs(float vehicle_speed, float speed_limit, 
                                bool *inputs_valid, uint8_t *validation_error)
{
    *inputs_valid = true;
    *validation_error = OSW_ERROR_NONE;
    
    /* 检查VehicleSpeed是否为NaN */
    if (isnan(vehicle_speed)) {
        *inputs_valid = false;
        *validation_error = OSW_ERROR_SPEED_NAN;
        return;
    }
    
    /* 检查VehicleSpeed是否超出合理范围[0.0, 300.0] */
    if (vehicle_speed < OSW_MIN_VALID_SPEED || vehicle_speed > OSW_MAX_VALID_SPEED) {
        *inputs_valid = false;
        *validation_error = OSW_ERROR_SPEED_RANGE;
        return;
    }
    
    /* 检查SpeedLimit是否为NaN、0或负值 */
    if (isnan(speed_limit) || speed_limit <= 0.0f) {
        *inputs_valid = false;
        *validation_error = OSW_ERROR_LIMIT_INVALID;
        return;
    }
    
    /* 检查SpeedLimit是否超出配置范围[30.0, 120.0] */
    if (speed_limit < OSW_MIN_SPEED_LIMIT || speed_limit > OSW_MAX_SPEED_LIMIT) {
        *inputs_valid = false;
        *validation_error = OSW_ERROR_LIMIT_RANGE;
        return;
    }
}

/**
 * @brief 速度比较子模块实现
 * 职责：执行速度比较逻辑
 */
void SpeedComparison_CompareSpeed(float vehicle_speed, float speed_limit, 
                                 bool inputs_valid, bool *speed_exceeded)
{
    *speed_exceeded = false;
    
    /* 仅当输入有效时执行比较操作 */
    if (inputs_valid) {
        /* 比较VehicleSpeed和SpeedLimit的大小关系 */
        if (vehicle_speed > speed_limit) {
            *speed_exceeded = true;
        }
    }
}

/**
 * @brief 告警生成子模块实现
 * 职责：生成和更新告警状态
 */
void AlarmGeneration_UpdateAlarm(bool speed_exceeded, bool inputs_valid, 
                                bool *alarm_status)
{
    /* 输入无效时强制告警状态为false */
    if (!inputs_valid) {
        *alarm_status = false;
    } else {
        /* 根据速度比较结果生成告警状态 */
        *alarm_status = speed_exceeded;
    }
}

/**
 * @brief 错误处理子模块实现
 * 职责：管理模块错误状态
 */
void ErrorHandler_UpdateStatus(uint8_t validation_error, bool inputs_valid, 
                              uint8_t *error_status)
{
    if (inputs_valid) {
        /* 输入有效时清除错误状态 */
        *error_status = OSW_ERROR_NONE;
    } else {
        /* 根据输入验证结果设置错误状态码 */
        *error_status = validation_error;
    }
}

/**
 * @brief 模块初始化函数
 */
void OSW_Init(void)
{
    /* 模块上电初始化，设置默认速度阈值80.0 km/h */
    g_osw_data.vehicle_speed = 0.0f;
    g_osw_data.speed_limit = OSW_DEFAULT_LIMIT;
    
    /* 初始化告警状态为false，错误状态为0 */
    g_osw_data.overspeed_alarm = false;
    g_osw_data.error_status = OSW_ERROR_NONE;
    
    /* 初始化内部状态 */
    g_osw_data.inputs_valid = true;
    g_osw_data.speed_exceeded = false;
    g_osw_data.validation_error = OSW_ERROR_NONE;
}

/**
 * @brief 10ms周期主函数
 */
void OSW_MainFunction(float vehicle_speed, float speed_limit)
{
    /* 更新输入数据 */
    g_osw_data.vehicle_speed = vehicle_speed;
    g_osw_data.speed_limit = speed_limit;
    
    /* 调用InputValidation_CheckInputs()验证输入信号有效性 */
    InputValidation_CheckInputs(vehicle_speed, speed_limit, 
                               &g_osw_data.inputs_valid, 
                               &g_osw_data.validation_error);
    
    /* 如果输入有效，调用SpeedComparison_CompareSpeed()进行速度比较 */
    SpeedComparison_CompareSpeed(vehicle_speed, speed_limit, 
                                g_osw_data.inputs_valid, 
                                &g_osw_data.speed_exceeded);
    
    /* 调用AlarmGeneration_UpdateAlarm()更新告警状态 */
    AlarmGeneration_UpdateAlarm(g_osw_data.speed_exceeded, 
                               g_osw_data.inputs_valid, 
                               &g_osw_data.overspeed_alarm);
    
    /* 调用ErrorHandler_UpdateStatus()更新错误状态 */
    ErrorHandler_UpdateStatus(g_osw_data.validation_error, 
                             g_osw_data.inputs_valid, 
                             &g_osw_data.error_status);
}

/**
 * @brief 获取当前告警状态
 */
bool OSW_GetAlarmStatus(void)
{
    return g_osw_data.overspeed_alarm;
}

/**
 * @brief 获取错误状态
 */
uint8_t OSW_GetErrorStatus(void)
{
    return g_osw_data.error_status;
}

/**
 * @brief 设置速度阈值
 */
bool OSW_SetSpeedLimit(float speed_limit)
{
    /* 验证新的速度阈值是否有效 */
    if (isnan(speed_limit) || speed_limit <= 0.0f) {
        return false;
    }
    
    if (speed_limit < OSW_MIN_SPEED_LIMIT || speed_limit > OSW_MAX_SPEED_LIMIT) {
        return false;
    }
    
    /* 设置新的速度阈值 */
    g_osw_data.speed_limit = speed_limit;
    return true;
}