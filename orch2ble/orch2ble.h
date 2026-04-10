#pragma once

#include <stdint.h>

enum ipc_kind {
  IPC_KIND_ASK = 1,
  IPC_KIND_REPLY = 2,
  IPC_KIND_EVENT = 3
};

enum ipc_type {
  IPC_HANDSHAKE_HELLO = 1,    // запрос
  IPC_HANDSHAKE_WELCOME = 2,  // ответ
  IPC_COMMAND_CONNECT = 3,
  IPC_RESULT_CONNECTED = 4,
  IPC_COMMAND_SUBSCRIBE = 5,
  IPC_RESULT_SUBSCRIBED = 6,
  IPC_EVENT_NOTIFY = 7,
  IPC_EVENT_ERROR = 8,
  IPC_COMMAND_DISCONNECT = 9,
  IPC_RESULT_DISCONNECT = 10,
  IPC_RESULT_FAIL = 11,
};

enum ipc_fail {
  IPC_FAIL_INTERNAL = 1,
  IPC_FAIL_HARDWARE = 2
};

enum ipc_error {
  IPC_ERROR_HARDWARE = 1
}

enum { IPC_PAYLOAD_MAX = 64u * 1024u }; 
enum { IPC_PAYLOAD_MIN = 64u };

enum { IPC_CODE = 34252343u /* CUBE */};

enum { IPC_MSG_SIZE = 56u };

// Это и есть то, что мы хотим передавать?
typedef struct ipc_msg_s {
  uint32_t session_id;
  uint32_t id;
  uint32_t reply_to_id;
  uint8_t  ipc_kind;
  uint16_t ipc_type;
  uint16_t ipc_code;  // только для ошибок, остальным 0
  char * payload;
  uint64_t payload_size;
} ipc_msg_t; 
