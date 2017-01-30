

#ifndef MotionDetector_h
#define MotionDetector_h


#include "ImageResize.h"
class vec2D;
class vec2Dc;


class MotionDetector
{
public:
        MotionDetector();
        //MotionDetector(const MotionDetector& md);
        ~MotionDetector();

// Operators
        //const MotionDetector& operator=(const MotionDetector& md);
        void init(unsigned int image_width, unsigned int image_height, float zoom = 0.125f);
        void close();

        const vec2Dc* detect(const unsigned char* pBGR);

// Operations
// Access
        inline int status() const;
        inline int set_background(const unsigned char* pBGR);
        inline const vec2Dc* get_motion_vector() const;
        inline unsigned char threshold() const;
        inline void threshold(unsigned char th);


// Inquiry

protected:
private:
        MotionDetector(const MotionDetector& md);
        const MotionDetector& operator=(const MotionDetector& md);

        int m_status;

        ImageResize m_background;
        ImageResize m_image;
        vec2D* m_difference_vector;
        vec2Dc* m_motion_vector;
        vec2Dc* m_tmp_motion_vector;

        unsigned char m_TH;

};

// Inlines
inline int MotionDetector::status() const
{
        return m_status;
}

inline int MotionDetector::set_background(const unsigned char* pBGR)
{        
        return m_background.resize(pBGR);
}

inline const vec2Dc* MotionDetector::get_motion_vector() const
{
        return m_motion_vector;
}

inline unsigned char MotionDetector::threshold() const
{
        return m_TH;
}

inline void MotionDetector::threshold(unsigned char th)
{        
        m_TH = th;
}

#endif MotionDetector_h

