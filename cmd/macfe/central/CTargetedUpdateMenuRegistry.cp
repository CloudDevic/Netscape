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

//	CTargetedUpdateMenuRegistry.cp

//
//	CTargetedUpdateMenuRegistry is used in conjunction with LEventDispatchers
//	which are CTargetedUpdateMenuRegistry-aware to enable a more targeted
//	update of menus.
//
//	One common example of this is menu items whose text should change when
//	certain modifiers are held down (Close -> Close All when optionKey is
//	held down, for example).
//
//	Usage notes for callers of UpdateMenus:
//
//		(1) Call SetCommands with a list of commands which should be updated.
//		(2) Call UpdateMenus.
//
//	Usage notes for implementors of UpdateMenus:
//
//		(1) When looping through commands in menus, determine if the registry
//			is active by calling UseRegistryToUpdateMenus.
//		(2) If the registry is active, then check to see if the command is
//			in the registry by calling CommandInRegistry before processing
//			command status for the command.
//
//	Note: For the targeted update to be useful, there should generally be
//	a very small number of targeted commands. In fact, command lookup will
//	slow down if there are too many commands.

#include "CTargetedUpdateMenuRegistry.h"

// === Static Members ===

Boolean			CTargetedUpdateMenuRegistry::sUseRegistryToUpdateMenus = false;
list<CommandT>	CTargetedUpdateMenuRegistry::sCommands;

// ---------------------------------------------------------------------------
//		? SetCommands
// ---------------------------------------------------------------------------

void
CTargetedUpdateMenuRegistry::SetCommands(
	const list<CommandT>& inCommands)
{
	sCommands = inCommands;
}	

// ---------------------------------------------------------------------------
//		? UpdateMenus
// ---------------------------------------------------------------------------

void
CTargetedUpdateMenuRegistry::UpdateMenus()
{
	if (LEventDispatcher::GetCurrentEventDispatcher())
	{
		StValueChanger<Boolean> setUseRegistryToUpdateMenus(sUseRegistryToUpdateMenus, true);
	
		LEventDispatcher::GetCurrentEventDispatcher()->UpdateMenus();
	}
}

// ---------------------------------------------------------------------------
//		? CommandInRegistry
// ---------------------------------------------------------------------------

Boolean
CTargetedUpdateMenuRegistry::CommandInRegistry(CommandT inCommand)
{
	list<CommandT>::const_iterator theCommand = find(
														sCommands.begin(),
														sCommands.end(),
														inCommand);
	return (theCommand != sCommands.end());
}
