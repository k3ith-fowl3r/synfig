/* === S Y N F I G ========================================================= */
/*!	\file valuenode_composite.cpp
**	\brief Template File
**
**	$Id: valuenode_composite.cpp,v 1.1.1.1 2005/01/04 01:23:15 darco Exp $
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

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "valuenode_composite.h"
#include "valuenode_const.h"
#include <stdexcept>
#include "general.h"
#include "valuenode_radialcomposite.h"
#include "vector.h"
#include "color.h"
#include "segment.h"

#endif

/* === U S I N G =========================================================== */

using namespace std;
using namespace etl;
using namespace synfig;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

synfig::ValueNode_Composite::ValueNode_Composite(const ValueBase &value):
	LinkableValueNode(value.get_type())
{
	switch(get_type())
	{
		case ValueBase::TYPE_VECTOR:
			set_link("x",ValueNode_Const::create(value.get(Vector())[0]));
			set_link("y",ValueNode_Const::create(value.get(Vector())[1]));
			break;
		case ValueBase::TYPE_COLOR:
			set_link("r",ValueNode_Const::create(value.get(Color()).get_r()));
			set_link("g",ValueNode_Const::create(value.get(Color()).get_g()));
			set_link("b",ValueNode_Const::create(value.get(Color()).get_b()));
			set_link("a",ValueNode_Const::create(value.get(Color()).get_a()));
			break;
		case ValueBase::TYPE_SEGMENT:
			set_link("p1",ValueNode_Const::create(value.get(Segment()).p1));
			set_link("t1",ValueNode_Const::create(value.get(Segment()).t1));
			set_link("p2",ValueNode_Const::create(value.get(Segment()).p2));
			set_link("t2",ValueNode_Const::create(value.get(Segment()).t2));
			break;
		case ValueBase::TYPE_BLINEPOINT:
		{
			BLinePoint bline_point(value);
			set_link(0,ValueNode_Const::create(bline_point.get_vertex()));
			set_link(1,ValueNode_Const::create(bline_point.get_width()));
			set_link(2,ValueNode_Const::create(bline_point.get_origin()));
			set_link(3,ValueNode_Const::create(bline_point.get_split_tangent_flag()));
			set_link(4,ValueNode_RadialComposite::create(bline_point.get_tangent1()));
			set_link(5,ValueNode_RadialComposite::create(bline_point.get_tangent2()));
			break;
		}
		default:
			assert(0);
			throw Exception::BadType(ValueBase::type_name(get_type()));			
	}
}

ValueNode_Composite::~ValueNode_Composite()
{
	unlink_all();
}

ValueNode_Composite*
ValueNode_Composite::create(const ValueBase &value)
{
	return new ValueNode_Composite(value);
}
	
LinkableValueNode*
ValueNode_Composite::create_new()const
{
	return new ValueNode_Composite(ValueBase(get_type()));
}

ValueBase
synfig::ValueNode_Composite::operator()(Time t)const
{
	switch(get_type())
	{
		case ValueBase::TYPE_VECTOR:
		{
			Vector vect;
			assert(components[0] && components[1]);
			vect[0]=(*components[0])(t).get(Vector::value_type());
			vect[1]=(*components[1])(t).get(Vector::value_type());
			return vect;
		}
		case ValueBase::TYPE_COLOR:
		{
			Color color;
			assert(components[0] && components[1] && components[2] && components[3]);
			color.set_r((*components[0])(t).get(Vector::value_type()));
			color.set_g((*components[1])(t).get(Vector::value_type()));
			color.set_b((*components[2])(t).get(Vector::value_type()));
			color.set_a((*components[3])(t).get(Vector::value_type()));
			return color;
		}
		case ValueBase::TYPE_SEGMENT:
		{
			Segment seg;
			assert(components[0] && components[1] && components[2] && components[3]);
			seg.p1=(*components[0])(t).get(Point());
			seg.t1=(*components[1])(t).get(Vector());
			seg.p2=(*components[2])(t).get(Point());
			seg.t2=(*components[3])(t).get(Vector());
			return seg;
		}
		case ValueBase::TYPE_BLINEPOINT:
		{
			BLinePoint ret;
			assert(components[0] && components[1] && components[2] && components[3] && components[4] && components[5]);
			ret.set_vertex((*components[0])(t).get(Point()));
			ret.set_width((*components[1])(t).get(Real()));
			ret.set_origin((*components[2])(t).get(Real()));
			ret.set_split_tangent_flag((*components[3])(t).get(bool()));
			ret.set_tangent1((*components[4])(t).get(Vector()));
			if(ret.get_split_tangent_flag())
				ret.set_tangent2((*components[5])(t).get(Vector()));
			return ret;
		}
		default:
			synfig::error(string("ValueNode_Composite::operator():")+_("Bad type for composite"));
			assert(components[0]);
			return (*components[0])(t);
	}
}

int
ValueNode_Composite::link_count()const
{
	switch(get_type())
	{
	case ValueBase::TYPE_VECTOR:
		return 2;
	case ValueBase::TYPE_COLOR:
		return 4;
	case ValueBase::TYPE_SEGMENT:
		return 4;
	case ValueBase::TYPE_BLINEPOINT:
		return 6;
	default:
		synfig::warning(string("ValueNode_Composite::component_count():")+_("Bad type for composite"));
		return 1;
	}
}

