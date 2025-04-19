#include "../Inc/GSControl.h"

static volatile GSControl gsControl;

uint32_t previous;
uint32_t previous2;
uint16_t testValueThermistance = 0;

static void executeInit(uint32_t timestamp_ms);
static void executeIdle(uint32_t timestamp_ms);
static void executeArming(uint32_t timestamp_ms);
static void executeIgnition(uint32_t timestamp_ms);
static void executePoweredFlight(uint32_t timestamp_ms);
static void executeUnpoweredFlight(uint32_t timestamp_ms);
static void executeAbort(uint32_t timestamp_ms);

static void initGPIOs();
static void initUART();
static void initUSB();
static void initButton();

static void initTelecom();

void GSControl_init(GPIO* gpios, UART* uart, USB* usb, Telecommunication* telecom, Button* button) {
  gsControl.errorStatus.value  = 0;
  gsControl.status.value       = 0;
  gsControl.currentState       = GS_CONTROL_STATE_INIT;
  
  gsControl.gpios  = gpios;
  gsControl.uart   = uart;
  gsControl.usb    = usb;

  gsControl.telecom = telecom;
  gsControl.button = button;

  initTelecom();

  initGPIOs();
  initUART();
  initUSB();
  initButton();
}

void GSControl_tick(uint32_t timestamp_ms) {
  gsControl.button->tick(gsControl.button, timestamp_ms);
  GSControl_execute(timestamp_ms);
}


void GSControl_execute(uint32_t timestamp_ms) {
  switch (gsControl.currentState) {
    case GS_CONTROL_STATE_INIT:
      executeInit(timestamp_ms);
      break;
    case GS_CONTROL_STATE_IDLE:
      executeIdle(timestamp_ms);
      break;
    case GS_CONTROL_STATE_ABORT:
      executeAbort(timestamp_ms);
      break;
    default:
    gsControl.errorStatus.bits.invalidState = 1;
      executeIdle(timestamp_ms);
      break;
  }
}

void executeInit(uint32_t timestamp_ms) {
  gsControl.currentState = GS_CONTROL_STATE_IDLE;

  gsControl.telecom->setupTelecom((struct Telecommunication*) gsControl.telecom);
}

void executeIdle(uint32_t timestamp_ms) {
  uint8_t data[] = "FUCK TRUMP!";

  if (HAL_GetTick() - previous >= 100) {
    previous = HAL_GetTick();
    gsControl.usb->transmit((struct USB*)gsControl.usb, data, sizeof(data));
  }

  if (gsControl.usb->status.bits.rxDataReady == 1) {
    uint8_t* test = gsControl.usb->rxBuffer;
    gsControl.usb->status.bits.rxDataReady = 0;
  }
  // Wait for arming command, collect data
  /*if(HAL_GetTick() - previous2 >= 500){
    previous2 = HAL_GetTick();

    engine.telecom->sendData((struct Telecommunication*)engine.telecom, data, sizeof(data)-1);
  }*/
  /*engine.valves[ENGINE_IPA_VALVE_INDEX].open((struct Valve*)&engine.valves[ENGINE_IPA_VALVE_INDEX], timestamp_ms);
  HAL_Delay(1000);
  engine.valves[ENGINE_IPA_VALVE_INDEX].setIdle((struct Valve*)&engine.valves[ENGINE_IPA_VALVE_INDEX]);
  engine.valves[ENGINE_IPA_VALVE_INDEX].close((struct Valve*)&engine.valves[ENGINE_IPA_VALVE_INDEX], timestamp_ms);
  HAL_Delay(1000);
  engine.valves[ENGINE_IPA_VALVE_INDEX].setIdle((struct Valve*)&engine.valves[ENGINE_IPA_VALVE_INDEX]);*/
  
}

void executeAbort(uint32_t timestamp_ms) {
  // Check flowcharts for wtf to do
}

void initGPIOs() {
  for (uint8_t i = 0; i < GS_CONTROL_GPIO_AMOUNT; i++) {
    if (gsControl.gpios[i].init == FUNCTION_NULL_POINTER) {
      gsControl.gpios[i].errorStatus.bits.nullFunctionPointer = 1;
      continue;
    }

    gsControl.gpios[i].init((struct GPIO*)&gsControl.gpios[i]);
  }
}

void initUART() {
  if (gsControl.uart->init == FUNCTION_NULL_POINTER) {
    gsControl.uart->errorStatus.bits.nullFunctionPointer = 1;
    return;
  }

  gsControl.uart->init(gsControl.uart);
}

void initUSB() {
  if (gsControl.usb->init == FUNCTION_NULL_POINTER) {
    gsControl.usb->errorStatus.bits.nullFunctionPointer = 1;
    return;
  }

  gsControl.usb->init((struct USB*)gsControl.usb);
}

void initTelecom(){
  if(gsControl.telecom->init == FUNCTION_NULL_POINTER){
    gsControl.telecom->errorStatus.bits.nullFunctionPointer = 1;
    return;
  }

  gsControl.telecom->init((struct Telecommunication*)gsControl.telecom);
  gsControl.telecom->uart = gsControl.uart;
}

void initButton(){
  if(gsControl.button->init == FUNCTION_NULL_POINTER){
    gsControl.button->errorStatus.bits.nullFunctionPointer = 1;
    return;
  }

  gsControl.button->init((struct Button*)gsControl.button);
  gsControl.button->gpio = &gsControl.gpios[GS_CONTROL_GPIO_BUTTON_IGNITE_INDEX];
}