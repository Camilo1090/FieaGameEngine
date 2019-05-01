#pragma once

#include "RTTI.h"
#include "vector.h"
#include <gsl/gsl>
#include <json/forwards.h>
#include <iostream>

namespace FieaGameEngine
{
	class IJsonParseHelper;

	/// <summary>
	/// Main class responsible for parsing json formatted strigs and files into defined
	/// data types
	/// </summary>
	class JsonParseMaster final
	{
	public:
		/// <summary>
		/// main class for storing and sharing the parsed data among helpers and the master
		/// </summary>
		class SharedData : public RTTI
		{
			/// <summary>
			/// master needs to set itself inside shared data
			/// </summary>
			friend JsonParseMaster;

			RTTI_DECLARATIONS(SharedData, RTTI)

		public:
			/// <summary>
			/// default destructor
			/// </summary>
			virtual ~SharedData() = default;

			/// <summary>
			/// initializes this shared data
			/// </summary>
			virtual void Initialize();

			/// <summary>
			/// creates a new instance of this kind of shared data
			/// </summary>
			/// <returns></returns>
			virtual gsl::owner<SharedData*> Create() const = 0;

			/// <summary>
			/// gets the current master
			/// </summary>
			/// <returns>current master</returns>
			JsonParseMaster* GetJsonParseMaster();

			/// <summary>
			/// gets the current master
			/// </summary>
			/// <returns>current master</returns>
			const JsonParseMaster* GetJsonParseMaster() const;

			/// <summary>
			/// increments depth
			/// </summary>
			void IncrementDepth();

			/// <summary>
			/// decrements depth
			/// </summary>
			void DecrementDepth();

			/// <summary>
			/// current depth
			/// </summary>
			/// <returns>current depth</returns>
			size_t Depth() const;

		private:
			/// <summary>
			/// master
			/// </summary>
			JsonParseMaster* mOwner = nullptr;

			/// <summary>
			/// depth
			/// </summary>
			size_t mDepth = 0;

			/// <summary>
			/// sets the master of this data
			/// </summary>
			/// <param name="parser"></param>
			void SetJsonParseMaster(JsonParseMaster& parser);
		};

	public:
		/// <summary>
		/// constructor, requires a shared data to work and an optional helper quantity to allocate memory
		/// </summary>
		/// <param name="data">data</param>
		/// <param name="helperCount">helper count</param>
		explicit JsonParseMaster(SharedData& data, size_t helperCount = 3);

		/// <summary>
		/// deleted copy constructor
		/// </summary>
		/// <param name="">master</param>
		JsonParseMaster(const JsonParseMaster&) = delete;

		/// <summary>
		/// move contructor
		/// </summary>
		/// <param name="other">master</param>
		JsonParseMaster(JsonParseMaster&& other);

		/// <summary>
		/// deleted copy assignment
		/// </summary>
		/// <param name="">master</param>
		/// <returns>reference</returns>
		JsonParseMaster& operator=(const JsonParseMaster&) = delete;

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="other">master</param>
		/// <returns>reference</returns>
		JsonParseMaster& operator=(JsonParseMaster&& other);

		/// <summary>
		/// destructor, deletes members if a clone
		/// </summary>
		~JsonParseMaster();

		/// <summary>
		/// clones this master with all its helpers and shared data
		/// </summary>
		/// <returns>pointer to new clone of master</returns>
		gsl::owner<JsonParseMaster*> Clone() const;

		/// <summary>
		/// initialize all helpers
		/// </summary>
		void Initialize();

		/// <summary>
		/// adds a helper, unique
		/// </summary>
		/// <param name="helper">helper</param>
		/// <returns>true if added, false otherwise</returns>
		bool AddHelper(IJsonParseHelper& helper);

		/// <summary>
		/// removes helper
		/// </summary>
		/// <param name="helper">helper</param>
		/// <returns>true if removed, false otherwise</returns>
		bool RemoveHelper(IJsonParseHelper& helper);

		/// <summary>
		/// attempts to parse the json formatted input string
		/// </summary>
		/// <param name="string">string</param>
		void Parse(const std::string& string);

		/// <summary>
		/// attempts to parse the json formatted input stream
		/// </summary>
		/// <param name="string">stream</param>
		void Parse(std::istream& stream);

		/// <summary>
		/// attempts to parse the json formatted input file
		/// </summary>
		/// <param name="string">file name</param>
		void ParseFromFile(const std::string& filename);

		/// <summary>
		/// gets the helper count
		/// </summary>
		/// <returns>count</returns>
		size_t GetHelperCount() const;

		/// <summary>
		/// gets the file name
		/// </summary>
		/// <returns>file name</returns>
		const std::string& GetFileName() const;

		/// <summary>
		/// gets the shared data
		/// </summary>
		/// <returns>shared data</returns>
		SharedData* GetSharedData();

		/// <summary>
		/// gets the shared data
		/// </summary>
		/// <returns>shared data</returns>
		const SharedData* GetSharedData() const;

		/// <summary>
		/// sets the shared data
		/// </summary>
		/// <param name="data">shared data</param>
		void SetSharedData(SharedData& data);

		/// <summary>
		/// whether or not this master is a clone
		/// </summary>
		/// <returns>true if clone, false otherwise</returns>
		bool IsClone() const;

	private:
		/// <summary>
		/// helpers
		/// </summary>
		Vector<IJsonParseHelper*> mHelpers;

		/// <summary>
		/// shared data
		/// </summary>
		SharedData* mData = nullptr;

		/// <summary>
		/// is this a clone?
		/// </summary>
		bool mIsClone = false;

		/// <summary>
		/// file name
		/// </summary>
		std::string mFileName;

		/// <summary>
		/// parse the members of a json object value
		/// </summary>
		/// <param name="value">json value</param>
		/// <param name="index">index</param>
		void ParseMembers(const Json::Value& value, size_t index = 0);

		/// <summary>
		/// parses a json value
		/// </summary>
		/// <param name="name">key of the json value</param>
		/// <param name="value">json value</param>
		/// <param name="index">index of value</param>
		void Parse(const std::string& name, const Json::Value& value, size_t index = 0);

		/// <summary>
		/// deletes helpers and shared data if this master is a clone
		/// </summary>
		void Clear();
	};
}
