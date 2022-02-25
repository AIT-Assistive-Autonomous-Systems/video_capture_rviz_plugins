#include "video_capture_tool.hpp"
#include <rviz_common/display_context.hpp>
#include <rviz_common/view_manager.hpp>
#include <rviz_common/display_group.hpp>
#include <rviz_common/display.hpp>
#include <rviz_common/render_panel.hpp>
#include <QImage>
#include <QScreen>
#include <QGuiApplication>
#include <opencv2/imgproc.hpp>
#include <Ogre.h>


namespace video_capture_rviz_plugins
{
VideoCaptureTool::VideoCaptureTool()
: Tool()
{
  shortcut_key_ = 'r';
//   file_name_property_ = new rviz::StringProperty(
//     "file_name", "output.mp4",
//     "file_name", this, SLOT(updateFileName()));
  width_property_ = new rviz_common::properties::IntProperty(
    "width", 1920,
    "Width of video in pixels", getPropertyContainer());
  height_property_ = new rviz_common::properties::IntProperty(
    "height", 1080,
    "Height of video in pixels", getPropertyContainer());

  tex_ = Ogre::TextureManager::getSingleton().createManual(
    "CaptureRenderTarget",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    Ogre::TextureType::TEX_TYPE_2D,
    width_property_->getInt(),
    height_property_->getInt(),
    3,
    0,
    Ogre::PixelFormat::PF_R8G8B8,
    Ogre::TextureUsage::TU_RENDERTARGET);
}

VideoCaptureTool::~VideoCaptureTool()
{
}

void VideoCaptureTool::activate()
{
  Ogre::RenderTexture * renderTexture = tex_->getBuffer()->getRenderTarget();
  if (renderTexture->getNumViewports() == 0) {
    auto viewport = renderTexture->addViewport(
      context_->getViewManager()->getCurrent()->getCamera());
    viewport = renderTexture->getViewport(0);
    viewport->setClearEveryFrame(true);

    Ogre::ColourValue clear_color;
    clear_color.r = 48.0 / 255.0;
    clear_color.g = 48.0 / 255.0;
    clear_color.b = 48.0 / 255.0;
    clear_color.a = 1.0;
    viewport->setBackgroundColour(clear_color);
    viewport->setOverlaysEnabled(false);
  }

  writer_.open(
    "output.mp4", cv::VideoWriter::fourcc('h', 'v', 'c', '1'), 30.0,
    cv::Size(width_property_->getInt(), height_property_->getInt()));
}

void VideoCaptureTool::deactivate()
{
  writer_.release();
}

void VideoCaptureTool::update(float, float)
{
  // rviz_common::RenderPanel * panel = context_->getViewManager()->getRenderPanel();
  // QPixmap screenshot =
  //   QGuiApplication::primaryScreen()->grabWindow(panel->winId());
  // QImage src = screenshot.toImage().convertToFormat(QImage::Format_RGB888);

  int w = 1920;
  int h = 1080;

  tex_->getBuffer()->getRenderTarget()->update();
  Ogre::HardwarePixelBufferSharedPtr buffer = tex_->getBuffer(0, 0);
  cv::Mat out_image(h, w, CV_8UC3);

  buffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);
  const Ogre::PixelBox & pb = buffer->getCurrentLock();
  // Obtain a pointer to the texture's buffer
  uint8_t * data = static_cast<uint8_t *>(pb.data);
  // Copy the data
  // memcpy(
  //   static_cast<uint8_t *>(pb.data),
  //   &image_ptr_->data[0],
  //   sizeof(uint8_t) * image_ptr_->data.size()
  // );
  cv::Mat rendered_image(h, w, CV_8UC3, data);
  // if (image.size().width != width_property_->getInt() ||
  //   image.size().height != height_property_->getInt())
  // {
  //   cv::resize(
  //     image, image, cv::Size(
  //       width_property_->getInt(),
  //       height_property_->getInt()), 0, 0, cv::INTER_LINEAR);
  // }
  cv::cvtColor(rendered_image, out_image, cv::COLOR_RGB2BGR);
  buffer->unlock();
  writer_ << out_image;
}
} // namespace video_capture_rviz_plugins

#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(video_capture_rviz_plugins::VideoCaptureTool, rviz_common::Tool)
