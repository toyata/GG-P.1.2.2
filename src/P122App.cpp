#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"
#include "cinder/Color.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HUE {
public:
    bool operator()(const Colorf& a, const Colorf& b) const {
        return rgbToHSV(a).x < rgbToHSV(b).x;
        //return a.r < b.r;
    }
};

class P122App : public AppNative {
  public:
    void prepareSettings(Settings *settings);
	void setup();
	void mouseDown(MouseEvent event);
	void mouseMove(MouseEvent event);
	void update();
	void draw();
    
    ci::Surface mSurface;
    gl::Texture mTexture;
    
    list<ColorA> colors;
    
    Vec2i mMouseLoc;
};

void P122App::prepareSettings(Settings *settings) {
	settings->setWindowSize(600, 600);
	settings->setFrameRate(10.0f);
}

void P122App::setup()
{
    mSurface = Surface(loadImage(loadResource("pic1.jpg")));
    mTexture = mSurface;
    
    Vec2i res = mSurface.getSize();
    
    for(int y=0; y < res.y; y++) {
        for(int x=0; x < res.x; x++) {
            //console() << mSurface.getPixel(Vec2i(x, y)) << std::endl;
            ColorA c = mSurface.getPixel(Vec2i(x, y));
            colors.push_back(c);
        }
    }
    
    //colors.sort(HUE());
    //console() << (rgbToHSV(*colors.begin()).x <  rgbToHSV(*colors.end()).x) << std::endl;
}

void P122App::mouseDown(MouseEvent event)
{
}

void P122App::mouseMove(MouseEvent event) {
    mMouseLoc = event.getPos();
}

void P122App::update()
{
}

void P122App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    Vec2i w = getWindowSize();
    int tileCount = w.x / std::max(5, mMouseLoc.x);
    if (tileCount < 1) tileCount = 1;
    float rectSize = w.x / float(tileCount);
    
    //console() << rectSize << std::endl;
    
	list<ColorA>::iterator p = colors.begin();
        
    for(int y=0; y < tileCount; y++) {
        for(int x=0; x < tileCount; x++) {
            ColorA c = *p;
            
            gl::color(c);
            gl::drawSolidRect(Rectf(x*rectSize, y*rectSize, x*rectSize+rectSize, y*rectSize+rectSize));
            
            advance(p, int(rectSize));
        }
        advance(p, int(w.x*(rectSize-1.0f)));
    }
}

CINDER_APP_NATIVE( P122App, RendererGl )
