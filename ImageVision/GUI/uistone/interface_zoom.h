#pragma once

//-------------------------------------------------------------------------------------
/**
    Zoom scale support.
    scale is a integer, if >= 1 means zoom in, if <= -1 means zoom out.
*/
#pragma warning(disable:4244)

class PCL_Interface_ZoomScale
{
    double   m_scale_x,m_scale_y;
public:
    PCL_Interface_ZoomScale() : m_scale_x(1),m_scale_y(1) {}
    virtual ~PCL_Interface_ZoomScale() {}

    /// Set zoom scale.
    void SetZoomScale (double nScaleX,double nScaleY)
    {
        assert(nScaleX) ;
		assert(nScaleY);
        if (nScaleX)
        {
			m_scale_x = nScaleX ;
        }
		if (nScaleY)
		{
			m_scale_y = nScaleY;
		}
    }

    /// Get zoom scale.
    double GetZoomScale(bool bSelX) const 
	{
		if(bSelX==true)
			return m_scale_x;
		return m_scale_y;
	}

    /// @name Coordinate transformation
    //@{
    /// scaled --> actual.
    void Scaled_to_Actual (POINT& pt) const
    {
        if (m_scale_x > 0)
        {
            pt.x /= m_scale_x ;
            //pt.y /= m_scale ;
        }
        else
		{   
			if (m_scale_x < 0)
            {
                pt.x *= -m_scale_x ;
                //pt.y *= -m_scale ;
            }
		}
		if (m_scale_y > 0)
		{
			//pt.x /= m_scale ;
			pt.y /= m_scale_y ;
		}
		else
		{
			if (m_scale_y < 0)
			{
				//pt.x *= -m_scale ;
				pt.y *= -m_scale_y ;
			}
		}
    }

    /// actual --> scaled.
    void Actual_to_Scaled (POINT& pt) const
    {
        if (m_scale_x > 0)
        {
            pt.x *= m_scale_x ;
            //pt.y *= m_scale ;
        }
        else
		{
			if (m_scale_x < 0)
            {
                pt.x /= -m_scale_x ;
                //pt.y /= -m_scale ;
            }
		}

		if (m_scale_y > 0)
		{
			//pt.x *= m_scale ;
			pt.y *= m_scale_y ;
		}
		else
		{
			if (m_scale_y < 0)
			{
				//pt.x /= -m_scale ;
				pt.y /= -m_scale_y ;
			}
		}
    }
    //@}
};
