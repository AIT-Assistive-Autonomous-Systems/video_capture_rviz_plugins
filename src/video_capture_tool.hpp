#pragma once

#include <string>
#include <rviz_common/tool.hpp>
// #include <rviz_common/properties/string_property.hpp>
// #include <rviz_common/properties/bool_property.hpp>
// #include <rviz_common/properties/float_property.hpp>
#include <rviz_common/properties/int_property.hpp>
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

  // rviz::StringProperty * file_name_property_;
  rviz_common::properties::IntProperty * width_property_;
  rviz_common::properties::IntProperty * height_property_;

  int width_;
  int height_;
  Ogre::TexturePtr tex_;
  cv::VideoWriter writer_;

// protected Q_SLOTS:
//   void updateWidth();
//   void updateHeight();
};
} // namespace video_capture_rviz_plugins
