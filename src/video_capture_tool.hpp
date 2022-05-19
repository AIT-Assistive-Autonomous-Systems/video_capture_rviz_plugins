#pragma once

#include <string>
#include <rviz_common/tool.hpp>
#include <rviz_common/properties/int_property.hpp>
#include <rviz_common/properties/color_property.hpp>
#include <rviz_common/properties/string_property.hpp>
#include <opencv2/videoio.hpp>
#include <Ogre.h>

namespace video_capture_rviz_plugins
{
class VideoCaptureTool : public rviz_common::Tool
{
  Q_OBJECT

public:
  VideoCaptureTool();
  virtual ~VideoCaptureTool();

  void activate() override;
  void deactivate() override;

protected:
  void update(float wall_dt, float ros_dt) override;

  rviz_common::properties::IntProperty * width_property_;
  rviz_common::properties::ColorProperty * clear_color_property_;
  rviz_common::properties::StringProperty * fourcc_property_;
  size_t video_height_;
  Ogre::TexturePtr tex_;
  cv::VideoWriter writer_;
};
} // namespace video_capture_rviz_plugins
