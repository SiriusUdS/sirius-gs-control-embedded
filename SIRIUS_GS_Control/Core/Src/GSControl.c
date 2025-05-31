#include "../Inc/GSControl.h"

static GSControl gsControl = {0};

BoardCommand currentBoardCommand = {0};
CommandResponse currentResponse = {0};
GSCommand currentGSCommand = {0};

EngineTelemetryPacket currentEngineTelemetryPacket = {0};
EngineStatusPacket currentEngineStatusPacket = {0};
FillingStationTelemetryPacket currentFillingStationTelemetryPacket = {0};
FillingStationStatusPacket currentFillingStationStatusPacket = {0};

uint8_t uartBuffer[sizeof(EngineTelemetryPacket)] = {0};

uint8_t testMessage[] = "LETS GO BRANDON";

uint32_t packetHeader = 0;
uint32_t packetType = 0;

static void executeInit(uint32_t timestamp_ms);
static void executeIdle(uint32_t timestamp_ms);
static void executeAbort(uint32_t timestamp_ms);

static void initGPIOs();
static void initUART();
static void initUSB();

static void initButton();
static void initTelecom();

static void updateButtonStates();
static void parseIncomingPackets();
static void parseUartPacket();
static void parseUsbPacket();

static void parseCommandResponsePacket();
static void parseBoardCommandPacket();
static void parseTelemetryPacket(uint32_t headerValue);
static void parseStatusPacket(uint32_t headerValue);

static void parseEngineTelemetryPacket();
static void parseEngineStatusPacket();
static void parseFillingStationTelemetryPacket();
static void parseFillingStationStatusPacket();

void GSControl_init(GPIO* gpios, UART* uart, volatile USB* usb, Telecommunication* telecom, Button* buttons) {
  gsControl.errorStatus.value  = 0;
  gsControl.status.value       = 0;
  gsControl.currentState       = GS_CONTROL_STATE_INIT;
  
  gsControl.gpios  = gpios;
  gsControl.uart   = uart;
  gsControl.usb    = usb;

  gsControl.telecommunication = telecom;
  gsControl.buttons = buttons;

  initTelecom();

  initGPIOs();
  initUART();
  initUSB();
  initButton();
}

void GSControl_tick(uint32_t timestamp_ms) {
  for (uint8_t i = 0; i < GS_CONTROL_BUTTON_AMOUNT;i++) {
    gsControl.buttons[i].tick((struct Button*)&gsControl.buttons[i], timestamp_ms);
  }
  gsControl.telecommunication->tick((struct Telecommunication*)gsControl.telecommunication, timestamp_ms);

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
  gsControl.telecommunication->config((struct Telecommunication*) gsControl.telecommunication);
  HAL_UART_Receive_DMA((UART_HandleTypeDef*)gsControl.uart->externalHandle, uartBuffer, sizeof(uartBuffer)); // Start the DMA
  gsControl.uart->status.bits.txReady = 1;
  gsControl.currentState = GS_CONTROL_STATE_IDLE;
}

void executeIdle(uint32_t timestamp_ms) {
  updateButtonStates();
  parseIncomingPackets();

  
  if (gsControl.uart->status.bits.txReady == 1) {
    HAL_UART_Transmit_DMA((UART_HandleTypeDef*)gsControl.uart->externalHandle, testMessage, sizeof(testMessage));
    gsControl.uart->status.bits.txReady = 0;
    HAL_Delay(200);
  }

  //HAL_UART_Receive_DMA();
  // INTERPRET COMMAND THEN SEND OR NO
}

