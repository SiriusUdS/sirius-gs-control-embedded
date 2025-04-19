#include "../../../Inc/Device/Telecommunication/Telecommunication.h"

void TELECOM_init(Telecommunication* instance){
  instance->receiveData = (Telecommunication_receiveData)TELECOM_receiveData;
  instance->sendData = (Telecommunication_sendData)TELECOM_sendData;
  instance->setupTelecom = (Telecommunication_setup)TELECOM_setup;
}

void TELECOM_sendData(Telecommunication* instance, uint8_t* data, uint16_t size){
  instance->uart->transmit((struct UART*)instance->uart, data, size);
}


void TELECOM_receiveData(Telecommunication* instance, uint8_t* data, uint16_t size){
  instance->uart->receive((struct UART*)instance->uart, data, size);
}


void TELECOM_setup(Telecommunication* instance){
  uint8_t cmdmode[] = "+++++";
  instance->uart->transmit((struct UART*)instance->uart, cmdmode, sizeof(cmdmode) - 1);
  uint8_t rec[3] = "00";
  //HAL_Delay(1200); // GUARDIAN TIME DATASHEET
  /*while(1){
    instance->uart->receive((struct UART)instance, rec, 3);
    if(rec[0] == 'O'){
      break;
    }
  }*/

  uint8_t index = 0;
  uint8_t atCommands[3][10] = {"ATDH0000\r", "ATDLFFFF\r", "ATID2015\r"};
  for(uint8_t i=0; i < 3; i++){
    rec[0] = '0';
    rec[1] = '0';
    /*for(uint8_t j=0; j < 2; j++){
      atCommands[i][j+4] = setup.data[index++];
    }*/
    //atCommands[i][7] = '\0';
    instance->uart->transmit((struct UART*)instance->uart, atCommands[i], 9);

    HAL_Delay(200);
    instance->uart->receive((struct UART*)instance, rec, 3);
  }

  uint8_t finalCommand[2][6] = {"ATWR\r", "ATCN\r"};

  for(uint8_t i =0; i < 2; i++){
    instance->uart->transmit((struct UART*)instance->uart, finalCommand[i], 6);
  }
}