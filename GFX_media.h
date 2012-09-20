#ifndef __GFX_MEDIA_H
#define __GFX_MEDIA_H

#include <iostream>

#include "GFX_object.h"
#include "LBMT_media.h"

class GFX_media: public GFX_object
{
  private:

    LBMT_Media    *media_;
    float         title_height_;

  public:

    GFX_media();

    GFX_media( LBMT_Media *media);
    GFX_media( const GFX_media& rhs);

    virtual ~GFX_media();

    GFX_media& operator=( const GFX_media& rhs);

    std::string get_artist( void);
    std::string get_title( void);

    void set_title_height( float height);
    float get_title_height( void);

    void set_media( LBMT_Media *media);
    LBMT_Media* get_media( void);

    void draw( void);
};

#endif
