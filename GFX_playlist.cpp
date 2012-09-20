#include <iostream>
#include <math.h>
#include "GFX_playlist.h"
#include "LBMT_iterator.h"

using namespace std;

#include <FTGL/FTGLPixmapFont.h>

#define COMIC_FONT "/usr/share/fonts/corefonts/comicbd.ttf"

GLfloat light_position[4] = { 0.0, -0.5, 2.0, 0.0};

GFX_playlist::GFX_playlist( BMT_texture_manager *texture_manager,
                            GLuint screen_x, GLuint screen_y,
                            GLuint width, GLuint height):
  texture_manager_(texture_manager)
{
  set_screen_position( screen_x, screen_y);
  set_screen_size( width, height);

  init();
}

GFX_playlist::~GFX_playlist()
{
}

void GFX_playlist::set_position( GLfloat x, GLfloat y, GLfloat z)
{
  original_x_pos_ = x;
  original_y_pos_ = y;
  original_z_pos_ = z;

  GFX_object::set_position( x, y, z);
}

void GFX_playlist::reset_positions( void)
{
  GLfloat      x_position = 0.0;
  GLfloat      y_rotation = 0.0;
  GLfloat      z_position = 0.0;
  unsigned int i;

  set_position( original_x_pos_,
                original_y_pos_,
                original_z_pos_);

  for (i=0; i < anchors_.size(); i ++)
  {
    if (i < media_nbr_left_)
    {
      x_position = -media_x_spacing_ * (media_nbr_left_ - i + 1) - media_x_offset_;
      y_rotation = media_y_angle_;
      z_position = 0.0;
    }
    else if (i == media_nbr_left_)
    {
      x_position = 0.0;
      y_rotation = 0.0;
      z_position = selected_media_z_offset_;
      selected_media_ = i;
    }
    else 
    {
      x_position = media_x_spacing_ * (i - media_nbr_left_ + 1) + media_x_offset_;
      y_rotation = -media_y_angle_;
      z_position = 0.0;
    }
  
    anchors_[i].set_position( x_position, 0.0f, z_position);
    anchors_[i].set_rotation( 0.0f, y_rotation, 0.0f);
  }
}

void GFX_playlist::init_anchors( void)
{
  unsigned int i;

  aspect_ = (float)get_screen_width() / (float)get_screen_height();
  media_nbr_left_ = (unsigned int)(3.75 * aspect_);
  media_nbr_right_ = (unsigned int)(3.75 * aspect_);

  number_anchors_ = media_nbr_left_ + 1 + media_nbr_right_;

  GFX_anchor anchor_point;

  // BM: Set up initial media anchor points (Visible Media).
  for (i=0; i < number_anchors_; i ++)
  {
    anchors_.push_back( anchor_point);
  }

  reset_positions();
}

void GFX_playlist::init( void)
{
  selected_media_ = 0;
  selected_media_z_offset_ = 0.15;
  media_y_angle_ = 70.0;
  media_x_spacing_ = 0.03;
  media_x_offset_ = 0.08;
  scale_ = 1.0;

  current_animation_ = ANIM_NONE;

  comic_font_small_ = new FTGLPixmapFont( COMIC_FONT);
  comic_font_small_->FaceSize( 18);
  comic_font_small_->UseDisplayList( true);

  comic_font_large_ = new FTGLPolygonFont( COMIC_FONT);
  comic_font_large_->FaceSize( 34);
  comic_font_large_->UseDisplayList( true);

  playlist_ = 0;

  init_anchors();
}

void GFX_playlist::add_playlist( LBMT_MediaBag *playlist)
{
}

void GFX_playlist::add_album( LBMT_Album *album)
{
  unsigned int media_index;
  int texture_id;

  if (playlist_ == 0)
  {
    playlist_ = new LBMT_Playlist();
  }

  playlist_->add_album( *album);

  if (playlist_->size() == 1)
  {
    playlist_left_it_ = playlist_->begin();
    playlist_right_it_ = playlist_->begin();
  }

  if (playlist_->size() < anchors_.size())
  {
    if (playlist_->size() > 1)
    {
      playlist_right_it_++;
    }

    texture_id = texture_manager_->add( album->get_coverart());

    media_index = playlist_->size();

    anchors_[media_index].set_media( album);
    anchors_[media_index].set_texture( texture_id);
    anchors_[media_index].set_media_index( media_index);

    // BM: This album is situated at the currently selected media anchor
    if (selected_media_ == media_index)
    {
      selected_media_iterator_ = playlist_right_it_;
    }
  }
}