bool
ValueNode_Composite::set_link_vfunc(int i,ValueNode::Handle x)
{
	assert(i>=0);
	assert(i<6);
	
	if(PlaceholderValueNode::Handle::cast_dynamic(x))
	{
		components[i]=x;
		return true;
	}

	switch(get_type())
	{
		case ValueBase::TYPE_VECTOR:
			assert(i<2);
			if(x->get_type()==ValueBase(Real()).get_type() || PlaceholderValueNode::Handle::cast_dynamic(x))
			{
				components[i]=x;
				return true;
			}
			break;

		case ValueBase::TYPE_COLOR:
			assert(i<4);
			if(x->get_type()==ValueBase(Real()).get_type() || PlaceholderValueNode::Handle::cast_dynamic(x))
			{
				components[i]=x;
				return true;
			}
			break;
			
		case ValueBase::TYPE_SEGMENT:
			assert(i<4);
			if(x->get_type()==ValueBase(Point()).get_type() || PlaceholderValueNode::Handle::cast_dynamic(x))
			{
				components[i]=x;
				return true;
			}
			break;

		case ValueBase::TYPE_BLINEPOINT:
			assert(i<6);
			if((i==0 || i==4 || i==5) && x->get_type()==ValueBase(Point()).get_type())
			{
				components[i]=x;
				return true;
			}
			if((i==1 || i==2) && x->get_type()==ValueBase(Real()).get_type())
			{
				components[i]=x;
				return true;
			}
			if(i==3 && x->get_type()==ValueBase(bool()).get_type())
			{
				components[i]=x;
				return true;
			}
			break;
			
		default:
			break;
	}
	return false;	
}

ValueNode::LooseHandle
ValueNode_Composite::get_link_vfunc(int i)const
{
	assert(i>=0 && i<6);
	return components[i];
}

String
ValueNode_Composite::link_local_name(int i)const
{
	assert(i>=0 && i<6);
	switch(get_type())
	{
		case ValueBase::TYPE_VECTOR:
			return strprintf("%c-Axis",'X'+i);

		case ValueBase::TYPE_COLOR:
			if(i==0)
				return _("Red");
			else if(i==1)
				return _("Green");
			else if(i==2)
				return _("Blue");
			else if(i==3)
				return _("Alpha");
			
		case ValueBase::TYPE_SEGMENT:
			if(i==0)
				return _("Vertex 1");
			else if(i==1)
				return _("Tangent 1");
			else if(i==2)
				return _("Vertex 2");
			else if(i==3)
				return _("Tangent 2");

		case ValueBase::TYPE_BLINEPOINT:
			if(i==0)
				return _("Vertex");
			else if(i==1)
				return _("Width");
			else if(i==2)
				return _("Origin");
			else if(i==3)
				return _("Split Tangents");
			else if(i==4)
				return _("Tangent 1");
			else if(i==5)
				return _("Tangent 2");

		default:
			break;
	}
	return etl::strprintf(_("C%d"),i+1);	
}	


String
ValueNode_Composite::link_name(int i)const
{
	assert(i>=0 && i<5);
	return strprintf("c%d",i);
}	

int
ValueNode_Composite::get_link_index_from_name(const String &name)const
{
	if(name.empty())
		throw Exception::BadLinkName(name);

	if(name[0]=='c')
		return name[1]-'0';

	switch(get_type())
	{
	case ValueBase::TYPE_COLOR:
		if(name[0]=='r')
			return 0;
		if(name[0]=='g')
			return 1;
		if(name[0]=='b')
			return 2;
		if(name[0]=='a')
			return 3;
	case ValueBase::TYPE_SEGMENT:
		if(name=="p1")
			return 0;
		if(name=="t1")
			return 1;
		if(name=="p2")
			return 2;
		if(name=="t2")
			return 3;
	case ValueBase::TYPE_VECTOR:
		if(name[0]=='x')
			return 0;
		if(name[0]=='y')
			return 1;
		if(name[0]=='z')
			return 2;
	case ValueBase::TYPE_BLINEPOINT:
		if(name[0]=='p' || name=="v1" || name=="p1")
			return 0;
		if(name=="w" || name=="width")
			return 1;
		if(name=="o" || name=="origin")
			return 2;
		if(name=="split")
			return 3;
		if(name=="t1")
			return 4;
		if(name=="t2")
			return 5;
	default:			
		break;
	}

	throw Exception::BadLinkName(name);
}

String
ValueNode_Composite::get_name()const
{
	return "composite";
}

String
ValueNode_Composite::get_local_name()const
{
	return _("Composite");
}

bool
ValueNode_Composite::check_type(ValueBase::Type type)
{
	return
		type==ValueBase::TYPE_SEGMENT ||
		type==ValueBase::TYPE_VECTOR ||
		type==ValueBase::TYPE_COLOR ||
		type==ValueBase::TYPE_BLINEPOINT;
}
