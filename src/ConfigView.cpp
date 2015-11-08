/*
 * Copyright 2015, Gerasim Troeglazov, 3dEyes@gmail.com. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "ConfigView.h"
#include "SVGTranslator.h"

#include <StringView.h>
#include <SpaceLayoutItem.h>
#include <ControlLook.h>

#include <stdio.h>

#define SVG_SETTING_SCALE_CHANGED 'scch'

ConfigView::ConfigView(TranslatorSettings *settings)
	: BGroupView("SVGTranslator Settings", B_VERTICAL, 0)
{
	fSettings = settings;

	BAlignment leftAlignment(B_ALIGN_LEFT, B_ALIGN_VERTICAL_UNSET);

	BStringView *stringView = new BStringView("title", "NanoSVG image translator");
	stringView->SetFont(be_bold_font);
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);

	float spacing = be_control_look->DefaultItemSpacing();
	AddChild(BSpaceLayoutItem::CreateVerticalStrut(spacing));

	char version[256];
	sprintf(version, "Version %d.%d.%d, %s",
		int(B_TRANSLATION_MAJOR_VERSION(SVG_TRANSLATOR_VERSION)),
		int(B_TRANSLATION_MINOR_VERSION(SVG_TRANSLATOR_VERSION)),
		int(B_TRANSLATION_REVISION_VERSION(SVG_TRANSLATOR_VERSION)),
		__DATE__);
	stringView = new BStringView("version", version);
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);

	stringView = new BStringView("copyright",
		B_UTF8_COPYRIGHT "2005-2015 Haiku Inc.");
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);

	stringView = new BStringView("my_copyright",
		B_UTF8_COPYRIGHT "2013-2015 Gerasim Troeglazov <3dEyes@gmail.com>");
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);
	
	AddChild(BSpaceLayoutItem::CreateVerticalStrut(spacing));

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
	
	AddChild(fScaleSlider);
	
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
	fCopyrightView->SetExplicitMinSize(BSize(300,200));

	BFont font;
	font.SetSize(font.Size() * 0.9);
	fCopyrightView->SetFontAndColor(&font, B_FONT_SIZE, NULL);
	AddChild(fCopyrightView);

	fCopyrightView->SetExplicitAlignment(leftAlignment);
	
	AddChild(BSpaceLayoutItem::CreateGlue());
	GroupLayout()->SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, 
		B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING);

	SetExplicitPreferredSize(GroupLayout()->MinSize());		
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
