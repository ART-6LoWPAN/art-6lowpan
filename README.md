# ART-6LoWPAN：基于 RT-Thread 的 6LoWPAN  开源项目

---

## 1、ART-6LoWPAN 是什么

[ART-6LoWPAN](https://github.com/ART-6LoWPAN) 是一个基于 [RT-Thread](https://github.com/RT-Thread/rt-thread) 的 6LoWPAN 开源项目，有配套的类 Arduino 开发板，软件和硬件都将开源出来。

**6LoWPAN** (IPv6 over Low-Power Wireless Personal Area Networks) 作为物联网的一种协议方式，已经发展了很多年。使用这项技术后，所有节在点网络层通过 IPv6 联系起来，组网方式更加标准化，极大的方便了上层应用的开发。

但是，网上的 6LoWPAN 教程很少，与 RT-Thread 这类 RTOS 结合的开源项目更少，这也进一步提高了 6LoWPAN 入门门槛。所以，想通过这个开源项目，能让大家，尤其是刚接触的新人，无需关注底层，快速上手 6LoWPAN 。还可以利用本项目提供的类 Arduino 的开发板，加速产品原型设计的验证过程。

> PS：近日，中共中央办公厅 国务院办公厅 印发了 [《推进互联网协议第六版（IPv6）规模部署行动计划》](http://www.gov.cn/zhengce/2017-11/26/content_5242389.htm) 。这说明了国家当前对于 IPv6 的重视，对我们团队也是一种极大的鼓舞，希望 IPv6 在物联网领域的应用会越来越广。

### 1.1 功能特点

- 采用 6LoWPAN 协议，节点支持 mesh 组网，并支持 IPv6
- Sub-1G 频段，传输距离远，穿墙性能更好
- 兼容 Arduino 接口，灵活接入更多传感器，加速产品原型设计的验证
- 基于 RT-Thread 3.0 物联网操作系统，丰富的组件及设备驱动，开发更容易
- 支持动态运行 Python 脚本，极强的可玩性
- 软件代码（Eclipse/IAR/Keil）、硬件设计资料全部开源，量产级要求，高质量开源

###  1.2 应用场景

- 远距离、多节点数据采集
- 无线信号穿墙性能要求较高的设备间通信
- 支持 Sub-1G 频段的多功能网关

## 2、ART-6LoWPAN 如何使用

### 2.1 硬件准备

项目开源硬件的引脚关系图如下：
[![board_pin](https://raw.githubusercontent.com/ART-6LoWPAN/art-6lowpan/master/docs/zh/images/board_pin.jpg)](https://github.com/ART-6LoWPAN/art-6lowpan)

#### 2.1.1 硬件配置

- MCU :
    - STM32F405RGT6
    - ARM Cortex-M4 @168MHz
    - 1 MB Flash
    - 192+4 KB SRAM
- RADIO :
    - SI4463 B1 
    - Band : 425-525MHz, Typ 433MHz
    - SMA/IPX
- Nor Flash : 128Mb
- USB-TTL : CP2102
- Communication Interfaces :
    - SPI/I2C/CAN
    - 2 x USART
- 2 x ADC channel
- 2 x PWM
- Debug : Serial wire debug (SWD)

> PS：如果没有使用配套的开发板，也可以手工搭建硬件测试环境，SI4463 无线模块网上很多，项目源码没有对硬件平台做任何限制。温馨提示：手工搭建环境对动手能力要求较高，风险较大，不建议新手尝试。

### 2.2 开发环境

本项目支持 Eclipse/IAR/Keil 3 款 IDE ，推荐使用 Eclipse + GCC 的方式，开源的工具也一样非常好用（下载地址：[百度云盘](https://pan.baidu.com/s/1qX8R2nq#list/path=%2FART-6LoWPAN%2Ftools%2FGCC%20MCU%20Eclpise%20IDE) ）。下载后解压到 C 盘根目录下即可。更多 Eclipse + GCC 使用介绍请查看：《熟悉 Eclipse + GCC 开发环境》 文档。

接下来就可以按照下面的文档/视频，进入 6LoWPAN 的世界了

- 入门文档
    - [上手指南](https://github.com/ART-6LoWPAN/art-6lowpan/tree/master/docs/zh/getting_started_guide.md)
- 搭建边界路由运行环境（文档|视频）
- CoAP 应用开发文档

> 关于射频驱动为什么是 `*.a` 的库文件：我们希望这个开源项目可以得到更多的开发者支持。所以 `* .a` 会在 GitHub Star 超过 **500** 的时候开源。如果觉得这个项目不错，请多多支持我们吧。

### 2.3 测试环境

请查看下面的说明文档/视频

- 如何进行无线性能测试

## 3、ART-6LoWPAN 的进展

### 3.1 软件

- [X] 基础功能
    - [X] RT-Thread 3.0 移植 by [@armink](https://github.com/armink)
    - [X] contiki 3.0 移植 by [@armink](https://github.com/armink)
    - [X] SI4463 射频驱动 by [@armink](https://github.com/armink) [@xidongxu](https://github.com/xidongxu)
    - [X] IAP 升级 by [@armink](https://github.com/armink)
    - [X] 6LoWPAN
        - [X] 普通节点 by [@armink](https://github.com/armink) [@xidongxu](https://github.com/xidongxu)
        - [X] 边界路由节点 by [@armink](https://github.com/armink)
        - [X] 边界路由服务器端 by [@armink](https://github.com/armink)
    - [X] CoAP
        - [X] 服务器 by [@xidongxu](https://github.com/xidongxu)
        - [X] 客户端 by [@xidongxu](https://github.com/xidongxu)
- [ ] IDE 工程配置
    - [X] Eclipse + GCC by [@armink](https://github.com/armink)
    - [X] Eclipse + IAR by [@armink](https://github.com/armink)
    - [X] IAR by [@armink](https://github.com/armink)
    - [ ] Keil by [@armink](https://github.com/armink)
- [ ] MicroPython
    - [ ] Python 脚本解析
    - [ ] RT-Thread Python IIC 驱动
    - [ ] DHT12 Python 驱动
- [ ] 移植 contiki-NG
- [ ] FOTA

### 3.2 硬件

- [X] 引脚关系图 by [@xuminglu1987](https://github.com/xuminglu1987)
- [ ] 硬件连接关系表 by [@xuminglu1987](https://github.com/xuminglu1987)
- [ ] 原理图 by [@xuminglu1987](https://github.com/xuminglu1987)
- [ ] PCB by [@xuminglu1987](https://github.com/xuminglu1987)

### 3.3 文档

- [X] 首页说明文档 by [@armink](https://github.com/armink)
- [ ] 入门文档
    - [X] 上手指南
    - [ ] 熟悉 Eclipse + GCC 开发环境
    - [ ] 常用 MSH shell 命令
- [ ] 搭建边界路由运行环境
- [ ] 如何进行无线性能测试
- [ ] CoAP 应用开发文档

### 3.4 视频

- [ ] 搭建边界路由运行环境

## 4、许可

MIT Copyright (c) [ART-6LoWPAN Development Team](https://github.com/ART-6LoWPAN)
