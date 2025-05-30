#include "../Inc/GSControl.h"

static volatile GSControl gsControl;

uint32_t previous;
uint32_t previous2;
uint16_t testValueThermistance = 0;

static void executeInit(uint32_t timestamp_ms);
static void executeIdle(uint32_t timestamp_ms);
static void executeAbort(uint32_t timestamp_ms);

static void initGPIOs();
static void initUART();
static void initUSB();
static void initButton();

static void initTelecom();

static void initDatabridge();

static void updateButtonStates();

void GSControl_init(GPIO* gpios, UART* uart, USB* usb, Telecommunication* telecom, Button* buttons, DataBridge* databridge) {
  gsControl.errorStatus.value  = 0;
  gsControl.status.value       = 0;
  gsControl.currentState       = GS_CONTROL_STATE_INIT;
  
  gsControl.gpios  = gpios;
  gsControl.uart   = uart;
  gsControl.usb    = usb;

  gsControl.telecom = telecom;
  gsControl.buttons = buttons;
  gsControl.DataBridge = databridge;


  initTelecom();

  initGPIOs();
  initUART();
  initUSB();
  initButton();
  //initDatabridge();

}

void GSControl_tick(uint32_t timestamp_ms) {
  for (uint8_t i = 0; i < GS_CONTROL_BUTTON_AMOUNT;i++) {
    gsControl.buttons[i].tick((struct Button*)&gsControl.buttons[i], timestamp_ms);
  }
  updateButtonStates();
  
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

  XBEE_config((struct Telecommunication*)&gsControl.telecom);

  gsControl.telecom->config((struct Telecommunication*) gsControl.telecom);
}

void executeIdle(uint32_t timestamp_ms) {
  if (HAL_GetTick() - previous >= 100) {
    previous = HAL_GetTick();
    //gsControl.usb->transmit((struct USB*)gsControl.usb, data, sizeof(data));
  }


   //gsControl.DataBridge->receiveUART(gsControl.DataBridge);
}

void executeAbort(uint32_t timestamp_ms) {
  // Check flowcharts for wtf to do
}

void updateButtonStates() {
  if (gsControl.buttons[GS_CONTROL_BUTTON_EMERGENCY_STOP_INDEX].status.bits.isPressed) {
    gsControl.status.bits.isEmergencyStopButtonPressed = 1;
  }
  else {
    gsControl.status.bits.isEmergencyStopButtonPressed = 0;
  }

  if (gsControl.buttons[GS_CONTROL_BUTTON_UNSAFE_INDEX].status.bits.isPressed) {
    gsControl.status.bits.isUnsafeKeySwitchPressed = 1;
  }
  else {
    gsControl.status.bits.isUnsafeKeySwitchPressed = 0;
  }

  if (gsControl.buttons[GS_CONTROL_BUTTON_ALLOW_FILL_INDEX].status.bits.isPressed) {
    gsControl.status.bits.isAllowFillSwitchOn = 1;
  }
  else {
    gsControl.status.bits.isAllowFillSwitchOn = 0;
  }

  if (gsControl.buttons[GS_CONTROL_BUTTON_ARM_IGNITER_INDEX].status.bits.isPressed) {
    gsControl.status.bits.isArmIgniterSwitchOn = 1;
  }
  else {
    gsControl.status.bits.isArmIgniterSwitchOn = 0;
  }

  if (gsControl.buttons[GS_CONTROL_BUTTON_ARM_VALVE_INDEX].status.bits.isPressed) {
    gsControl.status.bits.isArmServoSwitchOn = 1;
  }
  else {
    gsControl.status.bits.isArmServoSwitchOn = 0;
  }

  if (gsControl.buttons[GS_CONTROL_BUTTON_FIRE_IGNITER_INDEX].status.bits.isPressed) {
    gsControl.status.bits.isFireIgniterButtonPressed = 1;
  }
  else {
    gsControl.status.bits.isFireIgniterButtonPressed = 0;
  }

  if (gsControl.buttons[GS_CONTROL_BUTTON_UNUSED_INDEX].status.bits.isPressed) {
    gsControl.status.bits.isUnusedSwitchOn = 1;
  }
  else {
    gsControl.status.bits.isUnusedSwitchOn = 0;
  }
}

/*void initDatabridge() {
    if (gsControl.DataBridge == NULL) {
        gsControl.errorStatus.bits.notInitialized = 1;
        return;
    }

    gsControl.DataBridge->usb  = gsControl.usb;
    gsControl.DataBridge->uart = gsControl.uart;
    gsControl.DataBridge->xbee = gsControl.telecom;

    if (gsControl.DataBridge->init == FUNCTION_NULL_POINTER) {
        gsControl.errorStatus.bits.notInitialized = 1;  
        return;
    }

    //gsControl.DataBridge->init(gsControl.DataBridge);
}*/

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

  gsControl.uart->init((struct UART*)gsControl.uart);
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
  for (uint8_t i = 0; i < GS_CONTROL_BUTTON_AMOUNT; i++) {
    if (gsControl.buttons[i].init == FUNCTION_NULL_POINTER) {
      gsControl.buttons[i].errorStatus.bits.nullFunctionPointer = 1;
      continue;
    }

    gsControl.buttons[i].debounceTargetReadCount = 15;
    gsControl.buttons[i].delayBetweenReads_ms = 4;
    gsControl.buttons[i].init((struct Button*)&gsControl.buttons[i]);
    gsControl.buttons[i].gpio = &gsControl.gpios[i];
  }
}