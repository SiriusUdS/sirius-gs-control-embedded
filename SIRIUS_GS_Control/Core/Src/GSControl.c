#include "../Inc/GSControl.h"

static GSControl gsControl = {0};

BoardCommand currentBoardCommand = {0};
CommandResponse currentResponse = {0};
GSCommand currentGSCommand = {0};

EngineTelemetryPacket currentEngineTelemetryPacket = {0};
EngineStatusPacket currentEngineStatusPacket = {0};
FillingStationTelemetryPacket currentFillingStationTelemetryPacket = {0};
FillingStationStatusPacket currentFillingStationStatusPacket = {0};

GSControlStatusPacket currentGSControlStatusPacket = {
  .fields = {
    .header = {
      .bits = {
        .type = STATUS_TYPE_CODE,
        .boardId = TELEMETRY_GS_CONTROL_BOARD_ID,
        .RESERVED = 0
      }
    },
    .errorStatus = 0,
    .status = 0,
    .timestamp_ms = 0,
    .crc = 0
  }
};

uint8_t uartBuffer[880] = {0};

uint8_t testMessage[] = "LETS GO BRANDON";

uint32_t telemetryPacketHeader = 0;
uint32_t telemetryPacketType = 0;

uint32_t commandPacketHeader = 0;
uint32_t commandPacketType = 0;

uint8_t engineState;
uint8_t fillingStationState;

static void executeInit(uint32_t timestamp_ms);
static void executeIdle(uint32_t timestamp_ms);
static void executeAbort(uint32_t timestamp_ms);

static void initGPIOs();
static void initUART();
static void initUSB();

static void initButton();
static void initTelecom();

static void updateButtonStates();
static void handleIncomingPackets();
static void handleIncomingCommand();
static void handleCommunication(uint32_t timestamp_ms);
static void sendStatusPacket(uint32_t timestamp_ms);
static void sendBoardCommand();
static void sendACKResponse();

static uint8_t checkUnsafe();
static uint8_t checkAllowFill();
/*static uint8_t check();
static uint8_t checkUnsafe();*/

static void parseUartPacket();
static uint8_t parseUsbPacket();

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

  handleIncomingCommand();
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
  //gsControl.uart->status.bits.txReady = 1;
  gsControl.currentState = GS_CONTROL_STATE_IDLE;
}

void executeIdle(uint32_t timestamp_ms) {
  updateButtonStates();
  //handleIncomingPackets();
  handleCommunication(timestamp_ms);
  
  /*if (gsControl.uart->status.bits.txReady == 1) {
    HAL_UART_Transmit_IT((UART_HandleTypeDef*)gsControl.uart->externalHandle, testMessage, sizeof(testMessage));
    gsControl.uart->status.bits.txReady = 0;
    HAL_Delay(200);
  }*/
}

void executeAbort(uint32_t timestamp_ms) {
  updateButtonStates();
  //handleIncomingPackets();
  handleCommunication(timestamp_ms);
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

  if (gsControl.buttons[GS_CONTROL_BUTTON_ALLOW_DUMP_INDEX].status.bits.isPressed) {
    gsControl.status.bits.isAllowDumpSwitchOn = 1;
  }
  else {
    gsControl.status.bits.isAllowDumpSwitchOn = 0;
  }

  if (gsControl.buttons[GS_CONTROL_BUTTON_VALVE_START_INDEX].status.bits.isPressed) {
    gsControl.status.bits.isValveStartButtonPressed = 1;
  }
  else {
    gsControl.status.bits.isValveStartButtonPressed = 0;
  }
}

// PUT INTERRUPT BASED
void handleIncomingPackets() {
  
  if (gsControl.telecommunication->uart->status.bits.rxDataReady == 1) {
    // CHECK CRC
    //parseUartPacket();
    //gsControl.telecommunication->uart->status.bits.rxDataReady = 0;
  }
}

void handleIncomingCommand() {
  if (gsControl.usb->status.bits.rxDataReady) {
    if (!parseUsbPacket()) {
      //gsControl.errorStatus.bits.invalidCommand = 1;
      gsControl.usb->status.bits.rxDataReady = 0;
      return;
    }

    switch (currentBoardCommand.fields.header.bits.commandCode)
    {
      case BOARD_COMMAND_CODE_ABORT:
        sendBoardCommand();
        gsControl.status.bits.state = GS_CONTROL_STATE_ABORT;
        break;
      case BOARD_COMMAND_CODE_ACK:
        sendBoardCommand();
        sendACKResponse();
        break;
      case BOARD_COMMAND_CODE_RESET:
        if (gsControl.status.bits.state == GS_CONTROL_STATE_ABORT) {
          gsControl.status.bits.state = GS_CONTROL_STATE_IDLE;
          sendBoardCommand();
        }
        break;
      case BOARD_COMMAND_CODE_UNSAFE:
        if (checkUnsafe()) {
          sendBoardCommand();
        }
        break;
      case ENGINE_COMMAND_CODE_OPEN_VALVE:
        sendBoardCommand();
        break;
      /*case FILLING_STATION_COMMAND_ALLOW_FILL:
        if (checkAllowFill()) {
          sendBoardCommand();
        }*/
      default:
        break;
    }
    gsControl.usb->status.bits.rxDataReady = 0;
  }
}

