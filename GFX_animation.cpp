#include "GFX_animation.h"
#include "GFX_object.h"

void GFX_anim_node::init( void)
{
  origin_.position.x = destination_.position.x = 0.0;
  origin_.position.y = destination_.position.y = 0.0;
  origin_.position.z = destination_.position.z = 0.0;

  origin_.rotation.x = destination_.rotation.x = 0.0;
  origin_.rotation.y = destination_.rotation.y = 0.0;
  origin_.rotation.z = destination_.rotation.z = 0.0;

  origin_.size.width = destination_.size.width = 0;
  origin_.size.height = destination_.size.height = 0;

  origin_.screen_size.width = destination_.screen_size.width = 0;
  origin_.screen_size.height = destination_.screen_size.height = 0;

  origin_.screen_position.x = destination_.screen_position.x = 0;
  origin_.screen_position.y = destination_.screen_position.y = 0;

  texture_ = 0;
  pause_ = false;
  stop_ = false;
  keep_ = false;
  millisecs_ = 0;
  i_millisec_ = 0;
  relative_position_ = false;
  relative_rotation_ = false;
  animate_position_ = false;
  animate_rotation_ = false;
  animate_size_ = false;
  animate_screen_size_ = false;
  animate_screen_position_ = false;
}

GFX_anim_node::GFX_anim_node()
{
  init();
}

GFX_anim_node::~GFX_anim_node()
{
}

void GFX_anim_node::set_origin_position( GLfloat x, GLfloat y, GLfloat z)
{
  origin_.position.x = x;
  origin_.position.y = y;
  origin_.position.z = z;

  animate_position_ = true;
}


void GFX_anim_node::set_origin_position( T_Position position)
{
  origin_.position = position;

  animate_position_ = true;
}

T_Position GFX_anim_node::get_origin_position( void)
{
  return origin_.position;
}

void GFX_anim_node::set_destination_position( GLfloat x, GLfloat y, GLfloat z)
{
  destination_.position.x = x;
  destination_.position.y = y;
  destination_.position.z = z;

  animate_position_ = true;
}

void GFX_anim_node::set_destination_position( T_Position position)
{
  destination_.position = position;

  animate_position_ = true;
}

T_Position GFX_anim_node::get_destination_position( void)
{
  return destination_.position;
}

void GFX_anim_node::set_origin_rotation( T_Rotation rotation)
{
  origin_.rotation = rotation;

  animate_rotation_ = true;
}

void GFX_anim_node::set_origin_rotation( GLfloat x, GLfloat y, GLfloat z)
{
  origin_.rotation.x = x;
  origin_.rotation.y = y;
  origin_.rotation.z = z;

  animate_rotation_ = true;
}

T_Rotation GFX_anim_node::get_origin_rotation( void)
{
  return origin_.rotation;
}

void GFX_anim_node::set_destination_rotation( T_Rotation rotation)
{
  destination_.rotation = rotation;

  animate_rotation_ = true;
}


void GFX_anim_node::set_destination_rotation( GLfloat x, GLfloat y, GLfloat z)
{
  destination_.rotation.x = x;
  destination_.rotation.y = y;
  destination_.rotation.z = z;

  animate_rotation_ = true;
}

T_Rotation GFX_anim_node::get_destination_rotation( void)
{
  return destination_.rotation;
}

void GFX_anim_node::set_origin_size( GLuint width, GLuint height)
{
  origin_.size.width = width;
  origin_.size.height = height;

  animate_size_ = true;
}

void GFX_anim_node::set_origin_size( T_Size size)
{
  origin_.size = size;

  animate_size_ = true;
}

T_Size GFX_anim_node::get_origin_size( void)
{
  return origin_.size;
}

void GFX_anim_node::set_destination_size( GLuint width, GLuint height)
{
  destination_.size.width = width;
  destination_.size.height = height;

  animate_size_ = true;
}

void GFX_anim_node::set_destination_size( T_Size size)
{
  destination_.size = size;

  animate_size_ = true;
}

T_Size GFX_anim_node::get_destination_size( void)
{
  return destination_.size;
}

