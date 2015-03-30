/*
 * Copyright 2015, Gerasim Troeglazov, 3dEyes@gmail.com. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "SVGLoader.h"
#include "BaseTranslator.h"

#define NANOSVG_IMPLEMENTATION
#define NANOSVG_ALL_COLOR_KEYWORDS
#include "nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvgrast.h"

SVGLoader::SVGLoader(BPositionIO *source)
{
	fParsed = false;
	fImage = NULL;
	fRasterizer = NULL;

	source->Seek(0, SEEK_END);
	fSVGContentSize = source->Position();
	source->Seek(0, SEEK_SET);
	fSVGContent = new char[fSVGContentSize + 1];

	source->Read(fSVGContent, fSVGContentSize);
	fSVGContent[fSVGContentSize] = 0;

	fImage = nsvgParse(fSVGContent, "px", 96.0f);
	if (fImage == NULL)
		return;

	fParsed = true;
}


SVGLoader::~SVGLoader()
{
	if(fImage != NULL)
		nsvgDelete(fImage);
	delete[] fSVGContent;
}

bool
SVGLoader::IsParsed(void)
{
	return fParsed;
}

int
SVGLoader::RenderImage(BPositionIO *target, float scale)
{
	if (!fParsed)
		return B_NO_TRANSLATOR;

	int width = fImage->width * scale;
	int height = fImage->height *scale;

	fRasterizer = nsvgCreateRasterizer();
	if (fRasterizer == NULL)
		return 	B_NO_TRANSLATOR;
	
	unsigned char* img = new unsigned char[width * height * 4];

	nsvgRasterize(fRasterizer, fImage, 0, 0, scale, img, width, height, width*4);

	TranslatorBitmap bitsHeader;
	bitsHeader.magic = B_TRANSLATOR_BITMAP;
	bitsHeader.bounds.left = 0;
	bitsHeader.bounds.top = 0;
	bitsHeader.bounds.right = width - 1;
	bitsHeader.bounds.bottom = height - 1;
	bitsHeader.rowBytes = sizeof(uint32) * width;
	bitsHeader.colors = B_RGBA32;
	bitsHeader.dataSize = bitsHeader.rowBytes * height;
	if (swap_data(B_UINT32_TYPE, &bitsHeader,
		sizeof(TranslatorBitmap), B_SWAP_HOST_TO_BENDIAN) != B_OK) {		
		return B_NO_TRANSLATOR;
	}
	target->Write(&bitsHeader, sizeof(TranslatorBitmap));

	int pixels = width * height;
	
	uint8 *ptr = img;
	for (int i=0; i<pixels; i++, ptr+=4) {
		uint8 rgba[4];
		rgba[2] = ptr[0];
		rgba[1] = ptr[1];
		rgba[0] = ptr[2];
		rgba[3] = ptr[3];
		target->Write(rgba, 4);
	}
	
	nsvgDeleteRasterizer(fRasterizer);
	delete[] img;

	return B_OK;
}
