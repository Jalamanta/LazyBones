#include <iostream>
#include "GFX_media.h"

using namespace std;
 
GFX_media::GFX_media()
{
  media_ = 0;
  title_height_ = 0.15;
}

GFX_media::GFX_media( LBMT_Media *media)
{
  media_ = media;
  title_height_ =  0.15;
}

GFX_media::GFX_media( const GFX_media& rhs)
{
  media_ = rhs.media_;
  title_height_ =  0.15;
}

GFX_media::~GFX_media()
{
}

GFX_media& GFX_media::operator=( const GFX_media& rhs)
{
  if (this != &rhs)
  {
    GFX_object::operator=( rhs);
    media_ = rhs.media_;
    title_height_ = rhs.title_height_;
  }

  return *this;
}

void GFX_media::set_media( LBMT_Media *media)
{
  media_ = media;
}

LBMT_Media* GFX_media::get_media( void)
{
  return media_;
}

string GFX_media::get_artist( void)
{
  return media_->get_artist_name();
}

string GFX_media::get_title( void)
{
  return media_->get_title();
}

void GFX_media::set_title_height( float height)
{
  title_height_ = height;
}

float GFX_media::get_title_height( void)
{
  return title_height_;
}

void GFX_media::draw( void)
{
  float aspect = 0.0;
  float width, h_width, h_height;

  aspect = (float)(width_) / (float)height_;
  width = title_height_ * aspect;
  h_width = width / 2.0;
  h_height = title_height_ / 2.0;

  if (mask_ != 0)
  {
    glDisable( GL_DEPTH_TEST);
    glEnable( GL_BLEND);

    glBlendFunc( GL_DST_COLOR, GL_ZERO);
    glPushMatrix();
    {
      glTranslatef( position_.x, position_.y, position_.z);
      glRotatef( rotation_.x, 1.0, 0.0, 0.0);
      glRotatef( rotation_.y, 0.0, 1.0, 0.0);
      glRotatef( rotation_.z, 0.0, 0.0, 1.0);
      
      glBindTexture( GL_TEXTURE_2D, mask_);
      glEnable( GL_TEXTURE_2D);
      
      glBegin(GL_QUADS);
      {
        glNormal3f( 0.0, 0.0, 1.0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( -h_width, h_height, 0.0);  // top left vertex
        glTexCoord2f(0.0f, 0.0f); glVertex3f( -h_width, -h_height, 0.0); // bottom left vertex
        glTexCoord2f(1.0f, 0.0f); glVertex3f( h_width,  -h_height, 0.0);  // bottom right vertex
        glTexCoord2f(1.0f, 1.0f); glVertex3f( h_width,  h_height, 0.0);   // top right vertex
      }
      glEnd();
      glDisable( GL_TEXTURE_2D);
    }
    glPopMatrix();

    glBlendFunc( GL_ONE, GL_ONE);
  }

  glPushMatrix();
  {
    glTranslatef( position_.x, position_.y, position_.z);
    glRotatef( rotation_.x, 1.0, 0.0, 0.0);
    glRotatef( rotation_.y, 0.0, 1.0, 0.0);
    glRotatef( rotation_.z, 0.0, 0.0, 1.0);

    aspect = (float)(width_) / (float)height_;
    width = title_height_ * aspect;
    h_width = width / 2.0;
    h_height = title_height_ / 2.0;

    glBindTexture( GL_TEXTURE_2D, texture_);
    glEnable( GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    {
      glNormal3f( 0.0, 0.0, 1.0);
      glTexCoord2f(0.0f, 1.0f); glVertex3f( -h_width, h_height, 0.0);  // top left vertex
      glTexCoord2f(0.0f, 0.0f); glVertex3f( -h_width, -h_height, 0.0); // bottom left vertex
      glTexCoord2f(1.0f, 0.0f); glVertex3f( h_width,  -h_height, 0.0);  // bottom right vertex
      glTexCoord2f(1.0f, 1.0f); glVertex3f( h_width,  h_height, 0.0);   // top right vertex
    }
    glEnd();
    glDisable( GL_TEXTURE_2D);
  }
  glPopMatrix();

  if (mask_ != 0)
  {
    /* I should check the status of these and return them to what they were before */
    glEnable( GL_DEPTH_TEST);
    glDisable( GL_BLEND);
  }
}

