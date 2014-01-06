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
        return rgbToHSV(a).x > rgbToHSV(b).x;
    }
};

class SATURATION {
  public:
    bool operator()(const Colorf& a, const Colorf& b) const {
        return rgbToHSV(a).y > rgbToHSV(b).y;
    }
};

class BRIGHTNESS {
  public:
    bool operator()(const Colorf& a, const Colorf& b) const {
        return rgbToHSV(a).z > rgbToHSV(b).z;
    }
};

class GRAYSCALE {
  public:
    bool operator()(const Colorf& a, const Colorf& b) const {
        return GRAYSCALE::rgbToGrayscale(a) > GRAYSCALE::rgbToGrayscale(b);
    }
    static int rgbToGrayscale(const Colorf& c) {
        return (int)((0.299*c.r + 0.587*c.g + 0.114*c.b)*256);
    }
};

class P122App : public AppNative {
  public:
    void prepareSettings(Settings *settings);
	void setup();
	void changeImage(string name);
	void mouseDown(MouseEvent event);
	void mouseMove(MouseEvent event);
	void keyDown(KeyEvent event);
	void update();
	void draw();
    
    ci::Surface mSurface;
    
    list<ColorA> org;
    list<ColorA> colors;
    
    Vec2i mMouseLoc;
};

void P122App::prepareSettings(Settings *settings) {
	settings->setWindowSize(600, 600);
	settings->setFrameRate(10.0f);
}

void P122App::setup()
{
    changeImage("pic1.jpg");
}

void P122App::changeImage(string name)
{
    //console() << name << std::endl;
    
    org.clear();
    
    mSurface = Surface(loadImage(loadResource(name)));
    
    Vec2i res = mSurface.getSize();
    
    for(int y=0; y < res.y; y++) {
        for(int x=0; x < res.x; x++) {
            ColorA c = mSurface.getPixel(Vec2i(x, y));
            org.push_back(c);
        }
    }
    
    colors = org;
}

void P122App::mouseDown(MouseEvent event)
{
}

void P122App::mouseMove(MouseEvent event) {
    mMouseLoc = event.getPos();
}

void P122App::keyDown( KeyEvent event ) {
    switch (event.getChar()) {
        case '1':
            changeImage("pic1.jpg");
            break;
        case '2':
            changeImage("pic2.jpg");
            break;
        case '3':
            changeImage("pic3.jpg");
            break;
        case '4':
            colors = org;
            break;
        case '5':
            colors.sort(HUE());
            break;
        case '6':
            colors.sort(SATURATION());
            break;
        case '7':
            colors.sort(BRIGHTNESS());
            break;
        case '8':
            colors.sort(GRAYSCALE());
            break;
    }
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
    int rectw = w.x / tileCount;
    int dist = w.x - rectw*tileCount;
    
	list<ColorA>::iterator p = colors.begin();
        
    for(int y=0; y < tileCount; y++) {
        for(int x=0; x < tileCount; x++) {
            ColorA c = *p;
            
            gl::color(c);
            gl::drawSolidRect(Rectf(x*rectSize, y*rectSize, x*rectSize+rectSize, y*rectSize+rectSize));
            
            advance(p, rectw);
        }
        advance(p, dist + w.x*(rectw-1));
    }
}

CINDER_APP_NATIVE( P122App, RendererGl )
