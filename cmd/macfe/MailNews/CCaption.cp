/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1997 Netscape Communications Corporation.  All Rights
 * Reserved.
 */



// CCaption.cp

#include "CCaption.h"
#include <LStream.h>
#include "PascalString.h"
#include <TextUtils.h>
#include <UGraphicsUtilities.h>

///////////////////////////////////////////////////////////////////
// constants
const short booleanStringResID = 901;	// resource ID of the boolean strings in macfe.r

///////////////////////////////////////////////////////////////////
// CCaption

void CCaption::DrawSelf()
{
	Rect	frame;
	CalcLocalFrameRect(frame);
	
	StColorPenState	theColorPenState;
	StColorPenState::Normalize ();
	StTextState		theTextState;

	Int16	just = UTextTraits::SetPortTextTraits(mTxtrID);
	
	// ? Save off the text color as setup by the TextTrait
	RGBColor	textColor;
	::GetForeColor(&textColor);
	ApplyForeAndBackColors();
	::RGBForeColor(&textColor);
	
	// the following code adapted from LGARadioButton.cp
	
	// ? Loop over any devices we might be spanning and handle the drawing
	// appropriately for each devices screen depth
	StDeviceLoop	theLoop ( frame );
	Int16			depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		if ( depth < 4 )		//	? BLACK & WHITE
		{
			if ( !IsEnabled() )
			{
				// ? If the caption is dimmed then we use the grayishTextOr 
				// transfer mode to draw the text
				::TextMode ( grayishTextOr );
			}
		}
		else if ( depth >= 4 ) 	// ? COLOR
		{
			if ( !IsEnabled() )
			{
				// ? If the control is dimmed then we have to do our own version of the
				// grayishTextOr as it does not appear to work correctly across
				// multiple devices
				RGBColor textColor2 = UGraphicsUtilities::Lighten( &textColor );
				::TextMode ( srcOr );
				::RGBForeColor ( &textColor2 );
			}
		}	

		UTextDrawing::DrawWithJustification((Ptr)&mText[1], mText[0], frame, just);
	}
}

void CCaption::EnableSelf()
{
	Draw( nil );
}

void CCaption::DisableSelf()
{
	Draw( nil );
}

///////////////////////////////////////////////////////////////////
// CListenerCaption

// Default constructor
CListenerCaption::CListenerCaption( LStream *inStream ) : labelNum( default_menu_item ),
	CCaption ( inStream )
{
}

// Default destructor
CListenerCaption::~CListenerCaption()
{
}

// Change label
void
CListenerCaption::ChangeText( const LabelNum& newLabelNum )
{
	Str255	string;
	::GetIndString( string, resourceID, newLabelNum );
	// needs check and exception		 
	SetDescriptor( string );
	labelNum = newLabelNum;
}

// Return the label num
LabelNum
CListenerCaption::GetLabelNum() const
{
	return labelNum;
}

// Return the label num
void
CListenerCaption::SetLabelNum( const LabelNum& newLabelNum ) 
{
	labelNum = newLabelNum;
}

// Override of the ListenToMessage method
//
// *** Needs exceptions
//
void	
CListenerCaption::ListenToMessage( MessageT  inMessage, void *ioParam)
{
	if( mMsg_changeText == inMessage )
	{
			LabelNum menuItem = *( static_cast< LabelNum* >( ioParam ) );
			ChangeText( menuItem );		
	}
}

// Needs to be called before using this class
// 
// *** Needs exceptions
//
void
CListenerCaption::Init( const short strResID, const MessageT& getNew_msg )
{
	if( getNew_msg )
		 mMsg_changeText = getNew_msg;
		 	
	if( strResID )
		resourceID = strResID;
}
