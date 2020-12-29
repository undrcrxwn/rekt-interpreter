#pragma once

namespace stx
{
	class Element
	{
	public:
		enum class Type
		{
			Token,
			Pack
		};

		virtual Type GetType() const = 0;
	};
}