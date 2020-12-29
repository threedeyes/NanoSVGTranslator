/*
 * Copyright 2015-2020, Gerasim Troeglazov, 3dEyes@gmail.com. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ConfigView.h"
#include "SVGTranslator.h"
#include "SVGLoader.h"

#define kSVGMimeType "image/svg+xml"
#define kSVGName "SVG Image"

static const translation_format sInputFormats[] = {
	{
		SVG_IMAGE_FORMAT,
		B_TRANSLATOR_BITMAP,
		SVG_IN_QUALITY,
		SVG_IN_CAPABILITY,
		kSVGMimeType,
		kSVGName
	},
};

static const translation_format sOutputFormats[] = {
	{
		B_TRANSLATOR_BITMAP,
		B_TRANSLATOR_BITMAP,
		BITS_OUT_QUALITY,
		BITS_OUT_CAPABILITY,
		"image/x-be-bitmap",
		"Be Bitmap Format (NanoSVGTranslator)"
	},
};


static const TranSetting sDefaultSettings[] = {
	{B_TRANSLATOR_EXT_HEADER_ONLY, TRAN_SETTING_BOOL, false},
	{B_TRANSLATOR_EXT_DATA_ONLY, TRAN_SETTING_BOOL, false},
	{SVG_SETTING_SCALE, TRAN_SETTING_INT32, 10}
};

const uint32 kNumInputFormats = sizeof(sInputFormats)
	/ sizeof(translation_format);
const uint32 kNumOutputFormats = sizeof(sOutputFormats)
	/ sizeof(translation_format);
const uint32 kNumDefaultSettings = sizeof(sDefaultSettings)
	/ sizeof(TranSetting);


SVGTranslator::SVGTranslator()
	: BaseTranslator("SVG Images",
		"SVG image translator",
		SVG_TRANSLATOR_VERSION,
		sInputFormats, kNumInputFormats,
		sOutputFormats, kNumOutputFormats,
		"NanoSVGTranslator",
		sDefaultSettings, kNumDefaultSettings,
		B_TRANSLATOR_BITMAP, SVG_IMAGE_FORMAT)
{
}


SVGTranslator::~SVGTranslator()
{
}


status_t
SVGTranslator::DerivedIdentify(BPositionIO *stream,
	const translation_format *format, BMessage *ioExtension,
	translator_info *info, uint32 outType)
{	
	if (outType != B_TRANSLATOR_BITMAP)
		return B_NO_TRANSLATOR;
		
	char svgData[256];
	ssize_t readedBytes = 255;
	readedBytes = stream->Read(svgData, 255);
	svgData[readedBytes] = 0;

	if(strncasecmp(svgData, "<?xml", 5) != 0
		&& strncasecmp(svgData, "<svg", 4) != 0) {
		return B_NO_TRANSLATOR;
	}

	if(strcasestr(svgData, "<svg") == NULL
		&& strcasestr(svgData, "<!DOCTYPE SVG") == NULL) {
		return B_NO_TRANSLATOR;
	}

	info->type = SVG_IMAGE_FORMAT;
	info->group = B_TRANSLATOR_BITMAP;
	info->quality = SVG_IN_QUALITY;
	info->capability = SVG_IN_CAPABILITY;
	strcpy(info->name, kSVGName);
	strcpy(info->MIME, kSVGMimeType);

	return B_OK;
}


status_t
SVGTranslator::DerivedTranslate(BPositionIO *source,
	const translator_info *info, BMessage *ioExtension,
	uint32 outType, BPositionIO *target, int32 baseType)
{
	if (outType != B_TRANSLATOR_BITMAP)
		return B_NO_TRANSLATOR;

	switch (baseType) {
		case 0:
		{								
			if (outType != B_TRANSLATOR_BITMAP)
				return B_NO_TRANSLATOR;

			SVGLoader SVGFile(source);
			if (!SVGFile.IsParsed())
				return B_NO_TRANSLATOR;
			float scale = fSettings->SetGetInt32(SVG_SETTING_SCALE) / 10.0;
			return SVGFile.RenderImage(target, scale);
		}

		default:
			return B_NO_TRANSLATOR;
	}
}


status_t
SVGTranslator::DerivedCanHandleImageSize(float width, float height) const
{
	return B_OK;
}


BView *
SVGTranslator::NewConfigView(TranslatorSettings *settings)
{
	return new ConfigView(settings);
}


BTranslator *
make_nth_translator(int32 n, image_id you, uint32 flags, ...)
{
	if (n != 0)
		return NULL;

	return new SVGTranslator();
}

