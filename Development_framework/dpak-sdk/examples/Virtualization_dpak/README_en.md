# SP680 Virtualization Offload Demo

English | [简体中文](README.md)

## Introduction
Here we describe key features of the SP680 SmartNIC managed by OpenStack Train, including VirtlO-net passthrough device management, hot swap of VirtlO-net passthrough devices, and live migration of passthrough device.

## Prerequisites

1. Four physical machines, including a controller node, a storage node, and two compute nodes have been prepared. The OS must be openEuler 20.03 SP1. 

2. The firewalls of the four servers has been disabled.

3. On the compute nodes, hugepages have been configured. SP680 OVS and live migration drivers have been installed, and Neutron and the OVS bridge have been configured properly.

4. The OpenStack version of the controller node must be between 3.19 and 4.02. The Ceph node, Nova service, and Neutron service are normal.

5. The root user is required to connect to the server.

6. Password-free connections between the controller node and the storage node and compute nodes are configured.

7. If local compilation is involved in the process of configuring the target server, configure password-free login for the local host and the controller node.

## Usage Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Go to the project root directory.


   ```shell
   cd ./devkitdemo/Development_framework/dpak-sdk/examples/Virtualization_dpak/script
   ```

3. Run the demo.
   ```shell
   bash run_demo.sh
   ```