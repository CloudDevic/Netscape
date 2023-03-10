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
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */


#include "prtypes.h"

#include "LCustomizeMenu.h"

//
//	Constructor
//
LCustomizeMenu::LCustomizeMenu()
	: LPatchedMenu()
{
	fItemHeight = 16;	// fix constant, changable
	fItemCount = 0;
	fTextMargin = 14;	// fix constant, changable
	fMarkMargin = 2;	// fix constant, changable
}
//
//	Size
//
void LCustomizeMenu::Size(MenuHandle menu, MenuDefUPP* /* root */, Rect * /* rect */, Point /* hitPt */, short * /* item */)
{
	fItemCount = ::CountMItems(menu);
	int maxWidth = fTextMargin + 4;
	fItemCount = ::CountMItems(menu);
	for(int i = 1; i <= fItemCount; i++)
	{
		short itemWidth = MeasureItem(menu,  i);
		if(itemWidth > maxWidth)
			maxWidth = itemWidth;
	}	
	(**menu).menuHeight = fItemHeight * fItemCount;	
 	(**menu).menuWidth = maxWidth;
}

//
//	MeasureItem
//
short LCustomizeMenu::MeasureItem(MenuHandle menu, int item)
{
	Boolean AddSlop = FALSE;
	short itemWidth = fTextMargin + 4;
	Str255 itemtext;
	::GetMenuItemText(menu, item, itemtext);
	if(itemtext[1] != '-')
	{
		short  iconindex;
		short  cmd;
		short  mark;
		
		// Get the needed Item data
		::GetItemCmd(menu, item, &cmd);
		::GetItemIcon(menu, item, &iconindex);
		::GetItemMark(menu, item, &mark);
		
		itemWidth += MeasureItemText(itemtext);
		if(HaveCommand(cmd))	// add command width
		{
			itemWidth += MeasureItemCommand(cmd);
			AddSlop = TRUE;
		}
		if(HaveSubmenu(cmd))	// add SubmenuIndicator width
		{
			itemWidth += MeasureItemSubmenuIndicator();
			AddSlop = TRUE;
		}
		if(AddSlop)
			itemWidth += MeasureItemSlop();
	}
	return itemWidth;
}
//
//	MeasureItemText
//
short LCustomizeMenu::MeasureItemText(Str255 text)
{
	return ::StringWidth(text);
}
//
//	MeasureItemCommand
//
short LCustomizeMenu::MeasureItemCommand(short command)
{
	::TextFont(systemFont);
	return ::CharWidth(17) + ::CharWidth(command);
}

//
//	Draw
//
void LCustomizeMenu::Draw(MenuHandle menu, MenuDefUPP* /* root */, Rect *rect, Point /* hitPt */, short * /* item */)
{
	Assert_(fItemCount != 0);
	Assert_(fItemHeight != 0);	
	for(int i = 1; i <= fItemCount; i++)
	{
		Rect itemrect;
		GetItemRect(i, rect, itemrect);
		DrawItem(menu, i, itemrect);
	}
}
//
//	DrawItemSeparator
//
void LCustomizeMenu::DrawItemSeperator(Rect& itemrect )
{
	::PenPat(&qd.gray);
	::MoveTo(itemrect.left, itemrect.top + 8);
	::LineTo(itemrect.right, itemrect.top + 8);
	::PenNormal();
}
//
//	DrawItemIcon
//	???Unsupport Yet
//
void LCustomizeMenu::DrawItemIcon(Rect& /* itemrect */, short /* iconindex */)
{
	Assert_(FALSE);
	// We current do not support Icon
}
//
//	DrawItemCommand
//
void LCustomizeMenu::DrawItemCommand(Rect& /* itemrect */, short cmd)
{
	::DrawChar(17);
	::DrawChar(cmd);
}
//
//	DrawItemSubmenuIndicator
//	???Unsupport Yet
//
void LCustomizeMenu::DrawItemSubmenuIndicator(Rect& /* itemrect */)
{
	Assert_(FALSE);
	// We current do not support SubmenuIndicator
}
//
//	InvertItem
//
void LCustomizeMenu::InvertItem(MenuHandle menu, int item, Rect* menurect, Boolean leaveblack)
{
	RgnHandle rgn;
	rgn = ::NewRgn();
	::GetClip(rgn);
	
	Rect itemrect;
	GetItemRect(item, menurect, itemrect);
	
	::EraseRect(&itemrect);
	::ClipRect(&itemrect);
	
	DrawItem( menu, item, itemrect);
	
	::SetClip(rgn);
	::DisposeRgn(rgn);
	if(leaveblack)
		::InvertRect(&itemrect);
}
//
//	DrawItemDisable
//
void LCustomizeMenu::DrawItemDisable(Rect& itemrect)
{
	::PenPat(&qd.gray);
	::PenMode(patBic);
	itemrect.right++;
	itemrect.left++;
	::PaintRect(&itemrect);
	::PenNormal();
}
//
//	GetItemRect
//

