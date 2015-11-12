/*
 * Copyright 2015, Gerasim Troeglazov, 3dEyes@gmail.com. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "ConfigView.h"
#include "SVGTranslator.h"

#include <StringView.h>
#include <SpaceLayoutItem.h>
#include <ControlLook.h>
#include <LayoutBuilder.h>

#include <stdio.h>

#define SVG_SETTING_SCALE_CHANGED 'scch'

ConfigView::ConfigView(TranslatorSettings *settings)
	: BGroupView("SVGTranslator Settings", B_VERTICAL)
{
	fSettings = settings;

	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BAlignment leftAlignment(B_ALIGN_LEFT, B_ALIGN_VERTICAL_UNSET);

	BStringView *fTitle = new BStringView("title", "NanoSVG image translator");
	fTitle->SetFont(be_bold_font);

	char version[256];
	sprintf(version, "Version %d.%d.%d, %s",
		int(B_TRANSLATION_MAJOR_VERSION(SVG_TRANSLATOR_VERSION)),
		int(B_TRANSLATION_MINOR_VERSION(SVG_TRANSLATOR_VERSION)),
		int(B_TRANSLATION_REVISION_VERSION(SVG_TRANSLATOR_VERSION)),
		__DATE__);
	BStringView *fVersion = new BStringView("version", version);

	BStringView *fCopyright = new BStringView("copyright",
		B_UTF8_COPYRIGHT "2013-2015 Gerasim Troeglazov");

	int32 scale = fSettings->SetGetInt32(SVG_SETTING_SCALE);
	BString label = "Scale:";
	label << " " << scale / 10.0;

	fScaleSlider = new BSlider("scale", label.String(),
		NULL, 1, 100, B_HORIZONTAL);
	fScaleSlider->SetValue(scale);
	fScaleSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	fScaleSlider->SetHashMarkCount(100);
	fScaleSlider->SetModificationMessage(
		new BMessage(SVG_SETTING_SCALE_CHANGED));
	fScaleSlider->SetExplicitAlignment(leftAlignment);
	
	BString copyrightText;
	copyrightText << "NanoSVG - Simple stupid SVG parser\n";
	copyrightText << "https://github.com/memononen/nanosvg\n\n";
	copyrightText << "(c) 2013-15 Mikko Mononen memon@inside.org\n";	
		
	fCopyrightView = new BTextView("CopyrightLibs");
	fCopyrightView->SetExplicitAlignment(leftAlignment);
	fCopyrightView->MakeEditable(false);
	fCopyrightView->MakeResizable(true);
	fCopyrightView->SetWordWrap(true);
	fCopyrightView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fCopyrightView->SetText(copyrightText.String());

	BFont font;
	font.SetSize(font.Size() * 0.95);
	fCopyrightView->SetFontAndColor(&font, B_FONT_SIZE, NULL);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(fTitle)
		.Add(fVersion)
		.Add(fCopyright)
		.AddGlue()
		.AddStrut(B_USE_SMALL_SPACING)
		.Add(fScaleSlider)
		.AddStrut(B_USE_SMALL_SPACING)
		.Add(fCopyrightView)
		.AddStrut(B_USE_SMALL_SPACING)
		.AddGlue()
		.End();
}


ConfigView::~ConfigView()
{
	fSettings->Release();
}


void
ConfigView::AllAttached()
{
	fScaleSlider->SetTarget(this);
}


void
ConfigView::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case SVG_SETTING_SCALE_CHANGED:
		{
			int32 value = fScaleSlider->Value();			
			if (value <= 0 || value > 100.0)
				break;

			fSettings->SetGetInt32(SVG_SETTING_SCALE, &value);
			fSettings->SaveSettings();

			BString newLabel = "Scale:";
			newLabel << " " << value / 10.0;
			fScaleSlider->SetLabel(newLabel.String());
			return;
		}
		default:
			BView::MessageReceived(message);
	}
}
