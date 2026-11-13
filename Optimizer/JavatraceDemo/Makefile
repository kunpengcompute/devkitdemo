# ============================================================
# JavatraceDemo Makefile
#   目标：all  bad  good  clean  run  run-bad  run-good
#   切换 bad/good 需重新编译 .so（库名固定为 liborder_pipeline）
# ============================================================

# --- 自动探测 JAVA_HOME ---
ifndef JAVA_HOME
  JAVA_HOME := $(shell java -XshowSettings:properties -version 2>&1 | grep 'java.home' | awk '{print $$3}' | sed 's|/jre$$||')
endif
ifndef JAVA_HOME
  # 常见路径兜底
  ifneq ($(wildcard /usr/lib/jvm/default-java),)
    JAVA_HOME := /usr/lib/jvm/default-java
  else ifneq ($(wildcard /usr/lib/jvm/java),)
    JAVA_HOME := /usr/lib/jvm/java
  else ifneq ($(wildcard /opt/java),)
    JAVA_HOME := /opt/java
  else ifneq ($(wildcard C:/Program\ Files/Java/*),)
    JAVA_HOME := $(shell ls -d /c/Program\ Files/Java/* 2>/dev/null | head -n1)
  endif
endif
ifeq ($(JAVA_HOME),)
  $(error JAVA_HOME is not set and auto-detection failed. Please set it manually.)
endif

# --- JDK include 路径 ---
JDK_INC := $(JAVA_HOME)/include
UNAME_S := $(shell uname -s 2>/dev/null || echo Windows)
ifeq ($(UNAME_S),Linux)
  JDK_INC_PLAT := $(JDK_INC)/linux
  SO_EXT := so
else ifeq ($(UNAME_S),Darwin)
  JDK_INC_PLAT := $(JDK_INC)/darwin
  SO_EXT := dylib
else
  # Windows (MinGW / Git Bash)
  JDK_INC_PLAT := $(JDK_INC)/win32
  SO_EXT := dll
endif

# --- 编译器 ---
CXX      ?= g++
CXXFLAGS ?= -g -O0 -fPIC -fno-inline -fno-omit-frame-pointer \
            -fno-optimize-sibling-calls -pthread -shared \
            -I$(JDK_INC) -I$(JDK_INC_PLAT)

JAVAC    ?= $(JAVA_HOME)/bin/javac
JAVA     ?= $(JAVA_HOME)/bin/java

# --- 产物 ---
BAD_SRC  := order_pipeline_bad.cpp
GOOD_SRC := order_pipeline_good.cpp
SO_NAME  := liborder_pipeline.$(SO_EXT)
JAVA_SRCS := NativeBridge.java OrderPipelineDemo.java
JAVA_CLASSES := NativeBridge.class OrderPipelineDemo.class

# --- 默认目标：编译 good 版（对照版） ---
.PHONY: all bad good clean run run-bad run-good help

all: good

help:
	@echo "Targets:"
	@echo "  all       编译 good 版 .so + Java"
	@echo "  bad       编译 bad 版 .so + Java（锁竞争 + p99 长尾）"
	@echo "  good      编译 good 版 .so + Java（对照：锁外计算 + 无 usleep）"
	@echo "  clean     删除所有产物"
	@echo "  run       用当前 .so 跑 60 秒 / 4 worker"
	@echo "  run-bad   切到 bad 版并运行"
	@echo "  run-good  切到 good 版并运行"
	@echo ""
	@echo "Current JAVA_HOME = $(JAVA_HOME)"

# --- Java 编译 ---
$(JAVA_CLASSES): $(JAVA_SRCS)
	@echo "[javac] $(JAVA_SRCS)"
	$(JAVAC) -Xlint:all $(JAVA_SRCS)

# --- bad 版 ---
bad: $(SO_NAME)-bad
	@cp -f $(SO_NAME)-bad $(SO_NAME)
	@echo "[switch] -> bad: $(SO_NAME)"
$(SO_NAME)-bad: $(BAD_SRC) $(JAVA_CLASSES)
	@echo "[g++] $(BAD_SRC) -> $(SO_NAME)-bad"
	$(CXX) $(CXXFLAGS) -o $@ $<

# --- good 版 ---
good: $(SO_NAME)-good
	@cp -f $(SO_NAME)-good $(SO_NAME)
	@echo "[switch] -> good: $(SO_NAME)"
$(SO_NAME)-good: $(GOOD_SRC) $(JAVA_CLASSES)
	@echo "[g++] $(GOOD_SRC) -> $(SO_NAME)-good"
	$(CXX) $(CXXFLAGS) -o $@ $<

# --- 切换：把选中的版本复制为统一的 liborder_pipeline ---
switch-bad: bad
	@cp -f $(SO_NAME)-bad $(SO_NAME)
	@echo "[switch] -> bad: $(SO_NAME)"

switch-good: good
	@cp -f $(SO_NAME)-good $(SO_NAME)
	@echo "[switch] -> good: $(SO_NAME)"

# --- 清理 ---
clean:
	@rm -f $(SO_NAME) $(SO_NAME)-bad $(SO_NAME)-good $(JAVA_CLASSES)
	@rm -f hs_err_pid*.log
	@echo "[clean] done"

# --- 运行：目录必须与 .class / .so 同目录 ---
run:
	@test -f $(SO_NAME) || (echo "No $(SO_NAME) found. Run 'make run-bad' or 'make run-good' first." && exit 1)
	@echo "[run] java.library.path=."
	$(JAVA) -Djava.library.path=. OrderPipelineDemo 60 4

run-bad: switch-bad
	@echo "[run-bad]"
	$(JAVA) -Djava.library.path=. OrderPipelineDemo 60 4

run-good: switch-good
	@echo "[run-good]"
	$(JAVA) -Djava.library.path=. OrderPipelineDemo 60 4
