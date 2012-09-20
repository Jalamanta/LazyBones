#ifndef __GFX_ANCHOR_H
#define __GFX_ANCHOR_H

#include <iostream>
#include "GFX_media.h"
#include "LBMT_media.h"

class GFX_anchor: public GFX_media
{
  private:

    int media_index_;

  public:

    GFX_anchor():media_index_(-1){};

    GFX_anchor( LBMT_Media *media):GFX_media(media),media_index_(-1){};

    virtual ~GFX_anchor(){};

    GFX_anchor& operator=( const GFX_anchor& rhs);

    void set_media_index( int media_index);
    int get_media_index( void);
};

#endif
