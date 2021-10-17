//  System-H - A C++ library inspired by the .Net Framework
//	Copyright(C) 2021 Kris De Greve
//
//	This program is free software : you can redistribute it and /or modify
//	it under the terms of the GNU Affero General Public License as
//	published by the Free Software Foundation, either version 3 of the
//	License, or (at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//	GNU Affero General Public License for more details.
//
//	You should have received a copy of the GNU Affero General Public License
//	along with this program.If not, see < https://www.gnu.org/licenses/>.

#ifndef Lorentzoft
#define Lorentzoft

namespace Lorentzoft {
	enum class Formatting {
		Indented = 0,
		None = 1
	};

	class System_API IFiller : public System::Object {
	private:
	public:
		class System_API ObjectData {
			virtual void Fill(System::String const& name) = 0;
		};

		ObjectData* id = nullptr;

		ObjectData* GOD() const { return this->id; };

		IFiller(){}

		IFiller(const std::nullptr_t& n) {
			this->od = nullptr;
			this->id = nullptr;
		}

		IFiller(const IFiller& other) : System::Object(other) { this->id = other.id; }

		IFiller(IFiller&& other) noexcept : System::Object(std::move(other)) { this->id = other.id; other.id = nullptr;  }

		IFiller(System::Object::ObjectData* baseod, ObjectData* other) {
			this->od = baseod;
			this->id = other;
			if (this->od)
				this->AddRef();
		}

		IFiller& operator=(const IFiller& other) {
			System::Object::operator=(other);
			this->id = other.id;
			return *this;
		}

		IFiller& operator=(IFiller&& other) noexcept {
			System::Object::operator=(std::move(other));
			this->id = other.id;
			return *this;
		}

		IFiller* operator->() {
			return this;
		}



	};

	template<class T> class System_API DictionaryFiller : public System::Object {
	private:
		T dic;
	public:
		class System_API ObjectData : public System::Object::ObjectData, public IFiller::ObjectData {
			void Fill(System::String const& name, System::Object value) override {
				dic[name] = value;
			}
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		DictionaryFiller(){}

		DictionaryFiller(std::nullptr_t const & n) {
			this->od = nullptr;
		}

		DictionaryFiller(DictionaryFiller* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				this->od = dd;
			}
			else {
				this->od = pValue->od;
				pValue->od = pValue->od = nullptr;
			}
			delete pValue;
		}

		DictionaryFiller(DictionaryFiller const & other) : System::Object(other) { }

		DictionaryFiller(DictionaryFiller&& other) noexcept : System::Object(std::move(other)) { }

		DictionaryFiller(ObjectData* other) {
			this->od = other;
			if (this->od)
				this->AddRef();
		}

		DictionaryFiller& operator=(DictionaryFiller const & other) {
			System::Object::operator=(other);
			return *this;
		}

		DictionaryFiller& operator=(DictionaryFiller* other) {
			if (this->od == other->od)
				return *this;
			this->Release();
			this->od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		DictionaryFiller& operator=(DictionaryFiller&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		DictionaryFiller* operator->() {
			return this;
		}



	};



	class System_API JsonConvert {
	private:
		struct DeserializationState {
		private:
		public:
			System::String key = null;
			System::Collections::Generic::Stack<System::Tuple<System::String, IFiller>> prevoas{};
			System::Tuple<System::String, IFiller> curfiller = null;
			System::Func<IFiller> CreateObject = null;

		};


		template<class TObject, class TArray> static System::Object DeserializeObject(char16_t const* value, int offset, int count) {
			char16_t const* end = value + offset + count;
			char16_t const* cur = value + offset;

			System::Action<char16_t const, DeserializationState&> act = nullptr;
			System::Action<char16_t const, DeserializationState&> act0 = [](char16_t const chr, DeserializationState& s) {
				switch (chr) {
				case u'{':
				{
					if (s.curfiller != null) {
						s.prevoas.Push(s.curfiller);
					}
					s.curfiller = System::Tuple<System::String, IFiller>(s.key, s.CreateObject());



					break;
				}
				default:
					break;
				};
			};

			act = act0;

			DeserializationState state{};

			while (cur != end) {
				act(*cur, state);
				cur++;
			}

			return null;
		}

		template<class TObject, class TArray> static System::Object DeserializeObject(System::String const& value) {
			char16_t const* chrs = value.ToCharArray();
			return DeserializeObject<TObject, TArray>(chrs, 0, value.Length);
		}




	public:
		static System::String SerializeObject(System::Object const& value, Formatting formatting) {

			return null;
		}






		static System::Object DeserializeObject(System::String const& value) {
			return DeserializeObject<System::Dynamic::ExpandoObject, System::Collections::Generic::List<System::Object>>(value);
		}



	};





}
#endif
