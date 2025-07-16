#include "../Inc/GSControl.h"

static GSControl gsControl = {0};

BoardCommand currentReceivedBoardCommand = {0};
CommandResponse currentResponse = {0};

uint32_t lastReceivedGSCommandTimestamp_ms = 0;

BoardCommand currentCommand = {
  .fields = {
    .header = {
      .bits = {
        .type = BOARD_COMMAND_UNICAST_TYPE_CODE,
        .commandIndex = 0,
        .boardId = GS_CONTROL_BOARD_ID,
        .commandCode = BOARD_COMMAND_CODE_SAFE
      }
    },
    .value = 0,
    .padding = {0},
    .crc = 0
  }
};

BoardCommand safeCommand = {
  .fields = {
    .header = {
      .bits = {
        .type = BOARD_COMMAND_BROADCAST_TYPE_CODE,
        .commandIndex = 0,
        .boardId = 0,
        .commandCode = BOARD_COMMAND_CODE_SAFE
      }
    },
    .value = 0,
    .padding = {0},
    .crc = 0
  }
};

BoardCommand unsafeCommand = {
  .fields = {
    .header = {
      .bits = {
        .type = BOARD_COMMAND_BROADCAST_TYPE_CODE,
        .commandIndex = 0,
        .boardId = 0,
        .commandCode = BOARD_COMMAND_CODE_UNSAFE
      }
    },
    .value = 0,
    .padding = {0},
    .crc = 0
  }
};

BoardCommand igniteCommand = {
  .fields = {
    .header = {
      .bits = {
        .type = BOARD_COMMAND_UNICAST_TYPE_CODE,
        .commandIndex = 0,
        .boardId = ENGINE_BOARD_ID,
        .commandCode = ENGINE_COMMAND_CODE_FIRE_IGNITER
      }
    },
    .value = 0,
    .padding = {0},
    .crc = 0
  }
};

BoardCommand launchCommand = {
  .fields = {
    .header = {
      .bits = {
        .type = BOARD_COMMAND_UNICAST_TYPE_CODE,
        .commandIndex = 0,
        .boardId = ENGINE_BOARD_ID,
        .commandCode = ENGINE_COMMAND_CODE_OPEN_VALVE
      }
    },
    .value = 0,
    .padding = {0},
    .crc = 0
  }
};

GSControlStatusPacket currentGSControlStatusPacket = {
  .fields = {
    .header = {
      .bits = {
        .type = STATUS_TYPE_CODE,
        .boardId = GS_CONTROL_BOARD_ID,
        .RESERVED = 0
      }
    },
    .errorStatus = 0,
    .status = 0,
    .lastReceivedGSCommandTimestamp_ms = 0,
    .padding = {0},
    .timestamp_ms = 0,
    .crc = 0
  }
};

uint8_t uartBuffer[GS_CONTROL_TELECOMMUNICATION_UART_BUFFER_SIZE] = {0};

static void executeInit(uint32_t timestamp_ms);
static void executeSafe(uint32_t timestamp_ms);
static void executeUnsafe(uint32_t timestamp_ms);
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

static void handleCurrentCommand(uint32_t timestamp_ms);
static void sendReceivedBoardCommand();
static void sendSafeCommand(uint32_t timestamp_ms);
static void sendUnsafeCommand(uint32_t timestamp_ms);
static void sendIgniteCommand(uint32_t timestamp_ms);
static void sendLaunchCommand(uint32_t timestamp_ms);
static void sendACKResponse();

static uint8_t checkUnsafe();

static uint8_t parseUsbPacket();
static uint8_t parseBoardCommandPacket();

void GSControl_init(GPIO* gpios, UART* uart, volatile USB* usb, Telecommunication* telecom, Button* buttons, CRC_HandleTypeDef* hcrc) {
  gsControl.errorStatus.value  = 0;
  gsControl.status.value       = 0;
  gsControl.currentState       = GS_CONTROL_STATE_INIT;
  
  gsControl.gpios  = gpios;
  gsControl.uart   = uart;
  gsControl.usb    = usb;
  gsControl.hcrc   = hcrc;

  gsControl.telecommunication = telecom;
  gsControl.buttons = buttons;

  gsControl.communicationTimestampTarget_ms = 0;
  gsControl.commandTimestampTarget_ms = 0;


  gsControl.uartRxHalfReady = 0;
  gsControl.uartRxCpltReady = 0;

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
  
  updateButtonStates();
  handleCurrentCommand(timestamp_ms);
  handleIncomingCommand();
  handleIncomingPackets();
  handleCommunication(timestamp_ms);
  GSControl_execute(timestamp_ms);
}

