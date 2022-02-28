#
# Copyright 2017, Huawei Co. Ltd.
#
#
# API_LEVEL which indicates the GP API version of TA
# API_LEVEL=1 indicates GP 1.0 which is the current version of itrustee
# API_LEVEL=2 indicates GP 1.1.1 which is the current version of the partner
# API_LEVEL=3 indicates GP 1.2 which is the version we both going to support
# If no API_LEVEL is specified, API of GP 1.0 will be taked
CFLAGS += -DAPI_LEVEL=1
TARGET_IS_ARM64=y
