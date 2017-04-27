PACKAGES_DIR=../../packages
#To use awa communication
#CONTIKI_PROJECT=color2_n_awa

#To use UART to communicate with ci40-MIKROBUS_2
#To use this project you have to set USE_SERIAL_PADS=1 and edit contiki-mikro-e-main.c which is located in
#contiki/platform/mikro-e search for line: #define UART_DEBUG_BAUDRATE 115200
#replace with: #define UART_DEBUG_BAUDRATE 57600
#make clean, make
#CONTIKI_PROJECT=color2_n_uart

#To use UDP communication with ci40
#To have this working please go to project-conf.h and uncomment line: #define UIP_CONF_UDP 1
#Communication is done on port 9955, please make sure you have set GLOBAL_IPv6_ADDR* fields to match your board
#GLOBAL_IPv6_ADDR8 should be 0, it's changed to 1 in source code.
CONTIKI_PROJECT=color2_n_udp

VERSION?=$(shell git describe --abbrev=4 --dirty --always --tags)
CONTIKI=../../contiki
CONTIKI_WITH_IPV6=1
CONTIKI_WITH_RPL=0
USE_SERIAL_PADS=1
LWM2M_DIR=$(PACKAGES_DIR)/AwaLWM2M
TINYDTLS_DIR=tinydtls
TARGET=mikro-e
CONTIKI_WITH_IPV6 = 1
CONTIKI_WITH_RPL = 0
RESOLV_CONF_SUPPORTS_MDNS = 0
UIP_CONF_TCP = 1
TINYDTLS = 1
USE_SERIAL_PADS = 1

NDEBUG = 1

USE_CA8210=1
#USE_CC2520=1

#ds-mono marek
#DS_SERVER=coaps://ds-mono.flowcloud.systems:15684
#--1
#PSK_IDENTITY=7JfVmNURrEei4ORdIbwQSw
#PSK_SECRET_KEY=CCE3E6793122F4DD071D0BDB905A17011FF160889B705E76F3A47ABD367EFFDF
#--2
#PSK_IDENTITY=BO8dStpREUGdGwDRrxgC9w
#PSK_SECRET_KEY=CCFB762FE648FAC7568544FA22FF5F1C3941F5707F7CA6D790A72DCD91B3BDCE


#public DS
PSK_IDENTITY=LqeRRk8KAUOX65rPfgbzMQ
PSK_SECRET_KEY=39A5272C3BF161287E06D1E1C7E7F3972CBB72289266052F25331B928E37983B
DS_SERVER=coaps://deviceserver.creatordev.io:15684

ifeq ($(PSK_IDENTITY)x,x)
$(error Please define PSK_IDENTITY)
endif
ifeq ($(PSK_SECRET_KEY)x,x)
$(error Please define PSK_SECRET_KEY)
endif
PSK_BINARY_KEY:=$(shell echo $(PSK_SECRET_KEY) | sed -e 's.\([0-9A-F][0-9A-F]\).0x\1 .g' -e 's. 0x.,0x.g')

CFLAGS += '-DIDENTITY=\"$(PSK_IDENTITY)\"' '-DSECRET_KEY="$(PSK_SECRET_KEY)"' '-DBINARY_KEY=$(PSK_BINARY_KEY)'
CFLAGS += '-DDS_SERVER=\"$(DS_SERVER)\"'
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\" -DVERSION='\"$(VERSION)\"'
CFLAGS += -Wall -Wno-pointer-sign
CFLAGS += -I$(CONTIKI)/platform/$(TARGET)
CFLAGS += -I$(LWM2M_DIR)/api/include -DLWM2M_CLIENT -g -DDTLS_PEERS_NOHASH=1
CFLAGS += -DUIP_CONF_BUFFER_SIZE=4096
CFLAGS += -DDTLS_MAX_BUF=1000

ifneq (,$(filter  $(TARGET),seedeye mikro-e))
  CFLAGS += -fno-short-double
  LDFLAGS += -Wl,--defsym,_min_heap_size=64000
endif

APPS += erbium
APPS += er-coap
APPS += rest-engine
APPS += jsmn
APPS += b64
#APPS += hmac
APPS += client
APPS += common
APPS += tinydtls/aes tinydtls/sha2 tinydtls/ecc tinydtls

APPDIRS += $(CONTIKI)/platform/mikro-e/dev
APPDIRS += $(PACKAGES_DIR)
APPDIRS += $(LWM2M_DIR)/lib
APPDIRS += $(LWM2M_DIR)/core/src
APPDIRS += $(TINYDTLS_DIR)

MODULE += core/letmecreate

all: $(CONTIKI_PROJECT)
	xc32-bin2hex $(CONTIKI_PROJECT).$(TARGET)

distclean: cleanall

cleanall:
	rm -f $(CONTIKI_PROJECT).hex
	rm -f symbols.*

include $(CONTIKI)/Makefile.include