void executeAbort(uint32_t timestamp_ms) {
  updateButtonStates();
  parseIncomingPackets();

  // INTERPRET COMMAND THEN SEND OR NO
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

void parseIncomingPackets() {
  if (gsControl.telecommunication->uart->status.bits.rxDataReady == 1) {
    parseUartPacket();
    gsControl.telecommunication->uart->status.bits.rxDataReady = 0;
    HAL_UART_Receive_DMA((UART_HandleTypeDef*)gsControl.uart->externalHandle, uartBuffer, sizeof(uartBuffer));
  }

  if (gsControl.usb->status.bits.rxDataReady) {
    parseUsbPacket();
    gsControl.usb->status.bits.rxDataReady = 0;
  }
}

void parseUartPacket() {
  switch (packetType)
  {
    case COMMAND_RESPONSE_TYPE_CODE:
      parseCommandResponsePacket();
      break;
    case TELEMETRY_TYPE_CODE:
      parseTelemetryPacket(packetHeader);
      break;
    case STATUS_TYPE_CODE:
      parseStatusPacket(packetHeader);
      break;
    default:
      // RAISE ERROR FLAG
      break;
  }
}

void parseUsbPacket() {
  uint32_t packetHeader = uartBuffer[0] & uartBuffer[1] << 8 & uartBuffer[2] << 16 & uartBuffer[3] << 24;
  uint32_t type = packetHeader & 0xFFFFF000UL;
  
  if (type == BOARD_COMMAND_TYPE_CODE) {
    parseBoardCommandPacket();
  }
}

void parseCommandResponsePacket() {
  // Check CRC
  for (uint8_t i = 0; i < sizeof(CommandResponse); i++) {
    currentResponse.data[i] = uartBuffer[i];
  }
}

void parseBoardCommandPacket() {
  // Check CRC
  for (uint8_t i = 0; i < sizeof(BoardCommand); i++) {
    currentBoardCommand.data[i] = gsControl.usb->rxBuffer[i];
  }
}

void parseTelemetryPacket(uint32_t headerValue) {
  switch (headerValue & 0x000000D0UL) {
    case TELEMETRY_ENGINE_BOARD_ID:
      parseEngineTelemetryPacket();
      break;
    case TELEMETRY_FILLING_STATION_BOARD_ID:
      parseFillingStationTelemetryPacket();
      break;
    default:
      // UNKNOWN BOARD, RAISE ERROR FLAG
      break;
  }
}

void parseStatusPacket(uint32_t headerValue) {
  switch (headerValue & 0x000000D0UL) {
    case TELEMETRY_ENGINE_BOARD_ID:
      parseEngineStatusPacket();
      break;
    case TELEMETRY_FILLING_STATION_BOARD_ID:
      parseFillingStationStatusPacket();
      break;
    default:
      // UNKNOWN BOARD, RAISE ERROR FLAG
      break;
  }
}

void parseEngineTelemetryPacket() {
  // Check CRC
  for (uint8_t i = 0; i < sizeof(EngineTelemetryPacket); i++) {
    currentEngineTelemetryPacket.data[i] = uartBuffer[i];
  }
}

void parseEngineStatusPacket() {
  // Check CRC
  for (uint8_t i = 0; i < sizeof(EngineStatusPacket); i++) {
    currentEngineStatusPacket.data[i] = uartBuffer[i];
  }
}

void parseFillingStationTelemetryPacket() {
  // Check CRC
  for (uint8_t i = 0; i < sizeof(FillingStationTelemetryPacket); i++) {
    currentFillingStationTelemetryPacket.data[i] = uartBuffer[i];
  }
}

void parseFillingStationStatusPacket() {
  // Check CRC
  for (uint8_t i = 0; i < sizeof(FillingStationTelemetryPacket); i++) {
    currentFillingStationStatusPacket.data[i] = uartBuffer[i];
  }
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
  if(gsControl.telecommunication->init == FUNCTION_NULL_POINTER){
    gsControl.telecommunication->errorStatus.bits.nullFunctionPointer = 1;
    return;
  }

  gsControl.telecommunication->uart = gsControl.uart;
  gsControl.telecommunication->init((struct Telecommunication*)gsControl.telecommunication);
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

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART1) {
    packetHeader = uartBuffer[0] & uartBuffer[1] << 8 & uartBuffer[2] << 16 & uartBuffer[3] << 24;
    packetType = packetHeader & 0xFFFFF000UL;
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART1) {
    if (gsControl.uart->status.bits.rxDataReady == 0) {
      // CHECK CRC, IF NOT GOOD, RAISE FLAG FOR NEXT STATUS PACKET
      gsControl.uart->status.bits.rxDataReady = 1;
    }
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART1) {
    if (gsControl.uart->status.bits.txReady == 0) {
      gsControl.uart->status.bits.txReady = 1;
    }
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART1) {
    HAL_UART_Receive_DMA((UART_HandleTypeDef*)gsControl.telecommunication->uart->externalHandle, uartBuffer, sizeof(uartBuffer));
  }
}