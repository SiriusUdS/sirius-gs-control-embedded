#pragma once

/***************************************************/
/***            Sensors/Devices Amount           ***/
/***************************************************/
#define FILLING_STATION_TEMPERATURE_SENSOR_AMOUNT (uint8_t)0x04
#define FILLING_STATION_LOAD_CELL_AMOUNT           (uint8_t)0x02
#define FILLING_STATION_PRESSURE_SENSOR_AMOUNT     (uint8_t)0x02
#define FILLING_STATION_HEATPAD_AMOUNT             (uint8_t)0x04
#define FILLING_STATION_VALVE_AMOUNT               (uint8_t)0x02

/***************************************************/
/***           Peripheral Distribution           ***/
/***************************************************/
#define FILLING_STATION_PWM_PER_VALVE                      (uint8_t)0x01
#define FILLING_STATION_GPIO_PER_VALVE                     (uint8_t)0x02
#define FILLING_STATION_ADC_CHANNEL_PER_TEMPERATURE_SENSOR (uint8_t)0x01
#define FILLING_STATION_ADC_CHANNEL_PER_PRESSURE_SENSOR    (uint8_t)0x01

/***************************************************/
/***              Peripheral Amount              ***/
/***************************************************/
#define FILLING_STATION_ADC_CHANNEL_AMOUNT FILLING_STATION_TEMPERATURE_SENSOR_AMOUNT + FILLING_STATION_LOAD_CELL_AMOUNT
#define FILLING_STATION_PWM_AMOUNT         FILLING_STATION_VALVE_AMOUNT
#define FILLING_STATION_GPIO_AMOUNT        (FILLING_STATION_VALVE_AMOUNT * FILLING_STATION_GPIO_PER_VALVE)

/***************************************************/
/***                Array Indexes                ***/
/***************************************************/
#define FILLING_STATION_NOS_VALVE_INDEX      (uint8_t)0x00
#define FILLING_STATION_NOS_DUMP_VALVE_INDEX (uint8_t)0x01

#define FILLING_STATION_NOS_VALVE_PWM_INDEX      (uint8_t)0x00
#define FILLING_STATION_NOS_DUMP_VALVE_PWM_INDEX (uint8_t)0x01

#define FILLING_STATION_NOS_VALVE_CLOSED_GPIO_INDEX      (uint8_t)0x00
#define FILLING_STATION_NOS_VALVE_OPENED_GPIO_INDEX      (uint8_t)0x01
#define FILLING_STATION_NOS_DUMP_VALVE_CLOSED_GPIO_INDEX (uint8_t)0x02
#define FILLING_STATION_NOS_DUMP_VALVE_OPENED_GPIO_INDEX (uint8_t)0x03

#define FILLING_STATION_FILL_VALVE_THERMISTANCE_INDEX    (uint8_t)0x00
#define FILLING_STATION_QUICK_CONNECT_THERMISTANCE_INDEX (uint8_t)0x01
#define FILLING_STATION_NOS_VALVE_THERMISTANCE_INDEX     (uint8_t)0x02
#define FILLING_STATION_IPA_VALVE_THERMISTANCE_INDEX     (uint8_t)0x03

#define FILLING_STATION_FILL_VALVE_THERMISTANCE_ADC_CHANNEL_INDEX      (uint8_t)0x00
#define FILLING_STATION_QUICK_CONNECT_THERMISTANCE_ADC_CHANNEL_INDEX   (uint8_t)0x01
#define FILLING_STATION_NOS_VALVE_THERMISTANCE_ADC_CHANNEL_INDEX       (uint8_t)0x02
#define FILLING_STATION_IPA_VALVE_THERMISTANCE_ADC_CHANNEL_INDEX       (uint8_t)0x03
#define FILLING_STATION_TANK_LOAD_CELL_ADC_CHANNEL_INDEX               (uint8_t)0x04
#define FILLING_STATION_COMBUSTION_CHAMBER_LOAD_CELL_ADC_CHANNEL_INDEX (uint8_t)0x05

#define FILLING_STATION_TANK_LOAD_CELL_INDEX               (uint8_t)0x00
#define FILLING_STATION_COMBUSTION_CHAMBER_LOAD_CELL_INDEX (uint8_t)0x01

#define FILLING_STATION_FILL_VALVE_HEATPAD_INDEX     (uint8_t)0x00
#define FILLING_STATION_QUICK_CONNECT_HEATPAD_INDEX  (uint8_t)0x01
#define FILLING_STATION_NOS_VALVE_HEATPAD_INDEX      (uint8_t)0x02
#define FILLING_STATION_IPA_VALVE_HEATPAD_INDEX      (uint8_t)0x03