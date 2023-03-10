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
#pragma once
#include "CPrefsMediator.h"
class CMimeTableView;
//------------------------------------------------------------------------------
//	?	CBrowserApplicationsMediator
//------------------------------------------------------------------------------
//
#pragma mark
class CBrowserApplicationsMediator : public CPrefsMediator
{
	public:
		static	void	RegisterViewClasses();
		enum { class_ID = PrefPaneID::eBrowser_Applications };
		CBrowserApplicationsMediator(LStream*);
		virtual	~CBrowserApplicationsMediator() {};

		virtual void	ListenToMessage(MessageT inMessage, void *ioParam);

		virtual	void	LoadMainPane();
		virtual	void	WritePrefs();
		virtual	void	UpdateFromIC();
	private:
		Boolean			mModified;			// Have any MIMEs been modified
		CMimeTableView*	mMIMETable;			// Scrolling table of MIME types
		LArray			mDeletedTypes;

				void	EditMimeEntry( Boolean isNewEntry);
				void	DeleteMimeEntry();
};