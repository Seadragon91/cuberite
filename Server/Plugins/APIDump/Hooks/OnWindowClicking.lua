return
{
	HOOK_WINDOW_CLICKING =
	{
		CalledWhen = "Called when a player is clicking in a window",
		DefaultFnName = "OnWindowClicking",  -- also used as pagename
		Desc = [[
			This hook is called when a player is clicking in a lua window.
		]],
		Params =
		{
			{ Name = "Player", Type = "{{cPlayer}}", Notes = "The player who is clicking in the window" },
			{ Name = "LuaWindow", Type = "{{cLuaWindow}}", Notes = "The lua window in which the player is clicking" },
			{ Name = "ClickedItem", Type = "{{cItem}}", Notes = "The clicked item" },
			{ Name = "ClickAction", Type = "{{Globals#eClickAction|eClickAction}}", Notes = "The click action that the player did" },
			{ Name = "SlotNum", Type = "number", Notes = "The number of the slot that the player is clicking on. Note: Zero-based" },
		},
		Returns = [[
			If the function returns false or no value, the next plugin's callback is called, and finally
			Cuberite will process the click. If the function returns true, no other callback is called for this event.
			The content and the item that is been dragged, will be send to the player. It's possible to change
			the dragging item with {{cPlayer#SetDraggingItem_1|cPlayer#SetDraggingItem}}.
		]],
	},  -- HOOK_WINDOW_CLICKING
}