void GFX_anim_node::set_origin_screen_position( GLuint x, GLuint y)
{
  origin_.screen_position.x = x;
  origin_.screen_position.y = y;

  animate_screen_position_ = true;
}


void GFX_anim_node::set_origin_screen_position( T_ScreenPosition screen_position)
{
  origin_.screen_position = screen_position;

  animate_screen_position_ = true;
}

T_ScreenPosition GFX_anim_node::get_origin_screen_position( void)
{
  return origin_.screen_position;
}

void GFX_anim_node::set_destination_screen_position( GLuint x, GLuint y)
{
  destination_.screen_position.x = x;
  destination_.screen_position.y = y;

  animate_screen_position_ = true;
}

void GFX_anim_node::set_destination_screen_position( T_ScreenPosition screen_position)
{
  destination_.screen_position = screen_position;

  animate_screen_position_ = true;
}

T_ScreenPosition GFX_anim_node::get_destination_screen_position( void)
{
  return destination_.screen_position;
}

void GFX_anim_node::set_origin_screen_size( GLuint width, GLuint height)
{
  origin_.screen_size.width = width;
  origin_.screen_size.height = height;

  animate_screen_size_ = true;
}

void GFX_anim_node::set_origin_screen_size( T_ScreenSize screen_size)
{
  origin_.screen_size = screen_size;

  animate_screen_size_ = true;
}

T_ScreenSize GFX_anim_node::get_origin_screen_size( void)
{
  return origin_.screen_size;
}

void GFX_anim_node::set_destination_screen_size( GLuint width, GLuint height)
{
  destination_.screen_size.width = width;
  destination_.screen_size.height = height;

  animate_screen_size_ = true;
}

void GFX_anim_node::set_destination_screen_size( T_ScreenSize screen_size)
{
  destination_.screen_size = screen_size;

  animate_screen_size_ = true;
}

T_ScreenSize GFX_anim_node::get_destination_screen_size( void)
{
  return destination_.screen_size;
}

void GFX_anim_node::set_relative_position( bool value)
{
  relative_position_ = value;

  animate_position_ = true;
}

bool GFX_anim_node::get_relative_position( void)
{
  return relative_position_;
}

void GFX_anim_node::set_relative_rotation( bool value)
{
  relative_rotation_ = value;

  animate_rotation_ = true;
}

bool GFX_anim_node::get_relative_rotation( void)
{
  return relative_rotation_;
}

void GFX_anim_node::set_time( int millisecs)
{
  millisecs_ = millisecs;
  i_millisec_ = 0;
}

int GFX_anim_node::get_time( void)
{
  return millisecs_;
}

void GFX_anim_node::set_texture( GLuint texture)
{
  texture_ = texture;
}

GLuint GFX_anim_node::get_texture( void)
{
  return texture_;
}

void GFX_anim_node::set_pause( void)
{
  pause_ = true;
}

bool GFX_anim_node::get_pause( void)
{
  return pause_;
}

void GFX_anim_node::set_stop( void)
{
  stop_ = true;
}

bool GFX_anim_node::get_stop( void)
{
  return stop_;
}

void GFX_anim_node::set_keep( void)
{
  keep_ = true;
}

bool GFX_anim_node::get_keep( void)
{
  return keep_;
}

void GFX_anim_node::advance_time( int millisecs)
{
  i_millisec_ += millisecs; // BM: ???
}

float GFX_anim_node::get_percentage( void)
{
  return (float)i_millisec_ /(float)millisecs_;
}

void GFX_anim_node::clear( void)
{
  init();
}

bool GFX_anim_node::get_animate_position( void)
{
  return animate_position_;
}

bool GFX_anim_node::get_animate_rotation( void)
{
  return animate_rotation_;
}

bool GFX_anim_node::get_animate_size( void)
{
  return animate_size_;
}

bool GFX_anim_node::get_animate_screen_position( void)
{
  return animate_screen_position_;
}