void GFX_playlist::add_media( LBMT_Media *media)
{

  if (dynamic_cast<LBMT_Album*>(media) != 0)
  {
    add_album( (dynamic_cast<LBMT_Album*>(media)));
  }
  else if (dynamic_cast<LBMT_Song*>(media) != 0)
  {
//    add_song( (dynamic_cast<LBMT_Song*>(media)));
  }
  else if (dynamic_cast<LBMT_MediaBag*>(media) != 0)
  {
    add_playlist( (dynamic_cast<LBMT_MediaBag*>(media)));
  }
}

void GFX_playlist::remove_album( LBMT_Album& album)
{
  playlist_->remove_album( album);
}

void GFX_playlist::add_song( LBMT_Song& song)
{
  playlist_->add_song( song);
}

void GFX_playlist::remove_song( LBMT_Song& song)
{
  playlist_->remove_song( song);
}

void GFX_playlist::set_selected_media( int media_id)
{
  selected_media_ = media_id;
}

GFX_media& GFX_playlist::get_selected_media( void)
{
  return anchors_[selected_media_];
}

void GFX_playlist::set_scale( float value)
{
  if (value < -1.0)
  {
    scale_ = -1.0;
  }
  else if (value > 1.0)
  {
    scale_ = 1.0;
  }
  else
  {
    scale_ = value;
  }
}

float GFX_playlist::get_scale( void)
{
  return scale_;
}

void GFX_playlist::set_aspect( float value)
{
  aspect_ = value;
}

float GFX_playlist::get_aspect( void)
{
  return aspect_;
}

void GFX_playlist::animate_all_media_left( int millisecs)
{
  unsigned int i, j;
  T_Position destination_position;
  T_Rotation destination_rotation;

  if (get_animate() == false)
  {
    if (anchors_[selected_media_+1].get_media() != 0)
    {
      i=0;
      while( i <= selected_media_ && i < anchors_.size())
      {
        GFX_anim_node anim_node;

        j = selected_media_ - i;

        anim_node.set_origin_position( anchors_[i].get_position());
        anim_node.set_origin_rotation( anchors_[i].get_rotation());

        destination_position.x = (-media_x_spacing_ * (j+2)) - media_x_offset_;
        destination_position.y = 0.0;
        destination_position.z = 0.0;
        
        destination_rotation.x = 0.0;
        destination_rotation.y = media_y_angle_;
        destination_rotation.z = 0.0;
        
        anim_node.set_destination_position( destination_position);
        anim_node.set_destination_rotation( destination_rotation);
        anim_node.set_time( millisecs);

        anchors_[i].add_node( anim_node);
        anchors_[i].set_animate( true);
        i++;
      }
      
      if (i < anchors_.size())
      {
        GFX_anim_node anim_node;

        anim_node.set_origin_position( anchors_[i].get_position());
        anim_node.set_origin_rotation( anchors_[i].get_rotation());
        
        destination_position.x = 0.0;
        destination_position.y = 0.0;
        destination_position.z = selected_media_z_offset_;
        
        destination_rotation.x = 0.0;
        destination_rotation.y = 0.0;
        destination_rotation.z = 0.0;
        
        anim_node.set_destination_position( destination_position);
        anim_node.set_destination_rotation( destination_rotation);
        anim_node.set_time( millisecs);

        anchors_[i].add_node( anim_node);
        anchors_[i].set_animate( true);
        i++;
      }
      
      j = 0;
      while( i < anchors_.size())
      {
        GFX_anim_node anim_node;

        anim_node.set_origin_position( anchors_[i].get_position());
        anim_node.set_origin_rotation( anchors_[i].get_rotation());
        
        destination_position.x = (media_x_spacing_ * (j+2)) + media_x_offset_;
        destination_position.y = 0.0;
        destination_position.z = 0.0;
        
        destination_rotation.x = 0.0;
        destination_rotation.y = -media_y_angle_;
        destination_rotation.z = 0.0;
        
        anim_node.set_destination_position( destination_position);
        anim_node.set_destination_rotation( destination_rotation);
        anim_node.set_time( millisecs);

        anchors_[i].add_node( anim_node);
        anchors_[i].set_animate( true);
        i++;
        j++;
      }
      current_animation_ = ANIM_LEFT;
    }
  }
}

