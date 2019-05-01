#pragma once

#include "Action.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)

	public:
		static const std::string PrototypeKey;
		static const std::size_t PrototypeIndex;
		static const std::string ActionNameKey;
		static const std::size_t ActionNameIndex;
		static Vector<Signature> Signatures();

	public:
		explicit ActionCreateAction(const std::string& className = std::string(), const std::string& actionName = std::string(), const std::string& name = std::string());
		virtual ~ActionCreateAction() = default;

		virtual gsl::owner<ActionCreateAction*> Clone() const override;
		virtual void Update(WorldState& worldState) override;

		std::string& Prototype();
		const std::string& Prototype() const;
		std::string& ActionName();
		const std::string& ActionName() const;

	private:
		std::string mPrototype;
		std::string mActionName;
	};

	CONCRETE_FACTORY(ActionCreateAction, Scope)
}