bool GFX_anim_node::get_animate_screen_size( void)
{
  return animate_screen_size_;
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=- */
/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=- */

void GFX_animation::init( void)
{
  current_node_initialized_ = false;
}

GFX_animation::GFX_animation()
{
  init();
}

GFX_animation::~GFX_animation()
{
}

bool GFX_animation::clear(void)
{
  animation_.clear();
  current_node_initialized_ = false;

  return true;
}

bool GFX_animation::add_node( GFX_anim_node node)
{
  animation_.push_back( node);

  return true;
}

// BM: Initialise the first node in the animation list.
bool GFX_animation::init_current_node( void)
{
  bool result = false;

  if (animation_.size() > 0)
  {
    if (current_node_initialized_ == false)
    {
      current_node_ = animation_.front();
      anim_iter_ = animation_.begin();
      current_node_initialized_ = true;

      result = true;
    }
  }

  return result;
}

// BM: Gets the next animation node.
// BM: - If the current node needs to be kept then advance the iterator.
// BM:   otherwise erase it (this will also advance the iterator)
bool GFX_animation::get_next_node( void)
{
  bool result = false;

  if (animation_.size() > 0 &&
      anim_iter_ != animation_.end())
  {
    if (current_node_.get_keep() == false)
    {
      anim_iter_ = animation_.erase( anim_iter_ );
    }
    else
    {
      anim_iter_++;
    }

    // BM: Check that the iterator is still valid.
    if (anim_iter_ != animation_.end())
    {
      current_node_ = *anim_iter_;
      result = true;
    }
    else
    {
      current_node_initialized_ = false;
    }
  }

  return result;
}

// BM: Check to see if this animation is made of relative positional
// BM: or rotational moves.
// BM: - Set the origin and destination position/rotation based upon
// BM    the relative destination data.
bool GFX_animation::set_relative_data( GFX_object& object)
{
  bool result = false;

  if (current_node_.get_relative_position())
  {
    T_Position origin_pos, destination_pos;
    
    origin_pos = object.get_position();
    destination_pos = current_node_.get_destination_position();
    
    destination_pos.x += origin_pos.x;
    destination_pos.y += origin_pos.y;
    destination_pos.z += origin_pos.z;
    
    current_node_.set_origin_position( object.get_position());
    current_node_.set_destination_position( destination_pos);

    result = true;
  }
  
  // Relative animation.  Destination rotation is relative to current rotation..
  if (current_node_.get_relative_rotation())
  {
    T_Rotation origin_rot, destination_rot;

    origin_rot = object.get_rotation();
    destination_rot = current_node_.get_destination_rotation();
    
    destination_rot.x += origin_rot.x;
    destination_rot.y += origin_rot.y;
    destination_rot.z += origin_rot.z;
  
    current_node_.set_origin_rotation( object.get_rotation());
    current_node_.set_destination_rotation( destination_rot);

    result = true;
  }

  return result;
}
  
bool GFX_animation::animate( GFX_object &object, int millisecs)
{
  // BM: Check pause first.
  float diff;
  GFX_anim_node node;
  T_OrgDest origin;
  T_OrgDest destination;

  if (object.get_animate())
  {
    if (current_node_initialized_ == false)
    {
      if (init_current_node())
      {
        set_relative_data( object);
      }
      else
      {
        object.set_animate( false);
      }
    }
  }

  if (object.get_animate())
  {
    current_node_.advance_time( millisecs);

    destination.position = current_node_.get_destination_position();
    destination.rotation = current_node_.get_destination_rotation();
    destination.size = current_node_.get_destination_size();
    destination.screen_position = current_node_.get_destination_screen_position();
    destination.screen_size = current_node_.get_destination_screen_size();
    origin.position = current_node_.get_origin_position();
    origin.rotation = current_node_.get_origin_rotation();
    origin.size = current_node_.get_origin_size();
    origin.screen_position = current_node_.get_origin_screen_position();
    origin.screen_size = current_node_.get_origin_screen_size();

    if (current_node_.get_texture() != 0)
    {
      object.set_texture( current_node_.get_texture());
    }

    if (current_node_.get_stop() == true)
    {
      object.set_animate( false);
      get_next_node();
      set_relative_data( object);
    }
    else if (current_node_.get_percentage() >= 1.0)
    {
      // Do anything here for the end of an anim node.
      if (current_node_.get_animate_position())
      {
        object.set_position( destination.position);
      }
      if (current_node_.get_animate_rotation())
      {
        object.set_rotation( destination.rotation);
      }
      if (current_node_.get_animate_size())
      {
        object.set_size( destination.size);
      }
      if (current_node_.get_animate_screen_position())
      {
        object.set_screen_position( destination.screen_position);
      }
      if (current_node_.get_animate_screen_size())
      {
        object.set_screen_size( destination.screen_size);
      }

      if (get_next_node()==false)
      {
        object.set_animate( false);
      }
      else
      {
        set_relative_data( object);
      }
    }
    else
    {
      float percentage = current_node_.get_percentage();
      T_Position position = object.get_position();
      T_Rotation rotation = object.get_rotation();
      T_Size size = object.get_size();
      T_ScreenPosition screen_position = object.get_screen_position();
      T_ScreenSize screen_size = object.get_screen_size();

      if (current_node_.get_animate_position())
      {
        /* Animation in the X direction. */
        diff = destination.position.x - origin.position.x;
        if (diff > 0.000001 || diff < -0.000001)
        {
          position.x = (diff * percentage) + origin.position.x;
        }
        
        /* Animation in the Y direction. */
        diff = destination.position.y - origin.position.y;
        if (diff > 0.000001 || diff < -0.000001)
        {
          position.y = (diff * percentage) + origin.position.y;
        }
        
        /* Animation in the Z direction. */
        diff = destination.position.z - origin.position.z;
        if (diff > 0.000001 || diff < -0.000001)
        {
          position.z =(diff * percentage) + origin.position.z;
        }
        object.set_position( position);
      }

      if (current_node_.get_animate_rotation())
      {
        /* Rotation on the X axis. */
        diff = destination.rotation.x - origin.rotation.x;
        if (diff > 0.000001 || diff < -0.000001)
        {
          rotation.x = (diff * percentage) + origin.rotation.x;
        }
        
        /* Rotation on the Y axis. */
        diff = destination.rotation.y - origin.rotation.y;
        if (diff > 0.000001 || diff < -0.000001)
        {
          rotation.y =(diff * percentage) + origin.rotation.y;
        }
        
        /* Rotation on the Z axis. */
        diff = destination.rotation.z - origin.rotation.z;
        if (diff > 0.000001 || diff < -0.000001)
        {
          rotation.z = (diff * percentage) + origin.rotation.z;
        }
        object.set_rotation( rotation);
      }

      if (current_node_.get_animate_size())
      {
        /* Width */
        diff = destination.size.width - origin.size.width;
        if (diff > 0)
        {
          size.width = (GLuint)((diff * percentage) + origin.size.width);
        }

        /* Height */
        diff = destination.size.height - origin.size.height;
        if (diff != 0)
        {
          size.height = (GLuint)((diff * percentage) + origin.size.height);
        }

        object.set_size( size);
      }

      if (current_node_.get_animate_screen_position())
      {
        /* Animation in the X direction. */
        diff = (float)destination.screen_position.x - (float)origin.screen_position.x;
        if ((int)diff != 0)
        {
          screen_position.x = (GLuint)((diff * percentage) + origin.screen_position.x);
        }
        
        /* Animation in the Y direction. */
        diff = (float)destination.screen_position.y - (float)origin.screen_position.y;
        if ((int)diff != 0)
        {
          screen_position.y = (GLuint)((diff * percentage) + origin.screen_position.y);
        }
        
        object.set_screen_position( screen_position);
      }

      if (current_node_.get_animate_screen_size())
      {
        /* Width */
        diff = (float)destination.screen_size.width - (float)origin.screen_size.width;
        if ((int)diff != 0)
        {
          screen_size.width = (GLuint)((diff * percentage) + origin.screen_size.width);
        }

        /* Height */
        diff = (float)destination.screen_size.height - (float)origin.screen_size.height;
        if ((int)diff != 0)
        {
          screen_size.height = (GLuint)((diff * percentage) + origin.screen_size.height);
        }

        object.set_screen_size( screen_size);
      }
    }
  }

  return object.get_animate();
}

