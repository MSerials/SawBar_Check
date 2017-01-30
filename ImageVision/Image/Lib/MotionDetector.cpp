

#include "stdafx.h"
#include "motiondetector.h"

#include "vec2d.h"
#include "vec2dc.h"


MotionDetector::MotionDetector() : m_status(-1), m_difference_vector(0), m_motion_vector(0), m_tmp_motion_vector(0), m_TH(30)
{
}

MotionDetector::~MotionDetector()
{
        close();
}

void MotionDetector::init(unsigned int image_width, unsigned int image_height, float zoom)
{
        close();        

        unsigned int width = (unsigned int)(zoom * (float)image_width);
        unsigned int height = (unsigned int)(zoom * (float)image_height);
        m_difference_vector = new vec2D(height, width);
        m_motion_vector = new vec2Dc(height, width);
        m_tmp_motion_vector = new vec2Dc(height, width);        

        m_background.init(image_width, image_height, zoom);
        m_image.init(image_width, image_height, zoom);

        m_status = 0;
}

void MotionDetector::close()
{        
        if (m_difference_vector != 0) {
                delete m_difference_vector;
                m_difference_vector = 0;
        }
        if (m_motion_vector != 0) {
                delete m_motion_vector;
                m_motion_vector = 0;
        }
        if (m_tmp_motion_vector != 0) {
                delete m_tmp_motion_vector;
                m_tmp_motion_vector = 0;
        }
        m_status = -1;
}

const vec2Dc* MotionDetector::detect(const unsigned char* pBGR)
{
        if (status() < 0)
                return 0;
        
        m_image.resize(pBGR);

        //RGB version
        char** r1 = m_image.getr();
        char** g1 = m_image.getg();
        char** b1 = m_image.getb();
        char** r2 = m_background.getr();
        char** g2 = m_background.getg();
        char** b2 = m_background.getb();
        for (unsigned int y = 0; y < m_motion_vector->height(); y++) {
                for (unsigned int x = 0; x < m_motion_vector->width(); x++) {
                        if (abs(r1[y][x] - r2[y][x]) > m_TH ||
                            abs(g1[y][x] - g2[y][x]) > m_TH ||
                            abs(b1[y][x] - b2[y][x]) > m_TH)
                                (*m_motion_vector)(y, x) = 1;                        
                        else
                                (*m_motion_vector)(y, x) = 0;                        
                }
        }

        //gray scale version
        /*m_difference_vector->sub(*m_image.gety(), *m_background.gety());        
        for (unsigned int y = 0; y < m_motion_vector->height(); y++) {
                for (unsigned int x = 0; x < m_motion_vector->width(); x++) {
                        if (fabs((*m_difference_vector)(y, x)) > m_TH)
                                (*m_motion_vector)(y, x) = 1;                        
                        else
                                (*m_motion_vector)(y, x) = 0;                        
                }
        }*/

        m_tmp_motion_vector->dilate(*m_motion_vector, 3, 3);        
        m_motion_vector->erode(*m_tmp_motion_vector, 5, 5);
        m_tmp_motion_vector->dilate(*m_motion_vector, 3, 3);                

        return m_tmp_motion_vector;
}
