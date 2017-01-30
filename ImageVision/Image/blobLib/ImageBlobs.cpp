

#include "stdafx.h"
#include "ImageBlobs.h"

//#include "vec2D.h"
//#include "vec2Dc.h"




ImageBlobs::ImageBlobs() : m_image(0)
{
}

ImageBlobs::~ImageBlobs()
{
        if (m_image != 0)
                delete m_image;        
        delete_blobs();
}

void ImageBlobs::init(unsigned int width, unsigned int height)
{        
        if (m_image != 0)
                delete m_image;
        m_image = new vec2Dc(height, width);        
}

int ImageBlobs::find_blobs(const vec2Dc& image, unsigned int min_elements_per_blob)
{
        if (m_image == 0)       //not initialized
                return -1;
        
        m_image->copy(image);        

        while (true) {
                struct Blob blob; 
                blob.elements_number = 0;
                blob.area = 0;

                unsigned int y, x;
                //find first non-zero entry//////////////////////////////////
                for (y = 0; y < m_image->height(); y++) {
                        for (x = 0; x < m_image->width(); x++) {
                                if ((*m_image)(y, x) != 0) {                                
                                        struct Element element;
                                        element.coord.x = x;
                                        element.coord.y = y;                                                                                
                                        blob.elements_number = 1;                                        
                                        blob.elements.push_back(element);                                        
                                        blob.area = 0;
                                        memset(&blob.bounding_box, 0, sizeof(RECT));
                                        break;
                                }
                        }
                        if (blob.elements_number > 0)
                                break;
                }

                if (blob.elements_number == 0) {
                        mark_blobs_on_image();
                        return get_blobs_number();                
                }

                blob.elements.reserve(m_image->width() * m_image->height());
                //find blob//////////////////////////////////////////////////
                unsigned int index = 0;
                while (index < blob.elements_number) {
                        unsigned int N = (unsigned int)blob.elements_number;
                        for (unsigned int i = index; i < N; i++) {
                                add_up_neighbour(blob, i);
                                add_right_neighbour(blob, i);
                                add_down_neighbour(blob, i);
                                add_left_neighbour(blob, i);                                                
                        }
                        index = N;
                }
                remove_blob_from_image(blob);

                if (blob.elements_number > min_elements_per_blob) {
                        blob.area = (unsigned int)blob.elements_number;
                        blob.elements.reserve(blob.elements_number);
                        m_blobs.push_back(blob);                                                                
                }                
        }        
}

void ImageBlobs::delete_blobs()
{
        for (unsigned int i = 0; i < get_blobs_number(); i++) {
                for (unsigned int j = 0; j < (unsigned int)m_blobs[i].elements_number; j++)
                        m_blobs[i].elements[j].neighbs.clear();
                m_blobs[i].elements.clear();
        }
        m_blobs.clear();
}

void ImageBlobs::remove_blob_from_image(const struct Blob& blob)
{
        for (unsigned int i = 0; i < (unsigned int)blob.elements_number; i++) {
                const struct Element &element = blob.elements[i];                
                (*m_image)(element.coord.y, element.coord.x) = 0;
        }
}

void ImageBlobs::mark_blobs_on_image()
{
        for (unsigned int i = 0; i < get_blobs_number(); i++) {
                const struct Blob* blob = get_blob(i);
                for (unsigned int j = 0; j < (unsigned int)blob->elements_number; j++) {
                        const struct Element& element = blob->elements[j];
                        (*m_image)(element.coord.y, element.coord.x) = i + 1;                        
                }        
        }
}

void ImageBlobs::find_bounding_boxes()
{
        for (unsigned int i = 0; i < get_blobs_number(); i++) {
                Blob& blob = m_blobs[i];
                blob.bounding_box.top = m_image->height();
                blob.bounding_box.left = m_image->width();
                blob.bounding_box.right = 0;
                blob.bounding_box.bottom = 0;
                for (unsigned int j = 0; j < (unsigned int)blob.elements_number; j++) {
                        const struct Element& element = blob.elements[j];
                        if (element.coord.y < blob.bounding_box.top)
                                blob.bounding_box.top = element.coord.y;
                        if (element.coord.x < blob.bounding_box.left)
                                blob.bounding_box.left = element.coord.x;
                        if (element.coord.x > blob.bounding_box.right)
                                blob.bounding_box.right = element.coord.x;
                        if (element.coord.y > blob.bounding_box.bottom)
                                blob.bounding_box.bottom = element.coord.y;                        
                }
                blob.bounding_box.right++;
                blob.bounding_box.bottom++;
        }
}