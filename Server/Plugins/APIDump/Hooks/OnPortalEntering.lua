return
{
	HOOK_PORTAL_ENTERING =
	{
		CalledWhen = "Called when a entity is entering a portal",
		DefaultFnName = "OnPortalEntering",  -- also used as pagename
		Desc = [[
			This hook is called when a entity is entering a portal.
		]],
		Params =
		{
			{ Name = "Entity", Type = "{{cEntity}}", Notes = "The entity that is entering a portal" },
			{ Name = "Vector3d", Type = "{{Vector3d}}", Notes = "The destination position of the portal" },
		},
		Returns = [[
			If the function returns false or no value, the next plugin's callback is called.
			If the function only returns true, no other callback is called for this event
			and the entity will be disallowed to use the portal.
			<p>
			The function may return true and a Vector3d to change the destination position for the portal
			Returning a Vector3d will allow the entity to use the portal.
		]],
	},  -- HOOK_PORTAL_ENTERING
}
