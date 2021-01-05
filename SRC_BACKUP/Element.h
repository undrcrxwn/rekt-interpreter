#pragma once
#include "bindings.h"

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
		virtual void Expound(const Bindings& b) = 0;
		virtual void Normalize() = 0;
	};
}