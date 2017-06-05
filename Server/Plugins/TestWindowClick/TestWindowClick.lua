g_Plugin = nil

function Initialize(a_Plugin)
	a_Plugin:SetName("TestWindowClick")
	a_Plugin:SetVersion(1)
	
	g_Plugin = a_Plugin

	cPluginManager:AddHook(cPluginManager.HOOK_WINDOW_CLICKING, OnWindowClicking)
	
	cPluginManager:BindCommand("/openit", "openit", CommandOpenWindow, "opens a window for testing click events")
	
	return true
end



function CreateCommandGUI()
	local iniCommandWindow = cIniFile()
	iniCommandWindow:ReadFile(g_Plugin:GetLocalFolder() .. cFile:GetPathSeparator() .. "command_window.ini")
	
	-- Create lua window with rows and title
	local title = iniCommandWindow:GetValue("General", "Title")
	local rows = iniCommandWindow:GetValueI("General", "Rows")
	g_CommandWindow = cLuaWindow(cWindow.wtChest, 9, rows, title)
	
	-- Create a info for the slots and the action that should be done
	g_Actions = {}
	
	-- Read slots
	local lastIndex = (rows * 9) - 1
	for i = 0, lastIndex do
		local slot = "Slot:" .. i
		if iniCommandWindow:HasValue(slot, "ItemType") then
			g_Actions[i] = {}
			local item = cItem()
			local itemType = StringToItem(iniCommandWindow:GetValue(slot, "ItemType"), item)
			local itemLore = iniCommandWindow:GetValue(slot, "ItemLore", "")
			local action = ""
			if iniCommandWindow:HasValue(slot, "OpenWindow") then
				g_Actions[i].OpenWindow = iniCommandWindow:GetValueI(slot, "OpenWindow")
			elseif iniCommandWindow:HasValue(slot, "RunCommand") then
				g_Actions[i].RunCommand = iniCommandWindow:GetValue(slot, "RunCommand")
			end
			
			if iniCommandWindow:HasValue(slot, "CloseWindow") then
				g_Actions[i].CloseWindow = iniCommandWindow:GetValueI(slot, "CloseWindow")
			end
			
			item.m_Lore = itemLore
			g_CommandWindow:GetContents():SetSlot(i, item)
		end
	end
end


function OnWindowClicking(a_Player, a_Window, a_ClickedItem, a_ClickAction, a_SlotNum)
	-- Uncoment this for info: Sends to player: Window type, slot number and clicked item type
	-- a_Player:SendMessage(
	-- 	string.format("Window Type: %d; SlotNum: %d; ClickedItem Type: %d",
	-- 	a_Window:GetWindowType(), a_SlotNum, a_ClickedItem.m_ItemType)
	-- )
	
	-- If clicks is not inside the command window bail out
	if a_SlotNum >= (g_CommandWindow:GetContents():GetHeight() * g_CommandWindow:GetContents():GetWidth()) then
		return
	end
	
	-- If clicked item is empty, bail out
	if a_ClickedItem:IsEmpty() then
		return true
	end
	
	-- Check if clicked item slot number is in g_Actions
	if not(g_Actions[a_SlotNum]) then
		return true
	end
	
	-- Open crafting table, crafting not possible though
	if g_Actions[a_SlotNum].OpenWindow then
		a_Player:OpenWindow(cLuaWindow(g_Actions[a_SlotNum].OpenWindow, 3, 3, "Crafting"))
		return true
	end
	
	-- Execute command
	if g_Actions[a_SlotNum].RunCommand then
		cPluginManager:Get():ExecuteCommand(a_Player, g_Actions[a_SlotNum].RunCommand)
		if (g_Actions[a_SlotNum].CloseWindow) then
			a_Player:CloseWindow()
		end
		return true
	end	
	
	return true
end



function CommandOpenWindow(a_Split, a_Player)
	-- Recreate command gui on every call, to load files changes if there are any
	CreateCommandGUI()
	
	-- Open window
	a_Player:OpenWindow(g_CommandWindow)

	return true
end