void GFX_playlist::animate_all_media_right( int millisecs)
{
  unsigned int i, j;
  T_Position destination_position;
  T_Rotation destination_rotation;

  if (get_animate() == false)
  {
    if (anchors_[selected_media_-1].get_media() != 0)
    {
      i=0;
      while( i < (selected_media_ -1) && i < anchors_.size())
      {
        GFX_anim_node anim_node;

        j = selected_media_ - i;

        anim_node.set_origin_position( anchors_[i].get_position());
        anim_node.set_origin_rotation( anchors_[i].get_rotation());
        
        destination_position.x = (-media_x_spacing_ * (j)) - media_x_offset_;
        destination_position.y = 0.0;
        destination_position.z = 0.0;
        
        destination_rotation.x = 0.0;
        destination_rotation.y = media_y_angle_;
        destination_rotation.z = 0.0;
        
        anim_node.set_destination_position( destination_position);
        anim_node.set_destination_rotation( destination_rotation);
        anim_node.set_time( millisecs);

        anchors_[i].add_node( anim_node);
        anchors_[i].set_animate( true);

        i++;
      }
      
      if (i < anchors_.size())
      {
        GFX_anim_node anim_node;

        anim_node.set_origin_position( anchors_[i].get_position());
        anim_node.set_origin_rotation( anchors_[i].get_rotation());
        
        destination_position.x = 0.0;
        destination_position.y = 0.0;
        destination_position.z = selected_media_z_offset_;
        
        destination_rotation.x = 0.0;
        destination_rotation.y = 0.0;
        destination_rotation.z = 0.0;
        
        anim_node.set_destination_position( destination_position);
        anim_node.set_destination_rotation( destination_rotation);
        anim_node.set_time( millisecs);

        anchors_[i].add_node( anim_node);
        anchors_[i].set_animate( true);
        
        i++;
      }
      
      j = 0;
      while( i < anchors_.size())
      {
        GFX_anim_node anim_node;

        anim_node.set_origin_position( anchors_[i].get_position());
        anim_node.set_origin_rotation( anchors_[i].get_rotation());
        
        destination_position.x = (media_x_spacing_ * (j + 2)) + media_x_offset_;
        destination_position.y = 0.0;
        destination_position.z = 0.0;
        
        destination_rotation.x = 0.0;
        destination_rotation.y = -media_y_angle_;
        destination_rotation.z = 0.0;
        
        anim_node.set_destination_position( destination_position);
        anim_node.set_destination_rotation( destination_rotation);
        anim_node.set_time( millisecs);

        anchors_[i].add_node( anim_node);
        anchors_[i].set_animate( true);

        i++;
        j++;
      }
      current_animation_ = ANIM_RIGHT;
    }
  }
}

