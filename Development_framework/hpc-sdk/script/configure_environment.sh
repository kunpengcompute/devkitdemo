#!/bin/bash

# Copyright 2022 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

compiler_gcc=''
compiler_gcc_install=''
compiler_bisheng=''
compiler_bisheng_install=''
hyper_mpi_gcc=''
hyper_mpi_gcc_install=''
hyper_mpi_bisheng=''
hyper_mpi_bisheng_install=''


configure_env(){
    # configure env
    if [[ "${compiler_gcc}${compiler_bisheng}${hyper_mpi_gcc}${hyper_mpi_bisheng}test" == 'test' ]];then
        echo -e "\e[1;33mYou have not installed any software. Please run the installation script before running the script.\e[0m"
        exit
    fi
    [[ ${hyper_mpi_bisheng} ]] && set_hyper_mpi_env ${hyper_mpi_bisheng_install}
    [[ ${hyper_mpi_gcc} ]] && set_hyper_mpi_env ${hyper_mpi_gcc_install}
    [[ ${compiler_gcc} ]] && set_compiler_gcc_env ${compiler_gcc_install}
    [[ ${compiler_bisheng} ]] && set_compiler_bisheng_env ${compiler_bisheng_install}
    echo -e "\e[1;33mTo make the related commands take effect, run the following command:\e[0m"
    if [[ ${hyper_mpi_bisheng} || ${hyper_mpi_gcc} ]];then
        echo "source ~/.bashrc"
    fi

    if [[ ${compiler_gcc} || ${compiler_bisheng} ]];then
        echo "source /etc/profile"
    fi
}

set_compiler_gcc_env(){
    local install_compiler_path=$1
    echo "export PATH=${install_compiler_path}/${compiler_gcc}/bin:\$PATH" >>/etc/profile
    echo "export INCLUDE=${install_compiler_path}/${compiler_gcc}/INCLUDE:\$INCLUDE" >>/etc/profile
    echo "export LD_LIBRARY_PATH=${install_compiler_path}/${compiler_gcc}/lib64:\$LD_LIBRARY_PATH" >>/etc/profile
}

set_compiler_bisheng_env(){
    local install_compiler_path=$1
    echo "export PATH=${install_compiler_path}/${compiler_bisheng}/bin:\$PATH" >>/etc/profile
    echo "export LD_LIBRARY_PATH=${install_compiler_path}/${compiler_bisheng}/lib:\$LD_LIBRARY_PATH" >>/etc/profile

}

set_hyper_mpi_env(){
    local install_path=$1
    echo "hwmpi=${install_path}" >> ~/.bashrc
    echo 'export OPAL_PREFIX=${hwmpi}/ompi' >> ~/.bashrc
    echo 'export PATH=${hwmpi}/ompi/bin:${hwmpi}/ucx/bin:$PATH' >> ~/.bashrc
    echo 'export INCLUDE=${hwmpi}/ompi/include:${hwmpi}/ucx/include:$INCLUDE' >> ~/.bashrc
    echo 'export LD_LIBRARY_PATH=${hwmpi}/ompi/lib:${hwmpi}/ucx/lib:$LD_LIBRARY_PATH' >> ~/.bashrc
}

configure_env