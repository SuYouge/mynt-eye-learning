// Copyright 2018 Slightech Co., Ltd. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>

#include "mynteye/api/api.h"

MYNTEYE_USE_NAMESPACE //宏定义启用命名空间

int main(int argc, char const *argv[]) {
  auto &&api = API::Create(0, nullptr); // class API::Create
  if (!api) return 1;

  bool ok;
  auto &&request = api->SelectStreamRequest(&ok); // 选择视频流
  if (!ok) return 1;
  api->ConfigStreamRequest(request); // 将选择传递给api
  api->Start(Source::VIDEO_STREAMING); // 启动视频

  double fps;
  double t = 0.01;
  std::cout << "fps:" << std::endl;

  cv::namedWindow("frame");

  while (true) {
    api->WaitForStreams(); // 等待视频流就绪

    auto &&left_data = api->GetStreamData(Stream::LEFT); // 加载左眼数据
    auto &&right_data = api->GetStreamData(Stream::RIGHT); // 加载右眼数据

    cv::Mat img;
    if (!left_data.frame.empty() && !right_data.frame.empty()) { //如果双眼数据都不为空
      double t_c = cv::getTickCount() / cv::getTickFrequency();
      fps = 1.0/(t_c - t); // 计算帧率
      printf("\b\b\b\b\b\b\b\b\b%.2f", fps);
      t = t_c;
      cv::hconcat(left_data.frame, right_data.frame, img); //图像拼接函数
      cv::imshow("frame", img); // 显示图像
    }

    char key = static_cast<char>(cv::waitKey(1)); //退出程序
    if (key == 27 || key == 'q' || key == 'Q') {  // ESC/Q
      break;
    }
  }

  api->Stop(Source::VIDEO_STREAMING);
  return 0;
}