void GFX_playlist::resize( void)
{
#if 0
  unsigned int i, difference;
  unsigned int previous_number_of_anchors = number_anchors_;
  GLuint texture_id;
  int media_index, prev_media_index;
  GFX_anchor temp_anchor;

  aspect_ = (float)get_screen_width() / (float)get_screen_height();

  media_nbr_left_ = (unsigned int)(3.75 * aspect_);
  media_nbr_right_ = (unsigned int)(3.75 * aspect_);
  
  number_anchors_ = media_nbr_left_ + 1 + media_nbr_right_;

  difference = abs((int)(previous_number_of_anchors - number_anchors_));

  if (difference != 0)
  {
    media_index = anchors_[selected_media_].get_media_index();

    if (number_anchors_ > previous_number_of_anchors)
    {
      GFX_anchor anchor_point;
      anchor_point.set_media_index( -1);
      anchor_point.set_media( 0);
      
      for (i=0; i < difference; i++)
      {
        anchors_.push_back( anchor_point);
      }
      selected_media_ += difference/2;
    }
    else if (previous_number_of_anchors > number_anchors_)
    {
      for (i=0; i < difference; i++)
      {
        temp_anchor = anchors_.pop_back();
        texture_manager_->set_texture_unused( temp_anchor->get_texture());
      }
      selected_media_ -= difference/2;
    }

    if (anchors_.size() > 0)
    {
      
      texture_id = texture_manager_->add( playlist_[media_index].filename, /* Attempt Texture Reuse = */ true);
      anchors_[selected_media_].set_texture( texture_id);
      anchors_[selected_media_].set_media( playlist_[media_index]);
      anchors_[selected_media_].set_media_index( media_index);
      
      for (i=selected_media_-1; i <= 0; i--)
      {
        texture_id = texture_manager_->add( playlist_[media_index].filename, /* Attempt Texture Reuse = */ true);
        anchors_[i].set_texture( texture_id);
        anchors_[i].set_media( playlist_[media_index]);
        anchors_[i].set_media_index( media_index);
        media_index--;
        if (media_index < 0)
        {
          break;
        }
      }
      
      for (i=selected_media_+1; i <= anchors_.size(); i++)
      {
        texture_id = texture_manager_->add( playlist_[media_index].filename, /* Attempt Texture Reuse = */ true);
        anchors_[i].set_texture( texture_id);
        anchors_[i].set_media( playlist_[media_index]);
        anchors_[i].set_media_index( media_index);
        media_index++;
        if (media_index > playlist_.size())
        {
          break;
        }
      }
    }

//    reset_positions();
  }
#endif
}

void GFX_playlist::move_textures_left( void)
{
  GLuint texture_id = 0;
  unsigned int i;
  LBMT_Media *media = 0;

  if (anchors_.size() > 0)
  {
    texture_id = anchors_[0].get_texture();
    texture_manager_->set_texture_unused( texture_id);
  }

  if (anchors_[0].get_media() != 0)
  {
    if (playlist_left_it_ != playlist_->last())
    {
      playlist_left_it_++;
    }
  }

  for(i = 0; i < number_anchors_ - 1; i++)
  {
    anchors_[i].set_texture( anchors_[i+1].get_texture()); // Should Use A Copy Constructor
    anchors_[i].set_media( anchors_[i+1].get_media());
  }

  if (anchors_[number_anchors_-1].get_media() != 0 &&
      playlist_right_it_ != playlist_->last())
  {
    playlist_right_it_++;
    media = *playlist_right_it_;
  }

  texture_id = 0;

  if (media != 0)
  {
    texture_id = texture_manager_->add( media->get_coverart(), /* Attempt Texture Reuse = */ true);
  }

  anchors_[number_anchors_-1].set_texture( texture_id);
  anchors_[number_anchors_-1].set_media( media);
}

void GFX_playlist::move_textures_right( void)
{
  GLuint texture_id = 0;
  unsigned int i;
  LBMT_Media *media = 0;

  if (number_anchors_ -1 > 0)
  {
    texture_id = anchors_[number_anchors_-1].get_texture();
    texture_manager_->set_texture_unused( texture_id);
  }

  if (anchors_[number_anchors_-1].get_media() != 0)
  {
    if (playlist_right_it_ != playlist_->begin())
    {
      playlist_right_it_--;
    }
  }

  for(i = number_anchors_ -1; i > 0; i--)
  {
    anchors_[i].set_texture( anchors_[i-1].get_texture()); // Should Use A Copy Constructor
    anchors_[i].set_media( anchors_[i-1].get_media());
  }

  // BM: Following code fills the first left anchor only
  if (anchors_[0].get_media() != 0 &&
      playlist_left_it_ != playlist_->begin())
  {
    playlist_left_it_--;
    media = *playlist_left_it_;
  }

  texture_id = 0;

  if (media != 0)
  {
    texture_id = texture_manager_->add( media->get_coverart(), /* Attempt Texture Reuse = */ true);
  }

  anchors_[0].set_texture( texture_id);
  anchors_[0].set_media( media);
}

