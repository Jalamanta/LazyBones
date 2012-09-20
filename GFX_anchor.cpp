#include <iostream>
#include "GFX_anchor.h"
#include "LBMT_media.h"

using namespace std;
 
void GFX_anchor::set_media_index( int media_index)
{
  media_index_ = media_index;
}

int GFX_anchor::get_media_index( void)
{
  return media_index_;
}

GFX_anchor& GFX_anchor::operator=( const GFX_anchor& rhs)
{
  if (this != &rhs)
  {
    GFX_media::operator=( rhs);
    media_index_ = rhs.media_index_;
  }

  return *this;
}
