/* === S Y N F I G ========================================================= */
/*!	\file layer_shape.h
**	\brief Template Header
**
**	$Id: layer_shape.h,v 1.2 2005/01/24 03:08:18 darco Exp $
**
**	\legal
**	Copyright (c) 2002 Robert B. Quattlebaum Jr.
**
**	This software and associated documentation
**	are CONFIDENTIAL and PROPRIETARY property of
**	the above-mentioned copyright holder.
**
**	You may not copy, print, publish, or in any
**	other way distribute this software without
**	a prior written agreement with
**	the copyright holder.
**	\endlegal
*/
/* ========================================================================= */

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_LAYER_SHAPE_H
#define __SYNFIG_LAYER_SHAPE_H

/* === H E A D E R S ======================================================= */

#include "layer_composite.h"
#include "color.h"
#include "vector.h"
#include "blur.h"

#include <vector>

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig {
	
/*!	\class Layer_Shape
**	\beief writeme			*/
class Layer_Shape : public Layer_Composite, public Layer_NoDeform
{
	SYNFIG_LAYER_MODULE_EXT
	
private:
	
	//internal cacheing
	struct Intersector;
	Intersector	*edge_table;
	
	//exported data
	Color 					color;
	
	Point 	offset;
	bool	invert;
	bool	antialias;
	
	int		blurtype;
	Real	feather;
	
	std::vector< char > 	bytestream;
	
	//for use in creating the bytestream
	int						lastbyteop;
	int						lastoppos;

protected:

	Layer_Shape(const Real &a = 1.0, const Color::BlendMethod m = Color::BLEND_COMPOSITE);

public:

	~Layer_Shape();

	//! Clears out any data
	/*!	Also clears out the Intersector
	*/
	void clear();
	//void sync();

	void move_to(Real x, Real y);
	void line_to(Real x, Real y);
	void conic_to(Real x1, Real y1, Real x, Real y);
	void conic_to_smooth(Real x, Real y);				//x1,y1 derived from current tangent
	void curve_to(Real x1, Real y1, Real x2, Real y2, Real x, Real y);
	void curve_to_smooth(Real x2, Real y2, Real x, Real y);	//x1,y1 derived from current tangent
	void close();
	void endpath();

	virtual bool set_param(const String & param, const synfig::ValueBase &value);
	virtual ValueBase get_param(const String & param)const;
	
	virtual Vocab get_param_vocab()const;

	virtual Color get_color(Context context, const Point &pos)const;
	virtual bool accelerated_render(Context context,Surface *surface,int quality, const RendDesc &renddesc, ProgressCallback *cb)const;
	virtual synfig::Layer::Handle hit_check(synfig::Context context, const synfig::Point &point)const;
	virtual Rect get_bounding_rect()const;

private:
	class 		PolySpan;
	bool render_polyspan(Surface *surface,PolySpan &polyspan,
						Color::BlendMethod method,Color::value_type amount)const;
	bool render_polyspan(etl::surface<float> *surface,PolySpan &polyspan)const;
	virtual bool render_shape(Surface *surface,bool useblend,int quality,const RendDesc &renddesc, ProgressCallback *cb)const;
	virtual bool render_shape(etl::surface<float> *surface,int quality,const RendDesc &renddesc, ProgressCallback *cb)const;
}; // END of Layer_Shape

}; // END of namespace synfig
/* === E N D =============================================================== */

#endif
