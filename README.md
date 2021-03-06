# mynt-eye-sdk 例程学习

按照官方给出的sample进行各个api的测试

CMakeList.txt来自官方给出的案例，利用vs-code的cmake工具进行编译。

## 1. mynteye_demo

[mynt-eye_demo](mynteye_demo.cc)是官方给出的案例，主要涉及了以下几个API

1. `auto &&api = API::Create(0,nullptr);`  

2. `auto &&request = api->SelectStreamRequest(&ok);` 

3. `api->ConfigStreamRequest(request);`

4. `api->Start(Source::VIDEO_STREAMING);`

5. `api->WaitForStreams();`

6. `auto &&left_data = api->GetStreamData(Stream::LEFT);`

7. `api->Start(Source::ALL);`

8. `api->Stop(Source::ALL);`

## 2. get_device_info

[获取设备信息](get_device_info.cc)

```
I/utils.cc:35 Detecting MYNT EYE devices
I/utils.cc:48 MYNT EYE devices:
I/utils.cc:51   index: 0, name: MYNT-EYE-S2110, sn: XXXXXXXXXXXXXXXX, firmware: 1.4
I/utils.cc:60 Only one MYNT EYE device, select index: 0
I/synthetic.cc:52 camera calib model: kannala_brandt
2.5.0I/get_device_info.cc:23 Device name: MYNT-EYE-S2110
I/get_device_info.cc:24 Serial number: XXXXXXXXXXXXXXXX
I/get_device_info.cc:25 Firmware version: 1.4
I/get_device_info.cc:26 Hardware version: 1.0
I/get_device_info.cc:27 Spec version: 1.2
I/get_device_info.cc:28 Lens type: 0001
I/get_device_info.cc:29 IMU type: 0001
I/get_device_info.cc:30 Nominal baseline: 80
I/get_device_info.cc:31 Auxiliary chip version: 1.4
I/get_device_info.cc:33 Isp version: 1
```

## 3. get_img_params

[获取图像标定参数](get_img_params.cc)

1. `auto in_right = api->GetIntrinsicsBase(Stream::RIGHT);`

2. `if(in_left->calib_model() == CalibrationModel::PINHOLE)`

3. `in_left = std::dynamic_pointer_cast<IntrinsicsPinhole>(in_left)`

4. `in_left -> ResizeIntrinsics();`

5. `auto info = api->GetCameraROSMsgInfoPair();`


## 4. get_imu_params

1. `api->EnableMotionDatas();`

2. `api->EnableImuTimestampCorrespondence(true);`

3. `painter.DrawImgData(img, *left_data.img); // CVPainter`

## 5. get_stereo_rectified

[get_stereo_rectified](get_stereo_rectified.cc)类似 [mynt-eye_demo](mynteye_demo.cc)  

获取修正后的图像

1. `api->EnableStreamData(Stream::LEFT_RECTIFIED);`

2. `auto &&left_data = api->GetStreamData(Stream::LEFT_RECTIFIED);`

3. `api->WaitForStreams();`
  
双目纠正图像，属于上层合成数据。此类数据需要事先`EnableStreamData()`启用，然后`GetStreamData()`才能获取到。`WaitForStream`等待关键原始数据。刚开始时合成数据可能还在处理，取出的是空值，所以需要判断下不为空。

## 6. camera_with_junior_device_api

[获取原始图像](camera_with_junior_device_api.cc)

获取未修正过的图像以及IMU的数据

## 7. get_disparity

[获取视差图](get_disparity.cc)

获取视差图像，同样需要事先`EnableStreamData()`启用，然后`GetStreamData()`才能获取到，另外判断不为空后再使用。

可以通过插件提速，提高效果。

1. `auto &&disp_norm_data = api->GetStreamData(Stream::DISPARITY_NORMALIZED);`

## 8. get_depth_and_points

[获取视差，深度和点云图像](get_depth_and_points.cc)

