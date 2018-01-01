# 建立 6LoWPAN 运行环境：边界路由

---

## 1、准备工作

### 1.1 了解基础概念

一个 6LoWPAN 网络需要一个根节点，及 N 个普通节点。这些节点在硬件上都是一样的，仅仅是在固件上会有区别。

[边界路由](https://baike.baidu.com/item/%E8%BE%B9%E7%95%8C%E8%B7%AF%E7%94%B1%E5%99%A8) 做为整个 6LoWPAN 网络的根节点，是整个网络中最重要的节点。在本文档中，由两个硬件构成。一个是运行边界路由固件的 ART-6LoWPAN 开发板，另一个是运行边界路由服务器软件的硬件。

目前演示的边界路由服务器软件运行于虚拟机上（也可以直接运行在 PC、树莓派等其他硬件设备上），与边界路由节点使用 USB 转串口进行有线通信。

边界路由启动完成后，只要其他普通节点在线，即可自动完成 6LoWPAN 网络的组建。

### 1.2 下载、安装 VMware

打开 [百度云盘链接](https://pan.baidu.com/s/1qX8R2nq#list/path=%2FART-6LoWPAN%2Ftools&parentPath=%2F)，下载 `VMware.Workstation.v14.0.0.Win.rar` ，然后解压并安装。

> **注意**：强烈建议安装云盘中的提供的 VMware® Workstation 14 Pro 新版本，低版本可能不兼容我们提供的虚拟机映像

### 1.3 下载、导入虚拟机映像

打开 [百度云盘链接](https://pan.baidu.com/s/1qX8R2nq#list/path=%2FART-6LoWPAN%2Fresource&parentPath=%2F)，下载 `ART_6LoWPAN_VMware_20171205.zip` 。下载完成后，解压出来。

点击 `ART_6LoWPAN.vmx` ，将打开 WMware 软件，并显示如下效果。

![vmware_prestart](https://raw.githubusercontent.com/ART-6LoWPAN/art-6lowpan/master/docs/zh/images/vmware_prestart.png)

此时，虚拟机映像即导入完成。

### 1.4 下载固件

采用 [入门指南](https://github.com/ART-6LoWPAN/art-6lowpan/blob/master/docs/zh/getting_started_guide.md#3固件下载) 提到的 3 种方法。

测试时需要 1 个 **边界路由节点** ，以及至少 1 个 **普通节点** ，对应需要两种固件：

- 普通节点固件
    - 位于： [`tools/download_bin/application_6lbr.bin`](https://github.com/ART-6LoWPAN/art-6lowpan/blob/master/tools/download_bin/application_6lbr.bin)
    - 如需编译下载，修改 [`firmware/app/components/contiki/port/project-conf.h`](https://github.com/ART-6LoWPAN/art-6lowpan/blob/master/firmware/app/components/contiki/port/project-conf.h) 中的 `CONTIKI_SLIP_RAIDO` 宏定义为 **0** ，重新编译即可
- 边界路由节点固件
    - 位于： [`tools/download_bin/application_slip.bin`](https://github.com/ART-6LoWPAN/art-6lowpan/blob/master/tools/download_bin/application_slip.bin)
    - 如需编译下载，修改 [`firmware/app/components/contiki/port/project-conf.h`](https://github.com/ART-6LoWPAN/art-6lowpan/blob/master/firmware/app/components/contiki/port/project-conf.h) 中的 `CONTIKI_SLIP_RAIDO` 宏定义为 **1** ，重新编译即可

## 2、开启边界路由

### 2.1 准备工作

将边界路由节点通过 USB 数据线连接到电脑上，并将其余的普通节点通电。

### 2.2 启动虚拟机

点击 `开启此虚拟机` 按钮启动虚拟机，虚拟机里是 `Ubuntu 16.04 LTS` 32 位系统（默认密码： 11223344），里面运行的是边界路由服务器端软件。

### 2.3 建立连接

系统启动完成后，需要将已经与电脑 USB 连接的边界路由节点连接至 Ubuntu 系统。这样使得运行在虚拟机里的边界路由服务器可以与边界路由节点进行有线通信。

具体操作如下：

- 1、 **识别边界路由节点的 USB 端口** ：在虚拟机软件的右下角有一些灰色的类似 U 盘的图标，这些是电脑上可以被虚拟机连接的 USB 设备。我们的边界路由节点插入到电脑上后，提示信息为 `Cygnal Integrated Silicon Labs CP210x USB to UART Bridge`（将鼠标放置在图标上自动提示）；
![vmware_usb_notice](https://raw.githubusercontent.com/ART-6LoWPAN/art-6lowpan/master/docs/zh/images/vmware_usb_notice.png)

- 2、 **将边界路由节点连接至边界路由服务器** ： **右击** 上一步中边界路由节点对应的 USB 图标，会弹出下图中的选择框，选择 `连接(断开与 主机 的连接)(C)`
![vmware_usb_selection](https://raw.githubusercontent.com/ART-6LoWPAN/art-6lowpan/master/docs/zh/images/vmware_usb_selection.png)

此时会弹出如下对话框，再点击“确定”
![vmware_usb_dialog](https://raw.githubusercontent.com/ART-6LoWPAN/art-6lowpan/master/docs/zh/images/vmware_usb_dialog.png)

此时此 USB 图标会由灰色变为蓝色

## 3、查看运行情况

### 3.1 6lbr 首页

边界路由服务器采用 [6lbr](https://github.com/cetic/6lbr) 这个开源项目，整个 6LoWPAN 的网络运行状态都可以通过 6lbr 的 Web 网页界面进行展示。

点击 Ubuntu 左侧任务栏中的 火狐浏览器 图标，浏览器默认首页就是 6lbr 的首页（也可以打开书签栏中 Info-6LBR 链接），首页效果如下所示：
![6lbr_index](https://raw.githubusercontent.com/ART-6LoWPAN/art-6lowpan/master/docs/zh/images/6lbr_index.png)

> 如果无法打开 6lbr 首页，请查看文档最下方的 “常见问题及解决方法” 章节

### 3.2 节点信息

点击网页上的 Sensors 主标签页，再打开 Sensors 子标签页，如下图所示： 

![6lbr_node_info](https://raw.githubusercontent.com/ART-6LoWPAN/art-6lowpan/master/docs/zh/images/6lbr_node_info.png)

可以看到此时已经有一个节点已经连接到的 6LoWPAN 网络，简单介绍下每列的含义

- **Node** ：节点的 IPv6 地址，点击后可以查看该节点的更加详细的信息
- **Type** ：根据节点 MAC 地址来确定的节点类型，这里显示的是自定义节点
- **Web** ：快速链接到节点上的 HTTP 服务器（目前不存在）
- **CoAP** ：快速链接到节点上的 CoAP 服务器。目前节点默认已开启了 CoAP 服务器，并已在火狐浏览器上安装了 CoAP 插件（此插件目前仅适用于 55.0 含以下版本火狐浏览器， **切记不要对浏览器进行升级** ，忽略一切提示升级的信息），[点击查看节点 CoAP 服务器网页效果](https://raw.githubusercontent.com/ART-6LoWPAN/art-6lowpan/master/docs/zh/images/node_coap_server.png)
- **Parent** ：父节点 IPv6 地址
- **Up PRR** ：UDP 通道的上行报文接收率（Packet Reception Rate）
- **Down PRR** ：UDP 通道的下行报文接收率（Packet Reception Rate）
- **Last seen** ：该节点距离上次接收报文的时间
- **Status** ：状态。 OK：正常，NR：没有向着节点的下行路线，基本可认为离线了。

### 3.3 网络拓扑图

点击网页上的 Sensors 主标签页，再打开 Node tree 子标签页，如下图所示： 

![6lbr_node_tree](https://raw.githubusercontent.com/ART-6LoWPAN/art-6lowpan/master/docs/zh/images/6lbr_node_tree.png)

图中包含边界路由节点，一共两个节点， `3f3e` 节点的父节点为边界路由节点。这个图在节点数量越多时越直观

> 如果无法查看网络拓扑图，请查看文档最下方的 “常见问题及解决方法” 章节

### 3.4 ping6 测试

- 复制将要 ping 的节点 IPv6 地址。可以在节点信息中找到，例如： `fd00::2:232:3105:3f3e`
- 打开左侧任务栏中命令行终端
- 输入 `ping6 fd00::2:232:3105:3f3e(修改为你的节点地址)`
- 如果 ping 成功，可以看到如下效果：

![ping6_test](https://raw.githubusercontent.com/ART-6LoWPAN/art-6lowpan/master/docs/zh/images/ping6_test.png)

## 4、常见问题及解决方法

- **1、无法打开 6lbr 首页**
    - 边界路由节点的 USB 到虚拟机的连接是否正确，有可能电脑上连接了多个 ART-6LoWPAN 开发板或者某些其他开发板，导致出现多个相同的 CP210x USB 图标，此时务必确保连接到虚拟机的是边界路由节点的 USB 
    - 确定连接的节点运行的是否为边界路由固件。普通固件用电脑串口终端软件连接后可以进行命令交互，边界路由节点不可以
- **2、无法打开网络拓扑图**
    - 显示该图需要访问 Google 服务，所以国内环境下，需要用到“特殊的网络转换软件”
    - 首先，点击 Ubuntu 右上角的上下箭头网络图标。打开后，多次切换两个有线网关（有线连接 1 与有线连接 2 ），直到能够访问百度这样正常的网页
    - 开启任务栏绿色图标的 Shadowsocks 软件，测试下所有服务器的延迟，再连接至延迟较少的服务器
    - 在浏览器右上角，有个圆形的 SwitchyOmega 插件，选择 Shadowsocks
    - 此时 Google 服务如果可以正常访问，节点网络拓扑图即可正常显示
