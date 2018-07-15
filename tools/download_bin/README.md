# J-Link 一键烧写脚本使用指南

此脚本会自动将名为 `bootloader.bin` 的固件烧写到 0x08000000 的位置。再将名为 `application.bin` 的固件烧写到 0x08010000 的位置。

## 流程

- 1、复制编译生成的最新 bin 文件到此文件夹中，修改文件名为：`application.bin` 或 `bootloader.bin`；
- 2、连接好仿真器与待下载电路板；
- 3、点击 `点我烧写.bat` 完成烧写，如果烧写过程中出现卡死或非正常结束的现象，请检查电路板与仿真器之间的连接。

> **注意** ：还差擦除全片功能未添加

## 固件说明

- `application_6lbr.bin`：普通节点的应用程序固件
- `application_slip.bin`：边界路由节点的应用程序固件
- `application_6lbr_micropython.bin`：普通节点并包含 MicroPython 功能的应用程序固件