void LCustomizeMenu::GetItemRect(int i, Rect* menurect, Rect& itemrect)
{
	if(i > 0)
	{
		SetRect(&itemrect, menurect->left, menurect->top + ((i-1) * fItemHeight) , menurect->right, 0 );
		itemrect.bottom = itemrect.top + fItemHeight;
	}
	else
		SetRect(&itemrect, 0, 0 , 0, 0);
}
//
//	MoveToItemTextPosition
//
void LCustomizeMenu::MoveToItemTextPosition(Rect& itemrect)
{
	FontInfo finfo;
	::GetFontInfo(&finfo);
	::MoveTo(itemrect.left + fTextMargin, itemrect.bottom - finfo.descent);	
}
//
//	MoveToItemCommandPosition
//
void LCustomizeMenu::MoveToItemCommandPosition(Rect& itemrect)
{
	FontInfo finfo;
	::GetFontInfo(&finfo);
	::MoveTo(itemrect.left -24, itemrect.bottom - finfo.descent);	
}
//
//	MoveToItemMarkPosition
//
void LCustomizeMenu::MoveToItemMarkPosition(Rect& itemrect)
{
	FontInfo finfo;
	::GetFontInfo(&finfo);
	::MoveTo(itemrect.left + fMarkMargin, itemrect.bottom - finfo.descent);	
}
//
//	DrawItemText
//
void LCustomizeMenu::DrawItemText(Rect& /* itemrect */, Str255 itemtext )
{
	::DrawString(itemtext);	
}
//
//	DrawItemMark
//
void LCustomizeMenu::DrawItemMark(Rect& /* itemrect */, short mark )
{
	::DrawChar(mark);
}
//
//	DrawItem
//
void LCustomizeMenu::DrawItem(MenuHandle menu, int item, Rect& itemrect )
{
	if((item <=0) || (item > fItemCount))
		return;
	Str255 itemtext;
	::GetMenuItemText(menu, item, itemtext);
	if(itemtext[1] == '-')
	{
		// Draw Seperator
		DrawItemSeperator(itemrect );
	}
	else
	{
		Style style;
		short  iconindex;
		short  cmd;
		short  mark;
		
		// Get the needed Item data
		::GetItemStyle(menu, item, &style);
		::GetItemCmd(menu, item, &cmd);
		::GetItemIcon(menu, item, &iconindex);
		::GetItemMark(menu, item, &mark);

		// Deal with Item Text First
		::TextFace(style);
		MoveToItemTextPosition(itemrect);
		DrawItemText(itemrect, itemtext );		
		::TextFace(normal);

		// Draw Mark if necessary
		if(HaveMark(cmd, mark))
		{
			::TextFont(systemFont);
			MoveToItemMarkPosition(itemrect);
			DrawItemMark(itemrect, mark);
		}
		// Draw Icon if necessary
		if(HaveIcon(cmd, iconindex))
		{
			DrawItemIcon(itemrect, iconindex);
		}
		// Draw Command if necessary
		if(HaveCommand(cmd))
		{
			::TextFont(systemFont);
			MoveToItemCommandPosition(itemrect);
			DrawItemCommand(itemrect, cmd);
		}
		// Draw SubmenuIndicator if necessary
		if(HaveSubmenu(cmd))
		{
			DrawItemSubmenuIndicator(itemrect);
		}
		// Grayout if necessary
		if( ! ItemEnable(menu, item) )
			DrawItemDisable(itemrect);
	}
}
//
//	Choose
//
void LCustomizeMenu::Choose(MenuHandle menu, MenuDefUPP* /* root */, Rect *rect, Point hitPt, short *item)
{
	int olditem = *item;
	*item = 0;
	if( ::PtInRect(hitPt, rect))
	{
		int y = ((hitPt.v - rect->top) / fItemHeight) + 1;
		Rect tmprect;
		GetItemRect(y, rect, tmprect);
		
		*item = y;
		if(*item != olditem)
		{
			if(ItemEnable(menu, *item))
			{
				InvertItem(menu, *item, rect, TRUE);
				InvertItem(menu, olditem, rect, FALSE);		
			}
			else
			{
				InvertItem(menu, olditem, rect, FALSE);
				*item = 0;
			}
		}
	}
	else
	{
		InvertItem(menu, olditem, rect, FALSE);
	}
}

