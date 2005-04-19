/* === S Y N F I G ========================================================= */
/*!	\file circle.h
**	\brief Template Header
**
**	$Id: circle.h,v 1.2 2005/01/24 03:08:17 darco Exp $
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

/* === H E A D E R S ======================================================= */

#ifndef __SYNFIG_LAYER_CIRCLE_H__
#define __SYNFIG_LAYER_CIRCLE_H__

/* -- H E A D E R S --------------------------------------------------------- */

#include <synfig/layer_composite.h>
#include <synfig/color.h>
#include <synfig/vector.h>

using namespace synfig;
using namespace std;
using namespace etl;

/* -- M A C R O S ----------------------------------------------------------- */

/* -- T Y P E D E F S ------------------------------------------------------- */

/* -- S T R U C T S & C L A S S E S ----------------------------------------- */

class Circle : public synfig::Layer_Composite, public synfig::Layer_NoDeform
{
	SYNFIG_LAYER_MODULE_EXT
private:
	synfig::Color color;
	synfig::Point pos;
	synfig::Real radius;
	synfig::Real feather;
	bool invert;
	int falloff;

	//Caching system for circle
	struct CircleDataCache
	{
		Real inner_radius;
		Real outer_radius;
	
		Real inner_radius_sqd;
		Real outer_radius_sqd;
	
		Real diff_sqd;
		Real double_feather;
	};
	
	typedef	Real	FALLOFF_FUNC(const CircleDataCache &c, const Real &mag_sqd);
	
	FALLOFF_FUNC	*falloff_func;
	CircleDataCache	cache;

	void constructcache();

	static	Real	SqdFalloff(const CircleDataCache &c, const Real &mag_sqd);
	static	Real	InvSqdFalloff(const CircleDataCache &c, const Real &mag_sqd);			
	static	Real	SqrtFalloff(const CircleDataCache &c, const Real &mag_sqd);
	static	Real	InvSqrtFalloff(const CircleDataCache &c, const Real &mag_sqd);		
	static	Real	LinearFalloff(const CircleDataCache &c, const Real &mag_sqd);
	static	Real	InvLinearFalloff(const CircleDataCache &c, const Real &mag_sqd);		
	static	Real	SigmondFalloff(const CircleDataCache &c, const Real &mag_sqd);
	static	Real	InvSigmondFalloff(const CircleDataCache &c, const Real &mag_sqd);
	static	Real	CosineFalloff(const CircleDataCache &c, const Real &mag_sqd);
	static	Real	InvCosineFalloff(const CircleDataCache &c, const Real &mag_sqd);		
	
	FALLOFF_FUNC	*GetFalloffFunc()const;
	bool ImportParameters(const String &param, const ValueBase &value);

public:
	enum Falloff
	{
		FALLOFF_SQUARED		=0,
		FALLOFF_INTERPOLATION_LINEAR		=1,
		FALLOFF_SMOOTH		=2,
		FALLOFF_COSINE		=2,
		FALLOFF_SIGMOND		=3,
		FALLOFF_SQRT		=4
	};
	
	Circle();
	
	virtual bool set_param(const String &param, const ValueBase &value);
	
	virtual ValueBase get_param(const String &param)const;
	
	virtual Color get_color(Context context, const Point &pos)const;
	
	virtual bool accelerated_render(Context context,Surface *surface,int quality, const RendDesc &renddesc, ProgressCallback *cb)const;

	virtual synfig::Rect get_full_bounding_rect(synfig::Context context)const;
	virtual synfig::Rect get_bounding_rect()const;

	synfig::Layer::Handle hit_check(synfig::Context context, const synfig::Point &point)const;	
	
	virtual Vocab get_param_vocab()const;
};

/* -- E X T E R N S --------------------------------------------------------- */


/* -- E N D ----------------------------------------------------------------- */

#endif
