# Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
include ./config.mk
KUNPENG_SC_DEVEL_PATH=/usr/local/kunpeng-sc-devel
ITRUSTEE_BUILD_PATH=${KUNPENG_SC_DEVEL_PATH}/source/itrustee_sdk
include ${ITRUSTEE_BUILD_PATH}/build/mk/common.mk

# set the compilation tool chain example : export CC=path_to_gcc ; export LD=path_to_ld

SRC += $(wildcard src/*.c)

# set header directory
INCLUDEDIR += -I${CUR_DIR}/include
INCLUDEDIR += -I${CUR_DIR}/src
INCLUDEDIR += -I${ITRUSTEE_BUILD_PATH}/include/TA
INCLUDEDIR += -I${KUNPENG_SC_DEVEL_PATH}/source/libboundscheck/include

# set target
COBJS := $(SRC:%.c=%.o)
TARGET = $(COBJS)

sec_binary:combine
	python3 -B ${SIGNTOOL_DIR}/signtool_v3.py ${CUR_DIR} ${CUR_DIR} --privateCfg ${CUR_DIR}/config_cloud.ini

combine: $(TARGET)
	$(LD) $(LDFLAGS) $(TARGET) $(EXTRAO) -o libcombine.so
	bash $(ITRUSTEE_BUILD_PATH)/build/tools/ta_entry_check.sh $(READELF) $(shell pwd)/libcombine.so n y $(TARGET_IS_ARM64)

src/%.o: ./src/%.c
	$(CC) $(CFLAGS)  $(INCLUDEDIR) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS)  $(INCLUDEDIR) -c $< -o $@

clean:
	rm -f $(COBJS) *.so *.sec
