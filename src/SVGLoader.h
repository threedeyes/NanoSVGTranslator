/*
 * Copyright 2015, Gerasim Troeglazov, 3dEyes@gmail.com. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#ifndef PDF_LOADER_H
#define PDF_LOADER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Translator.h>
#include <TranslatorFormats.h>
#include <TranslationDefs.h>
#include <GraphicsDefs.h>
#include <InterfaceDefs.h>
#include <DataIO.h>
#include <File.h>
#include <ByteOrder.h>
#include <List.h>

#include "SVGTranslator.h"

#include <stdio.h>
#include <string.h>
#include <float.h>


class SVGLoader {
public:
					SVGLoader(BPositionIO *source);
					~SVGLoader();
		
	int				PageCount(void);
	int 			RenderImage(BPositionIO *target, float scale);
	bool			IsParsed(void);
private:
	bool			fParsed;
	float 			fScale;
	char			*fSVGContent;
	size_t			fSVGContentSize;
	struct NSVGimage *fImage;
	struct NSVGrasterizer *fRasterizer;
};


#endif	/* PDF_LOADER_H */