void GSControl_execute(uint32_t timestamp_ms) {
  switch (gsControl.currentState) {
    case GS_CONTROL_STATE_INIT:
      executeInit(timestamp_ms);
      break;
    case GS_CONTROL_STATE_SAFE:
      executeSafe(timestamp_ms);
      break;
    case GS_CONTROL_STATE_UNSAFE:
      executeUnsafe(timestamp_ms);
      break;
    case GS_CONTROL_STATE_ABORT:
      executeAbort(timestamp_ms);
      break;
    default:
      gsControl.errorStatus.bits.invalidState = 1;
      executeSafe(timestamp_ms);
      break;
  }
}

void executeInit(uint32_t timestamp_ms) {
  gsControl.telecommunication->config((struct Telecommunication*) gsControl.telecommunication);
  HAL_UART_Receive_DMA((UART_HandleTypeDef*)gsControl.uart->externalHandle, uartBuffer, sizeof(uartBuffer));
  gsControl.currentState = GS_CONTROL_STATE_SAFE;
}

void executeSafe(uint32_t timestamp_ms) {
  if (gsControl.status.bits.isEmergencyStopButtonPressed) {
    sendUnsafeCommand(timestamp_ms);
    gsControl.currentState = GS_CONTROL_STATE_UNSAFE;
  }
}

void executeUnsafe(uint32_t timestamp_ms) {
  if (!gsControl.status.bits.isEmergencyStopButtonPressed) {
    sendSafeCommand(timestamp_ms);
    gsControl.currentState = GS_CONTROL_STATE_SAFE;
    return;
  }

  if (gsControl.status.bits.isFireIgniterButtonPressed) {
    if (gsControl.status.bits.isArmIgniterSwitchOn) {
      if (!gsControl.status.bits.isAllowFillSwitchOn && !gsControl.status.bits.isAllowDumpSwitchOn) {
        sendIgniteCommand(timestamp_ms);
        return;
      }
    }
  }

  if (gsControl.status.bits.isValveStartButtonPressed) {
    if (gsControl.status.bits.isArmServoSwitchOn) {
      if (!gsControl.status.bits.isAllowFillSwitchOn && !gsControl.status.bits.isAllowDumpSwitchOn) {
        sendLaunchCommand(timestamp_ms);
        return;
      }
    }
  }

  if (currentReceivedBoardCommand.fields.header.bits.commandCode != BOARD_COMMAND_CODE_ABORT) {
    sendUnsafeCommand(timestamp_ms);
    return;
  }
  sendSafeCommand(timestamp_ms);
  gsControl.currentState = GS_CONTROL_STATE_ABORT;
}

void executeAbort(uint32_t timestamp_ms) {
  // wait for reset command
}