void handleCommunication(uint32_t timestamp_ms) {
  if (gsControl.communicationTimestampTarget_ms <= timestamp_ms) {
    sendStatusPacket(timestamp_ms);
    gsControl.communicationTimestampTarget_ms = timestamp_ms + GS_CONTROL_DELAY_BETWEEN_STATUS_PACKETS_MS;
  }
}

void sendStatusPacket(uint32_t timestamp_ms) {
  currentGSControlStatusPacket.fields.status.value = gsControl.status.value;
  currentGSControlStatusPacket.fields.errorStatus.value = gsControl.errorStatus.value;
  currentGSControlStatusPacket.fields.timestamp_ms = timestamp_ms;
  currentGSControlStatusPacket.fields.crc = 0; // CRC

  gsControl.usb->transmit((struct USB*)gsControl.usb, currentGSControlStatusPacket.data, sizeof(GSControlStatusPacket));
}

void sendBoardCommand() {
  HAL_UART_Transmit_DMA((UART_HandleTypeDef*)gsControl.uart->externalHandle, currentBoardCommand.data, sizeof(BoardCommand));
}

void sendACKResponse() {
  CommandResponse commandResponse = {
    .fields = {
      .header = {
        .bits = {
          .type = COMMAND_RESPONSE_TYPE_CODE,
          .boardId = TELEMETRY_GS_CONTROL_BOARD_ID,
          .commandIndex = currentBoardCommand.fields.header.bits.commandIndex,
          .response = RESPONSE_CODE_OK
        }
      },
      .crc = 0 // CRC
    }
  };

  gsControl.usb->transmit((struct USB*)gsControl.usb, commandResponse.data, sizeof(CommandResponse));
}

uint8_t checkUnsafe() {
  uint8_t result = 0;
  if (gsControl.status.bits.state != GS_CONTROL_STATE_ABORT) {
    result = 1;
  }
  return result;
}

uint8_t checkAllowFill() {
  uint8_t result = 0;
  if (gsControl.status.bits.state != GS_CONTROL_STATE_ABORT && 
      gsControl.status.bits.isUnsafeKeySwitchPressed) {
    result = 1;
  }
  return result;
}

void parseUartPacket() {
  switch (telemetryPacketType)
  {
    case COMMAND_RESPONSE_TYPE_CODE:
      parseCommandResponsePacket();
      break;
    case TELEMETRY_TYPE_CODE:
      parseTelemetryPacket(telemetryPacketHeader);
      break;
    case STATUS_TYPE_CODE:
      parseStatusPacket(telemetryPacketHeader);
      break;
    default:
      // RAISE ERROR FLAG
      break;
  }
}

uint8_t parseUsbPacket() {
  uint8_t successfullyParsed = 0;
  
  //commandPacketHeader = gsControl.usb->rxBuffer[0] | gsControl.usb->rxBuffer[1] << 8 | gsControl.usb->rxBuffer[2] << 16 | gsControl.usb->rxBuffer[3] << 24;
  currentBoardCommand.data[0] = gsControl.usb->rxBuffer[0];
  currentBoardCommand.data[1] = gsControl.usb->rxBuffer[1];
  currentBoardCommand.data[2] = gsControl.usb->rxBuffer[2];
  currentBoardCommand.data[3] = gsControl.usb->rxBuffer[3];
  
  if (currentBoardCommand.fields.header.bits.type == BOARD_COMMAND_TYPE_CODE) {
    // CHECK CRC
    parseBoardCommandPacket();
    successfullyParsed = 1;
  }
  return successfullyParsed;
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
  {
    // Handle UART RX complete callback
    //HAL_UART_Transmit_DMA(&huart1, uartRxBuffer, sizeof(uartRxBuffer)); // Echo back received data
    gsControl.usb->transmit((struct USB*)gsControl.usb, uartBuffer, sizeof(uartBuffer));
    HAL_UART_Receive_DMA(&huart, uartBuffer, sizeof(uartBuffer));
  }
}

/*void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART1) {
    telemetryPacketHeader = uartBuffer[0] & uartBuffer[1] << 8 & uartBuffer[2] << 16 & uartBuffer[3] << 24;
    telemetryPacketType = telemetryPacketHeader & 0xFFFFF000UL;
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
    HAL_UART_Receive_IT((UART_HandleTypeDef*)gsControl.telecommunication->uart->externalHandle, uartBuffer, sizeof(uartBuffer));
  }
}*/