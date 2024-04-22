//
// Created by César B. on 4/12/2024.
//

#include "globals/globals.hpp"

namespace globals
{	data_structures::theme_registry theme_registry =
	{	.categories =
		{	{	.label = "qmc2",
				.sub_categories = {},
				.items =
				{	{ .label = "Black", .resource_path = ":/RC_THEMES/qmc2/black-0.11/theme.qss" },
					{ .label = "Diefer", .resource_path = ":/RC_THEMES/qmc2/diefer-0.10/theme.qss" },
					{ .label = "Fire", .resource_path = ":/RC_THEMES/qmc2/fire-0.9/theme.qss" },
					{ .label = "Metal", .resource_path = ":/RC_THEMES/qmc2/metal-0.10/theme.qss" },
					{ .label = "Sandy", .resource_path = ":/RC_THEMES/qmc2/sandy-0.10/theme.qss" },
					{ .label = "XMas", .resource_path = ":/RC_THEMES/qmc2/xmas-0.5/theme.qss" },
				}
			},
			{	.label = "Jaime Quiroga",
				.sub_categories = {},
				.items =
				{	{ .label = "Aqua", .resource_path = ":/RC_THEMES/Jaime Quiroga/Aqua.qss" },
				}
			},
			{	.label = "Mathéo Allart",
				.sub_categories = {},
				.items =
				{	{ .label = "Coming Soon... (Default)", .resource_path = "" },
				}
			}
		},
		.global_items = { { .label = "Default", .resource_path = "" } }
	};
}