void GFX_playlist::spin_selected_media( void)
{
  T_Rotation    destination_rotation;
  T_Position    destination_position;
  GFX_anim_node anim_node;

  anim_node.set_origin_position( anchors_[selected_media_].get_position());
  anim_node.set_origin_rotation( anchors_[selected_media_].get_rotation());

  destination_rotation.x = 0.0;
  destination_rotation.y = 360.0;
  destination_rotation.z = 0.0;
  
  anim_node.set_destination_position( anchors_[selected_media_].get_position());
  anim_node.set_destination_rotation( destination_rotation);
  anim_node.set_time( 600);

  anchors_[selected_media_].add_node( anim_node);

  anim_node.clear();

  anim_node.set_origin_position( anchors_[selected_media_].get_position());
  anim_node.set_origin_rotation( anchors_[selected_media_].get_rotation());

  destination_position.x = 0.0;
  destination_position.y = -0.16;
  destination_position.z = 0.1;

  anim_node.set_destination_position( destination_position);
  anim_node.set_destination_rotation( anchors_[selected_media_].get_rotation());
  anim_node.set_time( 600);

  anchors_[selected_media_].add_node( anim_node);
  anchors_[selected_media_].set_animate( true);
}
  
bool GFX_playlist::animate( int millisecs)
{
  unsigned int i;
  bool         animating = false;
  bool         was_animating = false;

  was_animating = get_animate();
  animating = GFX_object::animate( millisecs);

  for (i=0 ; i < anchors_.size(); i++)
  {
    was_animating |= anchors_[i].get_animate();
    animating |= anchors_[i].animate( millisecs);
  }
  
  if (was_animating && animating == false)
  {
    reset_positions();

    switch( current_animation_)
    {
      case ANIM_LEFT:
        move_textures_left();
        break;

      case ANIM_RIGHT:
        move_textures_right();
        break;

      case ANIM_UP:
        break;
      case ANIM_DOWN:
        break;
      case ANIM_NONE:
        break;
      default:
        break;
    }
    current_animation_ = ANIM_NONE;
  }

  return animating;
}

bool GFX_playlist::get_animate( void)
{
  bool animating = false;
  unsigned int i;

  animating = GFX_object::get_animate();

  for (i=0 ; i < anchors_.size(); i++)
  {
    animating |= anchors_[i].get_animate();
  }

  return animating;
}

void GFX_playlist::draw_selected_media( void)
{
  glPushMatrix();
  {
    glTranslatef( position_.x, position_.y, position_.z);
    glRotatef( rotation_.x, 1.0, 0.0, 0.0);
    glRotatef( rotation_.y, 0.0, 1.0, 0.0);
    glRotatef( rotation_.z, 0.0, 0.0, 1.0);
  
    anchors_[media_nbr_left_].draw();
  }
  glPopMatrix();
}


void GFX_playlist::draw_selected_media_title( void)
{
  float llx, lly, llz, urx, ury, urz;

  string artist;
  string album;
  char album_display[256];

  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0, width_, height_, 0, -1.0f, 1.0f);
  glMatrixMode( GL_MODELVIEW);
  glLoadIdentity();

  glDisable( GL_LIGHTING);
  glColor3f( 0.7, 0.7, 0.7);

  anchors_[selected_media_];

  artist = anchors_[selected_media_].get_artist();
  album = anchors_[selected_media_].get_title();
  sprintf( album_display, "\"%s\"", album.c_str());
  
  comic_font_small_->BBox( artist.c_str(), llx, lly, llz, urx, ury, urz);
  glRasterPos2i( (int)(get_screen_width() - (urx -llx)) / 2, 25);
  comic_font_small_->Render( artist.c_str());

  comic_font_small_->BBox( album_display, llx, lly, llz, urx, ury, urz);
  glRasterPos2i( (int)(get_screen_width() - (urx -llx)) / 2, 50);
  comic_font_small_->Render( album_display);
}

void GFX_playlist::draw_floor( void) 
{
  glBegin(GL_QUADS);
  {
    glNormal3f( 0.0, 1.0, 0.0);
    glTexCoord2f( 0.0f, 1.0f); glVertex3f( -2.0, 0.0, 12.0);
    glTexCoord2f( 0.0f, 0.0f); glVertex3f( -2.0, 0.0,-2.0);
    glTexCoord2f( 1.0f, 0.0f); glVertex3f( 2.0, 0.0,-2.0);
    glTexCoord2f( 1.0f, 1.0f); glVertex3f( 2.0, 0.0, 12.0);
  }
  glEnd();
}


