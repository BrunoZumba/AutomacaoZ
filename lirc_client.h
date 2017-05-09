/****************************************************************************
** lirc_client.h ***********************************************************
****************************************************************************
*
* Copyright (C) 1998 Trent Piepho <xyzzy@u.washington.edu>
* Copyright (C) 1998 Christoph Bartelmus <lirc@bartelmus.de>
*
*/

#ifndef LIRC_CLIENT_H
#define LIRC_CLIENT_H

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "lirc/paths.h"
#include "lirc/lirc_config.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef __u32
typedef uint32_t __u32;
#endif

#define LIRC_RET_SUCCESS  (0)
#define LIRC_RET_ERROR   (-1)

#define LIRC_ALL ((char*)(-1))


enum lirc_flags { none = 0x00,
                once = 0x01,
                quit = 0x02,
                mode = 0x04,
                ecno = 0x08,
                startup_mode = 0x10,
                toggle_reset = 0x20, };

struct lirc_list {
        char*                   string;
        struct lirc_list*       next;
};

struct lirc_code {
        char*                   remote;
        char*                   button;
        struct lirc_code*       next;
};

struct lirc_config {
        char*                           lircrc_class;
        char*                           current_mode;
        struct lirc_config_entry*       next;
        struct lirc_config_entry*       first;

        int                             sockfd;
};

struct lirc_config_entry {
        char*                           prog;
        struct lirc_code*               code;
        unsigned int                    rep_delay;
        unsigned int                    ign_first_events;
        unsigned int                    rep;
        struct lirc_list*               config;
        char*                           change_mode;
        unsigned int                    flags;

        char*                           mode;
        struct lirc_list*               next_config;
        struct lirc_code*               next_code;

        struct lirc_config_entry*       next;
};

typedef struct {
        char    packet[PACKET_SIZE + 1];
        char    buffer[PACKET_SIZE + 1];
        char    reply[PACKET_SIZE + 1];
        int     head;
        int     reply_to_stdout;
        char*   next;
} lirc_cmd_ctx;

int lirc_init(const char* prog, int verbose);

int lirc_deinit(void);

int lirc_readconfig(const char* path,
                    struct lirc_config** config,
                    int (check) (char* s));

void lirc_freeconfig(struct lirc_config* config);

char* lirc_nextir(void);

char* lirc_ir2char(struct lirc_config* config, char* code);

int lirc_nextcode(char** code);

int lirc_code2char(struct lirc_config* config, char* code, char** string);


/* new interface for client daemon */
int lirc_readconfig_only(const char* file,
                        struct lirc_config** config,
                        int (check) (char* s));

int lirc_code2charprog(struct lirc_config* config,
                        char* code,
                        char** string,
                        char** prog);

size_t lirc_getsocketname(const char* id, char* buf, size_t size);

const char* lirc_getmode(struct lirc_config* config);

const char* lirc_setmode(struct lirc_config* config, const char* mode);

/* 0.9.2: New interface for sending data. */

int lirc_command_init(lirc_cmd_ctx* ctx, const char* fmt, ...);


int lirc_command_run(lirc_cmd_ctx* ctx, int fd);

void lirc_command_reply_to_stdout(lirc_cmd_ctx* ctx);

int lirc_send_one(int fd, const char* remote, const char* keysym);


int lirc_simulate(int fd,
                const char* remote,
                const char* keysym,
                int scancode,
                int repeat);


int lirc_get_remote_socket(const char* address, int port, int quiet);


int lirc_get_local_socket(const char* path, int quiet);


#ifdef __cplusplus
}
#endif

#endif
