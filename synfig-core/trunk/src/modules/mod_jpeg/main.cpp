/*! ========================================================================
** Synfig
** bleh
** $Id: main.cpp,v 1.1.1.1 2005/01/04 01:23:11 darco Exp $
**
** Copyright (c) 2002 Robert B. Quattlebaum Jr.
**
** This software and associated documentation
** are CONFIDENTIAL and PROPRIETARY property of
** the above-mentioned copyright holder.
**
** You may not copy, print, publish, or in any
** other way distribute this software without
** a prior written agreement with
** the copyright holder.
**
** === N O T E S ===========================================================
**
** ========================================================================= */

/* === H E A D E R S ======================================================= */

#define SYNFIG_MODULE

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <synfig/module.h>
#include "trgt_jpeg.h"
#include "mptr_jpeg.h"
#endif

/* === E N T R Y P O I N T ================================================= */

MODULE_DESC_BEGIN(mod_jpeg)
	MODULE_NAME("JPEG Module (libjpeg)")
	MODULE_DESCRIPTION("Provides a JPEG target and importer")
	MODULE_AUTHOR("Robert B. Quattlebaum Jr")
	MODULE_VERSION("1.0")
	MODULE_COPYRIGHT(SYNFIG_COPYRIGHT)
MODULE_DESC_END

MODULE_INVENTORY_BEGIN(mod_jpeg)
	BEGIN_TARGETS
		TARGET(jpeg_trgt)
		TARGET_EXT(jpeg_trgt,"jpeg")
		TARGET_EXT(jpeg_trgt,"jpg")
	END_TARGETS
	BEGIN_IMPORTERS
		IMPORTER_EXT(jpeg_mptr,"jpg")
		IMPORTER_EXT(jpeg_mptr,"jpeg")
	END_IMPORTERS
MODULE_INVENTORY_END
