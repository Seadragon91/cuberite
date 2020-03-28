return
{
	HOOK_PORTAL_CREATING =
	{
		CalledWhen = "Called before a portals is created",
		DefaultFnName = "OnPortalCreating",  -- also used as pagename
		Desc = [[
			This hook is called before a destination portal is created.
		]],
		Params =
		{
			{ Name = "Entity", Type = "{{cEntity}}", Notes = "The entity that has entered a portal" },
			{ Name = "Vector3d", Type = "{{Vector3d}}", Notes = "The destination position of the portal" },
		},
		Returns = [[
			If the function returns false or no value, the next plugin's callback is called, and finally
			Cuberite will create the destination portal. If the function returns true,
			no other callback is called for this event and the destination portal won't be created.
			<p>
			Note: The entity will still be teleported even if the callback has been cancelled,
			to disallow the teleport use the hook HOOK_ENTITY_CHANGING_WORLD.
		]],
	},  -- HOOK_PORTAL_CREATING
}
