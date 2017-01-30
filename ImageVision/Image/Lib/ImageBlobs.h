

#ifndef ImageBlobs_h
#define ImageBlobs_h


struct Coord {
        int x;
        int y;
};

struct Element {
        vector<struct Element> neighbs;
        struct Coord coord;        
};

struct Blob {
        unsigned int elements_number;
        vector<struct Element> elements;
        unsigned int area;
        RECT bounding_box;      //[top,left; right,bottom)    
};


//class vec2D;
//class vec2Dc;
#include "vec2D.h"
#include "vec2Dc.h"



class ImageBlobs
{
public:
        ImageBlobs();
        //ImageBlobs(const ImageBlobs& blobs);
        ~ImageBlobs();

// Operators
        //const ImageBlobs& operator=(const ImageBlobs& blobs);

// Operations
        void init(unsigned int width, unsigned int height);
        int find_blobs(const vec2Dc& image, unsigned int min_elements_per_blob = 0);
        void find_bounding_boxes();
        void delete_blobs();

// Access
// Inquiry
        inline unsigned int get_blobs_number() const;
        inline const struct Blob* get_blob(unsigned int i) const;
        inline const vec2Dc* get_image() const;        

protected:
private:
        ImageBlobs(const ImageBlobs& blobs);
        const ImageBlobs& operator=(const ImageBlobs& blobs);
        

        vec2Dc* m_image;                        //image with blobs 1, 2, ... N after find_blobs()        
        vector<struct Blob> m_blobs;


        void remove_blob_from_image(const struct Blob& blob);
        void mark_blobs_on_image();

        //if element has new_element as neighbour
        inline bool has_neighbour(const struct Element& element, const struct Element& new_element) const; 
        inline int is_element_present(const struct Blob& blob, const struct Element& new_element) const;
        inline unsigned int add_up_neighbour(struct Blob& blob, unsigned int i);
        inline unsigned int add_right_neighbour(struct Blob& blob, unsigned int i);
        inline unsigned int add_down_neighbour(struct Blob& blob, unsigned int i);
        inline unsigned int add_left_neighbour(struct Blob& blob, unsigned int i);
                       
};

// Inlines
inline unsigned int ImageBlobs::get_blobs_number() const
{
        return (unsigned int)m_blobs.size();
}

inline const struct Blob* ImageBlobs::get_blob(unsigned int i) const
{
        return &m_blobs[i];
}

inline const vec2Dc* ImageBlobs::get_image() const
{
        return m_image;
}

inline int ImageBlobs::is_element_present(const struct Blob& blob, const struct Element& new_element) const
{
        //int index = 0;
        for(int i = (int)blob.elements_number - 1; i >= 0; i--) {        
                const struct Element& element = blob.elements[i];
                if (element.coord.x == new_element.coord.x &&
                    element.coord.y == new_element.coord.y) {
                            //wprintf(L" %d\n", blob.elements_number - 1 - i);
                            return i;
                }
                //if (++index > 2)  //inspect at least 2 last elements
                //        break;                
        }
        return -1;
}

inline bool ImageBlobs::has_neighbour(const struct Element& element, const struct Element& new_element) const
{
        unsigned int N = (unsigned int)element.neighbs.size();
        if (N > 0) {
                for (unsigned int i = 0; i < N; i++) {
                        if (element.neighbs[i].coord.x == new_element.coord.x &&
                            element.neighbs[i].coord.y == new_element.coord.y) 
                            return true;                        
                }
                return false;
        }
        else
                return false;
}

inline unsigned int ImageBlobs::add_up_neighbour(struct Blob& blob, unsigned int i)
{
        const struct Element& element = blob.elements[i];
        if (element.coord.y - 1 < 0)
                return 0;
        else if ((*m_image)(element.coord.y - 1, element.coord.x) > 0) {
                struct Element new_element;
                new_element.coord.x = element.coord.x;
                new_element.coord.y = element.coord.y - 1;                                
                if (has_neighbour(element, new_element) == false) { 
                        int index = is_element_present(blob, new_element);
                        if (index >= 0) {
                                blob.elements[index].neighbs.push_back(element);
                                return 0;
                        }                        
                        new_element.neighbs.push_back(element);
                        blob.elements_number++;
                        blob.elements.push_back(new_element);                        
                        return 1;
                }
                else
                        return 0;
        }
        else 
                return 0;
}

inline unsigned int ImageBlobs::add_right_neighbour(struct Blob& blob, unsigned int i)
{
        const struct Element& element = blob.elements[i];
        if (element.coord.x + 1 >= m_image->width())
                return 0;
        else if ((*m_image)(element.coord.y, element.coord.x + 1) > 0) {
                struct Element new_element;
                new_element.coord.x = element.coord.x + 1;
                new_element.coord.y = element.coord.y;                
                if (has_neighbour(element, new_element) == false) {
                        int index = is_element_present(blob, new_element);
                        if (index >= 0) {
                                blob.elements[index].neighbs.push_back(element);
                                return 0;
                        }                        
                        blob.elements_number++;
                        blob.elements.push_back(new_element);                        
                        return 1;
                }
                else
                        return 0;
        }
        else 
                return 0;
}

inline unsigned int ImageBlobs::add_down_neighbour(struct Blob& blob, unsigned int i)
{
        const struct Element& element = blob.elements[i];
        if (element.coord.y + 1 >= m_image->height())
                return 0;
        else if ((*m_image)(element.coord.y + 1, element.coord.x) > 0) {
                struct Element new_element;
                new_element.coord.x = element.coord.x;
                new_element.coord.y = element.coord.y + 1;                
                if (has_neighbour(element, new_element) == false) {
                        int index = is_element_present(blob, new_element);
                        if (index >= 0) {
                                blob.elements[index].neighbs.push_back(element);
                                return 0;
                        }                        
                        blob.elements_number++;
                        blob.elements.push_back(new_element);                        
                        return 1;
                }
                else
                        return 0;
        }
        else 
                return 0;
}

inline unsigned int ImageBlobs::add_left_neighbour(struct Blob& blob, unsigned int i)
{
        const struct Element& element = blob.elements[i];
        if (element.coord.x - 1 < 0)
                return 0;
        else if ((*m_image)(element.coord.y, element.coord.x - 1) > 0) {
                struct Element new_element;
                new_element.coord.x = element.coord.x - 1;
                new_element.coord.y = element.coord.y;                
                if (has_neighbour(element, new_element) == false) {
                        int index = is_element_present(blob, new_element);
                        if (index >= 0) {
                                blob.elements[index].neighbs.push_back(element);
                                return 0;
                        }                        
                        blob.elements_number++;
                        blob.elements.push_back(new_element);                        
                        return 1;
                }
                else
                        return 0;
        }
        else 
                return 0;
}

#endif ImageBlobs_h

