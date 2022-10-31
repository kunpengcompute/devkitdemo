#!/bin/bash

set -x
set -e

current_dir=$(cd $(dirname $0); pwd)
version="1.0.0"


bash download_package.sh
bash build_runtime.sh ${version}

bash download_src.sh
bash build_dev.sh ${version}
