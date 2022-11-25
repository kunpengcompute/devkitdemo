# SP680 Network Acceleration-OVS Offload Demo

English | [简体中文](README.md)

## Introduction
The network acceleration fuction of the SP680 SmartNIC is used to cooperate with the server to offload the network. This demo checks the basic configurations of the server environment, such as the firewall status, hugepage configuration, SP680 driver dependency installation, group configuration, and ICMP protocol offload (whether two VMs on the same host or different hosts can ping each other and methods to view the flow table).

## Prerequisites

1. The running environment must be a physical machine and the OS must be openEuler 20.03 SP1. 

2. The firewall has been disabled.

3. Hugepages have been configured.

4. The SP680 driver dependency has been installed.

5. The network OVS acceleration software package has been deployed and the OVS service has been enabled.

6. The network has been configured.

7. The root user is required to connect to the server.

8. A VM has been created and the VF device has been passed througe to the VM. Two VMs that can communicate with each other (can ping each other) have been created and password-free communication between physical machines and VMs has beeb configure.

```
-------------          -------------
|192.168.1.2|          |192.168.1.3|
|           |----------|           |
-------------          -------------
     |                       |
 ---------              ----------
```

9. The IP address of the management port that supports SSH bandwidth has been configured for the VM by running the 'virsh attach-interface vm1 --type bridge --source virbr0 --model virtio --current' command.

## Usage Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Go to the project root directory.


   ```shell
   cd ./devkitdemo/Development_framework/dpak-sdk/examples/OVS_dpak/OVS_offload
   ```

3. Run the demo.
   ```shell
   bash OVS_offload.sh
   ```