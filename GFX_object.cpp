#include <GL/gl.h>
#include "GFX_object.h"

void GFX_object::init( void)
{
  position_.x = 0.0;
  position_.y = 0.0;
  position_.z = 0.0;

  screen_position_.x = 0;
  screen_position_.y = 0;

  rotation_.x = 0.0;
  rotation_.y = 0.0;
  rotation_.z = 0.0;

  width_ = 300.0;
  height_ = 300.0;

  animate_ = false;
  texture_ = 0;
  mask_ = 0;
}

GFX_object::GFX_object()
{
  init();
}

GFX_object::GFX_object( GLuint texture)
{
  init();
  texture_ = texture;
}

GFX_object::GFX_object( GLuint texture, GLuint mask)
{
  init();
  texture_ = texture;
  mask_ = mask;
}

GFX_object::GFX_object( const GFX_object& rhs)
{
  
}

GFX_object::~GFX_object()
{
}

GFX_object& GFX_object::operator=( const GFX_object& rhs)
{
  if (this != &rhs)
  {
    animation_ = rhs.animation_;
    position_ = rhs.position_;
    rotation_ = rhs.rotation_;
    screen_position_ = rhs.screen_position_;
    screen_size_ = rhs.screen_size_;
    width_ = rhs.width_;
    height_ = rhs.height_;
    animate_ = rhs.animate_;
    texture_ = rhs.texture_;
    mask_ = rhs.mask_;
  }

  return *this;
}

void GFX_object::set_position( GLfloat x, GLfloat y, GLfloat z)
{
  position_.x = x;
  position_.y = y;
  position_.z = z;
}

void GFX_object::set_position( T_Position position)
{
  position_ = position;
}

T_Position GFX_object::get_position( void)
{
  return position_;
}

void GFX_object::set_screen_position( GLuint x, GLuint y)
{
  screen_position_.x = x;
  screen_position_.y = y;
}

void GFX_object::set_screen_position( T_ScreenPosition screen_position)
{
  screen_position_ = screen_position;
}

T_ScreenPosition GFX_object::get_screen_position( void)
{
  return screen_position_;
}

GLuint GFX_object::get_screen_x( void)
{
  return screen_position_.x;
}

GLuint GFX_object::get_screen_y( void)
{
  return screen_position_.y;
}

void GFX_object::set_screen_size( GLuint width, GLuint height)
{
  screen_size_.width = width;
  screen_size_.height = height;
}

void GFX_object::set_screen_size( T_ScreenSize screen_size)
{
  screen_size_ = screen_size;
}

T_ScreenSize GFX_object::get_screen_size( void)
{
  return screen_size_;
}

GLuint GFX_object::get_screen_width( void)
{
  return screen_size_.width;
}

GLuint GFX_object::get_screen_height( void)
{
  return screen_size_.height;
}

void GFX_object::set_rotation( GLfloat x, GLfloat y, GLfloat z)
{
  rotation_.x = x;
  rotation_.y = y;
  rotation_.z = z;
}

void GFX_object::set_rotation( T_Rotation rotation)
{
  rotation_ = rotation;
}

T_Rotation GFX_object::get_rotation( void)
{
  return rotation_;
}

void GFX_object::set_size( GLfloat width, GLfloat height)
{
  width_ = width;
  height_ = height;
}

void GFX_object::set_size( T_Size size)
{
  width_ = size.width;
  height_ = size.height;
}

void GFX_object::set_width( GLfloat width)
{
  width_ = width;
}

void GFX_object::set_height( GLfloat height)
{
  height_ = height;
}


GLfloat GFX_object::get_width( void)
{
  return width_;
}

GLfloat GFX_object::get_height( void)
{
  return height_;
}

T_Size GFX_object::get_size( void)
{
  T_Size size;

  size.width = width_;
  size.height = height_;

  return size;
}

void GFX_object::set_animate( bool animate)
{
  animate_ = animate;
}

bool GFX_object::get_animate( void)
{
  return animate_;
}

bool GFX_object::animate( int millisecs)
{
  animation_.animate( *this, millisecs);

  return animate_;
}

bool GFX_object::add_node( GFX_anim_node node)
{
  return animation_.add_node( node);
}

bool GFX_object::clear_animation( void)
{
  return animation_.clear();
}

bool GFX_object::set_animation( GFX_animation animation)
{
  animation_.clear();

  animation_ = animation;

  return true;
}

void GFX_object::set_texture( GLuint texture)
{
  texture_ = texture;
}

GLuint GFX_object::get_texture( void)
{
  return texture_;
}

void GFX_object::set_mask( GLuint mask)
{
  mask_ = mask;
}

GLuint GFX_object::get_mask( void)
{
  return mask_;
}

void GFX_object::draw( void)
{
}

