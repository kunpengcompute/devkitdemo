# Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.

CUR_DIR=$(shell pwd)
KUNPENG_SC_DEVEL_PATH=/usr/local/kunpeng-sc-devel
ITRUSTEE_BUILD_PATH=${KUNPENG_SC_DEVEL_PATH}/source/itrustee_sdk

TARGET_APP := certmanager

APP_SOURCES := ./cert_file.c
APP_SOURCES += ./cert_manager.c

APP_SOURCES += $(ITRUSTEE_BUILD_PATH)/src/CA/libteec_adaptor.c

APP_CFLAGS += -fstack-protector-strong -fPIC

APP_CFLAGS += -I$(ITRUSTEE_BUILD_PATH)/include/CA
APP_CFLAGS += -I${KUNPENG_SC_DEVEL_PATH}/source/libboundscheck/include

APP_LDFLAGS += -ldl -lpthread -lboundscheck

APP_LDFLAGS += -z text -z now -z relro -z noexecstack -pie -s

APP_OBJECTS := $(APP_SOURCES:.c=.o)
$(TARGET_APP): $(APP_SOURCES)
	$(CC) $(APP_CFLAGS) -o $@ $(APP_SOURCES) $(APP_LDFLAGS)

clean:
	@rm -f *.o $(TARGET_APP)
