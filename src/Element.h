#pragma once

namespace stx
{
	struct Bindings;

	class Element
	{
	public:
		enum class Type
		{
			Token,
			Pack
		};

		virtual Type GetElementType() const = 0;
		virtual void Normalize() = 0;
	};
}