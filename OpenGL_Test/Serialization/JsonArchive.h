#pragma once
#include "../externals/json/json.hpp"
#include "VectorInterface.h"
#include <iostream>
#include "../glm-master/glm/glm.hpp"


using JSONObject	= nlohmann::ordered_json;

enum SerializationMode {
	Serialize,
	Deserialize
};

//Wer das ließt, ist ultra hübsch *.*;


//struct IJsonArchive
//{
//
//	//virtual bool IsInput() const = 0;
//	//virtual bool IsOutput() const = 0;
//
//	//virtual void Serialize(bool& value, const std::string& label) = 0;
//	//virtual void Serialize(uint8_t& value, const std::string& label) = 0;
//	//virtual void Serialize(_int8& value, const std::string& label) = 0;
//	//virtual void Serialize(uint16_t& value, const std::string& label) = 0;
//	//virtual void Serialize(_int16& value, const std::string& label) = 0;
//	//virtual void Serialize(uint32_t& value, const std::string& label) = 0;
//	//virtual void Serialize(_int32& value, const std::string& label) = 0;
//	//virtual void Serialize(uint64_t& value, const std::string& label) = 0;
//	//virtual void Serialize(_int64& value, const std::string& label) = 0;
//	//virtual void Serialize(float& value, const std::string& label) = 0;
//	//virtual void Serialize(double& value, const std::string& label) = 0;
//	//virtual void Serialize(std::string& value, const std::string& label) = 0;
//	//virtual void Serialize(VectorInterface& value, const std::string& label) = 0;
//
//	template<typename T>
//	void Serialize(std::vector<T>& value, const std::string& label)
//	{
//		VectorInterface_Impl<T> vectorInterfaceImpl(value);
//		Serialize(vectorInterfaceImpl, label);
//	}
//
//
//	
//
//
//};

template<typename T>
void IJsonArchive::Serialize(std::vector<T>& value, const std::string& label)
	{
		VectorInterface_Impl<T> vectorInterfaceImpl(value);
		Serialize(vectorInterfaceImpl, label);
	}


class JsonSerializationArchive : public IJsonArchive {

	static const std::string ROOTPATH;

	JSONObject m_Root;
	 //used for nesting with vectors

	SerializationMode m_CurrentState;	 /*Speicher dir am Besten einfach den SerializationState hier ;) (Und vll sollte er SerialisationModeheißen?);
										 Dann brauchst du auch keine zwei Variablen, die das gleiche sagen :);*/

public:

	JSONObject* m_pCurrentJsonObject;
	JsonSerializationArchive(SerializationMode state)
		: m_pCurrentJsonObject(&m_Root)
		{ 
		/*Hier wäre auch möglich state == SerializationState.Serialize;*/
		if (state < 2 && state > -1) {
			 m_CurrentState = state;
			
		}
		else{
		std::cout << "Serialization state neither serialize nor deserialize" << std::endl; 
			}
		}

	//might need these later, at least these are the save and load functions
	void Save(const std::string& name); 
	void Load(const std::string& name);

	 SerializationMode GetSerializationMode() { return m_CurrentState; }
	 

	 void Serialize(bool& value, const std::string& label) ;
	 /*void Serialize(int& value, const std::string& label);*/
	 void Serialize(uint8_t& value, const std::string& label) ;
	 void Serialize(_int8& value, const std::string& label) ;
	 void Serialize(uint16_t& value, const std::string& label) ;
	 void Serialize(_int16& value, const std::string& label) ;
	 void Serialize(uint32_t& value, const std::string& label) ;
	 void Serialize(_int32& value, const std::string& label) ;
	 void Serialize(uint64_t& value, const std::string& label) ;
	 void Serialize(_int64& value, const std::string& label) ;
	 void Serialize(float& value, const std::string& label) ;
	 void Serialize(double& value, const std::string& label) ;
	 void Serialize(std::string& value, const std::string& label) ;
	 void Serialize(VectorInterface& value, const std::string& label) ;

	 // Vector
	 template <typename T>
	 void Serialize(std::vector<T*>& vectorT, const std::string& label)
	 {
		 JSONObject* nodeBeginPtr = m_pCurrentJsonObject;
		 if (GetSerializationMode() == SerializationMode::Serialize)
		 {
			 // Saving
			 auto jsonArray = nlohmann::json::array();
			 for (const auto& entryT : vectorT)
			 {
				 JSONObject entryObject;
				 m_pCurrentJsonObject = &entryObject;
				 entryT->Serialize(*this);
				 jsonArray.push_back(entryObject);
			 }

			 m_pCurrentJsonObject = nodeBeginPtr;
			 (*m_pCurrentJsonObject)[label] = jsonArray;
		 }
		 else
		 {
			 // Loading
			 vectorT.clear();
			 auto jsonArray = (*m_pCurrentJsonObject)[label];
			 const size_t size = jsonArray.size();
			 vectorT.reserve(size);

			 for (int i = 0; i < size; i++)
			 {
				 auto jsonEntry = jsonArray.at(i);
				 T* entryT = new T();
				 m_pCurrentJsonObject = &jsonEntry;
				 entryT->Serialize(*this);

				 vectorT.push_back(entryT);
			 }

			 m_pCurrentJsonObject = nodeBeginPtr;
		 }
	 }

	 // Vector with factory
	 template <typename T>
	 void Serialize(std::vector<T*>& vectorT, const std::string& label, std::function<T* (const nlohmann::ordered_json&)> factoryFunction)
	 {
		 JSONObject* nodeBeginPtr = m_pCurrentJsonObject;
		 if (GetSerializationMode() == SerializationMode::Serialize)
		 {
			 // Saving
			 auto jsonArray = nlohmann::json::array();
			 for (const auto& entryT : vectorT)
			 {
				 JSONObject entryObject;
				 m_pCurrentJsonObject = &entryObject;
				 entryT->Serialize(*this);
				 jsonArray.push_back(entryObject);
			 }

			 m_pCurrentJsonObject = nodeBeginPtr;
			 (*m_pCurrentJsonObject)[label] = jsonArray;
		 }
		 else
		 {
			 // Loading
			 vectorT.clear();
			 auto jsonArray = (*m_pCurrentJsonObject)[label];
			 const size_t size = jsonArray.size();
			 vectorT.reserve(size);

			 for (int i = 0; i < size; i++)
			 {
				 auto jsonEntry = jsonArray.at(i);
				 T* entryT = factoryFunction(jsonEntry);
				 m_pCurrentJsonObject = &jsonEntry;
				 entryT->Serialize(*this);

				 vectorT.push_back(entryT);
			 }

			 m_pCurrentJsonObject = nodeBeginPtr;
		 }
	 }
	 

	/* void Serialize(glm::mat4& value, const std::string& label);*/

	/* template<typename T>
	 void Serialize(std::vector<T>& value, const std::string& label)
	 {
		 VectorInterface_Impl<T> vectorInterfaceImpl(value);
		 Serialize(vectorInterfaceImpl, label);
	 }*/

	 /*Hier wollen wir (future TODO, nicht jetzt) eine template funktion bauen, die uns das hier erlaubt: archive.Serialize("DiggesDing", m_DiggesDing);, wobei DiggesDing eine eigene Klasse ist.
	 Das Template würde einen neuen JSON Knoten aufmachen und dann die funktion DiggesDing::Serialize(*this) aufrufen. (*this ist eine referenz zu uns selbst, also dem archive);*/
	

};