点云图像，属于上层合成数据。需要事先`EnableStreamData()`启用，然后`GetStreamData()`获取。另外，判断不为空后再使用。

### 8.1 C++知识点

[知识点总结]()

* 右值引用（&&）
* lambda表达式
* 回调函数（callback）
* 无名命名空间
* C++类设计的五个基本原则
* explicit禁用隐式类型转换
* 类成员的冒号初始化
* std::function
* std::move
* reinterpret_cast/static_cast

### 8.2 main函数流程

1. 设置相机参数，进入while循环
2. 获取左右相机的原始数据并拼接显示：`api->GetStreamData(Stream::LEFT);`
3. 获取视差图和深度（depth图）：
    * `api->GetStreamData(Stream::DISPARITY_NORMALIZED);`
    * `api->GetStreamData(Stream::DEPTH);`
4. 设置鼠标点击的回调函数
    * `cv::setMouseCallback("depth", OnDepthMouseCallback, &depth_region);`
5. 调用`DepthRegion`类的`DrawRect`和`ShowElems`
6. 获取点云信息`api->GetStreamData(Stream::POINTS)`并且每当有心的闲云信息时对`pcviewer`进行更新  
7. 检测while终止，`api->Stop(Source::VIDEO_STREAMING)`关闭视频流

### 8.3 `class DepthRegion`

### 8.4 `OnDepthMouseCallback(...)`

## 9. get_imu_correspondence

[获取imu即进行帧同步](get_imu_correspondence.cc)

imu和stream之间的时间戳同步。

1. `api->EnableTimestampCorrespondence(Stream::LEFT); `

2. 读取两眼图像信息并判断是否有不为空的帧，如果有则读取某一只眼的（左眼）时间戳`auto img_stamp = left_data.img->timestamp;`

3. 计算两帧间的时间差`img_stamp - prev_img_stamp`

4. 读取imu信息`auto &&motion_datas = api->GetMotionDatas();`如果此vector大小为0则跳过这一次循环。

5. 遍历`motion_datas`队列，为每个成员判断数据类型，读取时间戳并计算它们与上一个`imu_stamp`之间的差值  以及它们与图像时间戳之间的差值（并+1），最后更新时间戳

第2步到第5步以图像帧的更新为准。即获取两帧之间`imu`可能累积许多组数据，细节取决于`GetMotionDatas()`的实现。

## 10. get_imu

[读取imu数据](get_imu.cc)

相对`get_imu_correspondence`而言，没有计算图像帧与数据帧之间的时间戳差值  
在通过`auto &&motion_datas = api->GetMotionDatas();`获取imu数据之后将数据备份在`motion_datas_s`内。备份用`static`修饰，静态局部变量只初始化一次。

[c++中的static](https://www.cnblogs.com/33debug/p/7223869.html)

> 1.该变量在全局数据区分配内存；  
2.静态局部变量在程序执行到该对象的声明处时被首次初始化，即以后的函数调用不再进行初始化；  
3.静态局部变量一般在声明处初始化，如果没有显式初始化，会被程序自动初始化为0；  
4.它始终驻留在全局数据区，直到程序运行结束。但其作用域为局部作用域，当定义它的函数或语句块结束时，其作用域随之结束。  

# 11. get_data_withoutselect

[获取默认视频流](get_data_withoutselect.cc)
调用 `auto request = api->GetStreamRequest();`获取默认的视频流。

# 12. get_from_callbacks

[从回调函数获取图像](get_from_callbacks.cc)

一共有3个数据流通过回调函数获取，左眼图像、深度图和imu数据。每个都符合如下流程：

1. 设置计数器：`std::atomic_uint left_count(0);`

2. 设置回调函数：`api->SetStreamCallback(...)`，传入所需的类以及一个lambda表达式。

设定全部完成后通过`api->Start(Source::ALL);`开启获取。3种数据中，深度图和imu各持有一个锁。

在`while`循环中通过`CVPainter`来进行各个数据的绘制。