void GFX_playlist::draw_media( bool draw_selected)
{
  unsigned int i;

  glPushMatrix();
  {
  
    for (i=0; i < media_nbr_left_; i++)
    {
      if (anchors_[i].get_texture() > 0)
      {
        anchors_[i].draw();
      }
    }
    
    if (draw_selected)
    {
      anchors_[media_nbr_left_].draw();
    }

    for (i=media_nbr_left_+1; i < number_anchors_; i++)
    {
      if (anchors_[i].get_texture() > 0)
      {
        anchors_[i].draw();
      }
    }
  }
  glPopMatrix();
}

void GFX_playlist::draw( bool draw_selected_title)
{
  double equation[] = { 0.0f, -1.0f, 0.0f, 0.0f};
  
  glViewport( get_screen_x(), get_screen_y(), get_screen_width(), get_screen_height());

  glDisable( GL_LIGHTING);
  glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  if (draw_selected_title &&
      get_animate() == false)
  {
    draw_selected_media_title();
  }

  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)get_screen_width() / (GLfloat)get_screen_height(), 0.1f, 20.0f);
  glMatrixMode( GL_MODELVIEW);
  glLoadIdentity();

  glEnable( GL_LIGHTING);

  glTranslatef( position_.x, position_.y, position_.z);
  glRotatef( rotation_.x, 1.0, 0.0, 0.0);
  glRotatef( rotation_.y, 0.0, 1.0, 0.0);
  glRotatef( rotation_.z, 0.0, 0.0, 1.0);

  glScalef( scale_, scale_, scale_);

  glPushMatrix();
  {
    glTranslatef( 0.0f, -0.06f, 0.0);
    glColorMask( 0, 0, 0, 0);
    glEnable( GL_STENCIL_TEST);
    glStencilFunc( GL_ALWAYS, 1, 1);
    glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE);
    glDisable( GL_DEPTH_TEST);

    draw_floor();

    glEnable( GL_DEPTH_TEST);
    glColorMask( 1, 1, 1, 1);

    glStencilFunc( GL_EQUAL, 1, 1);
    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP);
    glClipPlane( GL_CLIP_PLANE0, equation);
    glEnable( GL_CLIP_PLANE0);

    glPushMatrix();
    {
      glScalef( 1.0, -1.0, 1.0);
      glTranslatef( 0.0f, 0.08f, 0.0);
      draw_media( true);
    }
    glPopMatrix();

    glDisable( GL_CLIP_PLANE0);
    glDisable( GL_STENCIL_TEST);
    glEnable( GL_BLEND);
    glDisable( GL_LIGHTING);

    glColor4f( 0.0f, 0.0f, 0.0f, 0.8);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    draw_floor();

    glEnable( GL_LIGHTING);
    glDisable( GL_BLEND);
  }
  glPopMatrix();

  glTranslatef( 0.0f, 0.015f, 0.0);
  draw_media( true);
}

void GFX_playlist::draw( void)
{
  draw( true);
}

void GFX_playlist::animate_playlist_up( int millisecs)
{
  T_Position     origin_position;
  T_Position     destination_position;
  GFX_anim_node  anim_node;

  origin_position.x = original_x_pos_;
  origin_position.y = original_y_pos_ - 0.16;
  origin_position.z = original_z_pos_;

  anim_node.set_origin_position( origin_position);

  destination_position.x = original_x_pos_;
  destination_position.y = original_y_pos_;
  destination_position.z = original_z_pos_;
        
  anim_node.set_destination_position( destination_position);
  anim_node.set_time( millisecs);

  add_node( anim_node);
  set_animate( true);
}

void GFX_playlist::animate_playlist_down( int millisecs)
{
  T_Position     origin_position;
  T_Position     destination_position;
  GFX_anim_node  anim_node;

  origin_position.x = original_x_pos_;
  origin_position.y = original_y_pos_;
  origin_position.z = original_z_pos_;

  anim_node.set_origin_position( origin_position);
        
  destination_position.x = original_x_pos_;
  destination_position.y = original_y_pos_ - 0.16;
  destination_position.z = original_z_pos_;
        
  anim_node.set_destination_position( destination_position);
  anim_node.set_time( millisecs);

  add_node( anim_node);
  set_animate( true);
}


void GFX_playlist::stats( void)
{
  cout << texture_manager_->get_nbr_textures() << endl;
  cout << number_anchors_ << endl;
}

