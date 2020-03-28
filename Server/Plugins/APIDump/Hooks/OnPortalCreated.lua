return
{
	HOOK_PORTAL_CREATED =
	{
		CalledWhen = "Called when a entity has caused the creation of a destination portal",
		DefaultFnName = "OnPortalCreated",  -- also used as pagename
		Desc = [[
			This hook is called when a destination portal has been created.
		]],
		Params =
		{
			{ Name = "Entity", Type = "{{cEntity}}", Notes = "The entity that has entered the origin of the destination portal" },
			{ Name = "Vector3d", Type = "{{Vector3d}}", Notes = "The destination position of the portal" },
		},
		Returns = [[
			If this function returns false or no value, Cuberite calls other plugins with the same event. If
			this function returns true, no other plugin is called for this event.
		]],
	},  -- HOOK_PORTAL_CREATED
}
