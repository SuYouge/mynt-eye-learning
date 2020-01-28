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
