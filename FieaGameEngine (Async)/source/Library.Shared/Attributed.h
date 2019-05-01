#pragma once

#include "Scope.h"
#include "Signature.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Attributed abstract class, mirrors derived classes and their data members into scopes with entries
	/// </summary>
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope)

	public:
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="other">attributed</param>
		Attributed(const Attributed& other);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">attributed</param>
		Attributed(Attributed&& other);

		/// <summary>
		/// Copy assignment
		/// </summary>
		/// <param name="other">attributed</param>
		/// <returns>reference to self</returns>
		Attributed& operator=(const Attributed& other);

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="other">attributed</param>
		/// <returns>reference to self</returns>
		Attributed& operator=(Attributed&& other);

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Attributed() = default;

		/// <summary>
		/// Equality operator
		/// </summary>
		/// <param name="other">attributed</param>
		/// <returns>true if equal, false otherwise</returns>
		bool operator==(const Attributed& other) const;

		/// <summary>
		/// unequality operator
		/// </summary>
		/// <param name="other">attributed</param>
		/// <returns>true if not equal, false otherwise</returns>
		bool operator!=(const Attributed& other) const;

		/// <summary>
		/// Adds an attribute (entry) to this scope
		/// </summary>
		/// <exception cref="std::exception">thrown if the name is the name of a prescribed attribute</exception>
		/// <param name="name">name for the new attribute</param>
		/// <returns>datum mapped to the added or existing attribute</returns>
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <param name="datum"></param>
		/// <returns></returns>
		Datum& AppendAuxiliaryAttribute(const std::string& name, const Datum& datum);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pair"></param>
		/// <returns></returns>
		Datum& AppendAuxiliaryAttribute(const Scope::EntryType& pair);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pair"></param>
		/// <returns></returns>
		Datum& AppendAuxiliaryAttribute(Scope::EntryType&& pair);

		/// <summary>
		/// determines if the given name is the name of an attribute in this scope
		/// </summary>
		/// <param name="name">name</param>
		/// <returns>true if present, false otherwise</returns>
		bool IsAttribute(const std::string& name) const;

		/// <summary>
		/// determines if the given name is the name of a prescribed attribute in this scope
		/// a prescribed attribute is an attribute that is always present on all instances of the class
		/// </summary>
		/// <param name="name">name</param>
		/// <returns>true if prescribed, false otherwise</returns>
		bool IsPrescribedAttribute(const std::string& name) const;

		/// <summary>
		/// determines if the given name is the name of a auxiliary attribute in this scope
		/// an auxiliary attribute is a an attribute that is not prescribed and are only present
		/// in the instance they were created
		/// </summary>
		/// <param name="name">name</param>
		/// <returns>true if auxiliary, false otherwise</returns>
		bool IsAuxiliaryAttribute(const std::string& name) const;

		/// <summary>
		/// gets all the attributes (entries) of this scope
		/// </summary>
		/// <returns>vector of pointer to entries of this scope</returns>
		Vector<EntryType*> Attributes();

		/// <summary>
		/// gets all the attributes (entries) of this scope
		/// </summary>
		/// <returns>vector of pointer to entries of this scope</returns>
		Vector<const EntryType*> Attributes() const;

		/// <summary>
		/// gets the prescribed attributes (entries) of this scope
		/// </summary>
		/// <returns>vector of pointer to entries of this scope</returns>
		Vector<EntryType*> PrescribedAttributes();

		/// <summary>
		/// gets the prescribed attributes (entries) of this scope
		/// </summary>
		/// <returns>vector of pointer to entries of this scope</returns>
		Vector<const EntryType*> PrescribedAttributes() const;

		/// <summary>
		/// gets the auxiliary attributes (entries) of this scope
		/// </summary>
		/// <returns>vector of pointer to entries of this scope</returns>
		Vector<EntryType*> AuxiliaryAttributes();

		/// <summary>
		/// gets the auxiliary attributes (entries) of this scope
		/// </summary>
		/// <returns>vector of pointer to entries of this scope</returns>
		Vector<const EntryType*> AuxiliaryAttributes() const;

		/// <summary>
		/// removes all the entries in this scope
		/// external prescribed attributes are not removed or changed
		/// internal prescribed attributes (nested scopes) are default constructed
		/// </summary>
		virtual void Clear() override;

		void ClearAuxiliaryAttributes();

		/// <summary>
		/// creates a clone of this object (heap allocated)
		/// </summary>
		/// <returns>pointer to cloned object</returns>
		virtual gsl::owner<Attributed*> Clone() const override = 0;

		/// <summary>
		/// compares two attributeds for equality
		/// </summary>
		/// <param name="other">attributed</param>
		/// <returns>true if equal, false otherwise</returns>
		virtual bool Equals(const RTTI* other) const override;

		/// <summary>
		/// gets the string representation of this object
		/// </summary>
		/// <returns>string</returns>
		virtual std::string ToString() const override;

	protected:
		/// <summary>
		/// Constructor, needs to know which type is being instantiated
		/// <param name="type">type id</param>
		/// </summary>
		explicit Attributed(RTTI::IdType type);

	private:
		/// <summary>
		/// gets the signatures for the given class
		/// </summary>
		/// <param name="type">class type</param>
		/// <returns>vector of signatures</returns>
		Vector<Signature> GetSignatures() const;

		/// <summary>
		/// gets the number of prescribed attributes for this attributed object
		/// </summary>
		/// <returns>number of prescribed attributes</returns>
		size_t GetPrescribedAttributeCount() const;

		/// <summary>
		/// populates this scope according to its list of signatures
		/// </summary>
		/// <param name="type">type of this instance</param>
		void Populate(RTTI::IdType type);

		/// <summary>
		/// updates the prescribed attributes of this scope
		/// </summary>
		/// <param name="type">class type</param>
		void UpdatePrescribedAttributes(RTTI::IdType type);

		void PrescribedNameGuard(const std::string& name);
	};
}