void handleCurrentCommand(uint32_t timestamp_ms) {
  if (gsControl.commandTimestampTarget_ms <= timestamp_ms && gsControl.currentState != GS_CONTROL_STATE_ABORT) {
    //if (timestamp_ms - lastReceivedGSCommandTimestamp_ms < GS_CONTROL_BOARD_COMMAND_DURATION_MS) {
    //  return;
    //}
    gsControl.commandTimestampTarget_ms = timestamp_ms + GS_CONTROL_DELAY_BETWEEN_COMMANDS_MS;
    HAL_UART_Transmit_DMA((UART_HandleTypeDef*)gsControl.uart->externalHandle, currentCommand.data, sizeof(BoardCommand));
  }
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

void handleIncomingPackets() {
  if (gsControl.uartRxHalfReady == 1) {
    gsControl.usb->transmit((struct USB*)gsControl.usb, uartBuffer, sizeof(uartBuffer) / 2);
    gsControl.uartRxHalfReady = 0;
  }

  if (gsControl.uartRxCpltReady == 1) {
    gsControl.usb->transmit((struct USB*)gsControl.usb, uartBuffer + (sizeof(uartBuffer) / 2), sizeof(uartBuffer) / 2);
    gsControl.uartRxCpltReady = 0;
  }
}

void handleIncomingCommand() {
  if (gsControl.usb->status.bits.rxDataReady) {
    if (!parseUsbPacket()) {
      gsControl.usb->status.bits.rxDataReady = 0;
      return;
    }

    switch (currentReceivedBoardCommand.fields.header.bits.commandCode)
    {
      case BOARD_COMMAND_CODE_ABORT:
        sendReceivedBoardCommand();
        gsControl.status.bits.state = GS_CONTROL_STATE_ABORT;
        break;
      case BOARD_COMMAND_CODE_ACK:
        sendReceivedBoardCommand();
        sendACKResponse();
        break;
      case BOARD_COMMAND_CODE_RESET:
        if (checkUnsafe()) {
          if (gsControl.status.bits.state == GS_CONTROL_STATE_ABORT) {
            gsControl.status.bits.state = GS_CONTROL_STATE_SAFE;
            sendReceivedBoardCommand();
          }
        }
        break;
      case FILLING_STATION_COMMAND_CODE_OPEN_DUMP_VALVE_PCT:
        if (checkUnsafe() && gsControl.status.bits.isAllowDumpSwitchOn) {
          sendReceivedBoardCommand();
        }
        break;
      case FILLING_STATION_COMMAND_CODE_OPEN_FILL_VALVE_PCT:
        if (checkUnsafe() && gsControl.status.bits.isAllowFillSwitchOn) {
          sendReceivedBoardCommand();
        }
        break;
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
  currentGSControlStatusPacket.fields.lastReceivedGSCommandTimestamp_ms = lastReceivedGSCommandTimestamp_ms;
  currentGSControlStatusPacket.fields.lastBoardSentCommandCode = currentCommand.fields.header.bits.commandCode;
  currentGSControlStatusPacket.fields.crc = HAL_CRC_Calculate((CRC_HandleTypeDef*)gsControl.hcrc, currentGSControlStatusPacket.data32, (sizeof(GSControlStatusPacket) / sizeof(uint32_t)) - sizeof(uint8_t));
  gsControl.usb->transmit((struct USB*)gsControl.usb, currentGSControlStatusPacket.data, sizeof(GSControlStatusPacket));
}

void sendReceivedBoardCommand() {
  lastReceivedGSCommandTimestamp_ms = HAL_GetTick();
  HAL_UART_Transmit_DMA((UART_HandleTypeDef*)gsControl.uart->externalHandle, currentReceivedBoardCommand.data, sizeof(BoardCommand));
}

void sendSafeCommand(uint32_t timestamp_ms) {
  currentCommand = safeCommand;
  currentCommand.fields.crc = HAL_CRC_Calculate((CRC_HandleTypeDef*)gsControl.hcrc, currentCommand.data32, (sizeof(BoardCommand) / sizeof(uint32_t)) - 1);
}

void sendUnsafeCommand(uint32_t timestamp_ms) {
  currentCommand = unsafeCommand;
  currentCommand.fields.crc = HAL_CRC_Calculate((CRC_HandleTypeDef*)gsControl.hcrc, currentCommand.data32, (sizeof(BoardCommand) / sizeof(uint32_t)) - 1);
}

void sendIgniteCommand(uint32_t timestamp_ms) {
  currentCommand = igniteCommand;
  currentCommand.fields.crc = HAL_CRC_Calculate((CRC_HandleTypeDef*)gsControl.hcrc, currentCommand.data32, (sizeof(BoardCommand) / sizeof(uint32_t)) - 1);
}

void sendLaunchCommand(uint32_t timestamp_ms) {
  currentCommand = launchCommand;
  currentCommand.fields.crc = HAL_CRC_Calculate((CRC_HandleTypeDef*)gsControl.hcrc, currentCommand.data32, (sizeof(BoardCommand) / sizeof(uint32_t)) - 1);
}

void sendACKResponse() {
  CommandResponse commandResponse = {
    .fields = {
      .header = {
        .bits = {
          .type = COMMAND_RESPONSE_TYPE_CODE,
          .boardId = GS_CONTROL_BOARD_ID,
          .commandIndex = currentReceivedBoardCommand.fields.header.bits.commandIndex,
          .response = RESPONSE_CODE_OK
        }
      },
      .crc = 0
    }
  };

  commandResponse.fields.crc = HAL_CRC_Calculate((CRC_HandleTypeDef*)gsControl.hcrc, commandResponse.data32, (sizeof(CommandResponse) / sizeof(uint32_t)) - sizeof(uint8_t));

  gsControl.usb->transmit((struct USB*)gsControl.usb, commandResponse.data, sizeof(CommandResponse));
}

uint8_t checkUnsafe() {
  return gsControl.status.bits.isEmergencyStopButtonPressed == 1;
}

uint8_t parseUsbPacket() {
  uint8_t successfullyParsed = 0;
  
  currentReceivedBoardCommand.data[0] = gsControl.usb->rxBuffer[0];
  currentReceivedBoardCommand.data[1] = gsControl.usb->rxBuffer[1];
  currentReceivedBoardCommand.data[2] = gsControl.usb->rxBuffer[2];
  currentReceivedBoardCommand.data[3] = gsControl.usb->rxBuffer[3];
  
  if (currentReceivedBoardCommand.fields.header.bits.type == BOARD_COMMAND_UNICAST_TYPE_CODE ||
      currentReceivedBoardCommand.fields.header.bits.type == BOARD_COMMAND_BROADCAST_TYPE_CODE) {
    parseBoardCommandPacket();
    successfullyParsed = 1;
  }
  return successfullyParsed;
}

uint8_t parseBoardCommandPacket() {
  for (uint8_t i = 0; i < sizeof(BoardCommand); i++) {
    currentReceivedBoardCommand.data[i] = gsControl.usb->rxBuffer[i];
  }
  return 1; // CRC
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

void initTelecom() {
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if(huart->Instance == USART1) {
    gsControl.uartRxCpltReady = 1;
  }
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
  if(huart->Instance == USART1) {
    gsControl.uartRxHalfReady = 1;
  }
}