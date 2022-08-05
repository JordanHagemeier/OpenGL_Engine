#include "JsonArchive.h"
#include <iostream>
#include <sstream>
#include <fstream>


const std::string JsonSerializationArchive::ROOTPATH = "../saves/";

void JsonSerializationArchive::Save(const std::string& name)
{
	// Init Saving
	std::stringstream filePath;
	filePath << ROOTPATH << name << ".json";

	std::ofstream fileStream;
	fileStream.open(filePath.str()); 

	// Save Stuff
	m_Root["hello"] = "world";
	
	// Call Serialize on GameObjectManager, who call Serialize on all GOs, ..

	// Save To File
	fileStream << m_Root.dump(4);
	fileStream.close();

}

void JsonSerializationArchive::Load(const std::string& name)
{
	// Init Loading
	std::stringstream filePath;
	filePath << ROOTPATH << name << ".json";

	std::ifstream fileStream;
	fileStream.open(filePath.str()); 

	// Load from File
	m_Root.clear();
	fileStream >> m_Root;
	fileStream.close();

	// Load Stuff
	std::cout<< m_Root.dump(4);
	std::cout<< m_Root["hello"];
	
	// Call Serialize on GameObjectManager, who call Serialize on all GOs, ..

}



void JsonSerializationArchive::Serialize(bool& value, const std::string& label)
{
	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) { //Du versuchst hier, auf m_ArchiveSaves zuzugreifen, als wäre es eine statische variable. Stattdessen möchtest du einfach nur "m_ArchiveSaves" oder sowas wie "IsSerializing";
		(*m_pCurrentJsonObject)[label] = value;
	}

	else {
		value = (*m_pCurrentJsonObject)[label].get<bool>();
	}
	
}

//void JsonSerializationArchive::Serialize(int& value, const std::string& label) {
//	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) {
//		(*m_pCurrentJsonObject)[label] = value;
//	}
//
//	else {
//		value = (*m_pCurrentJsonObject)[label].get<int>();
//	}
//
//}

void JsonSerializationArchive::Serialize(uint8_t& value, const std::string& label)
{
	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) {
		(*m_pCurrentJsonObject)[label] = value;
	}

	else {
		value = (*m_pCurrentJsonObject)[label].get<uint8_t>();
	}

}

void JsonSerializationArchive::Serialize(_int8& value, const std::string& label)
{
	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) {
		(*m_pCurrentJsonObject)[label] = value;
	}

	else {
		value = (*m_pCurrentJsonObject)[label].get<_int8>();
	}

}

void JsonSerializationArchive::Serialize(uint16_t& value, const std::string& label)
{
	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) {
		(*m_pCurrentJsonObject)[label] = value;
	}

	else {
		value = (*m_pCurrentJsonObject)[label].get<uint16_t>();
	}

}

void JsonSerializationArchive::Serialize(_int16& value, const std::string& label)
{
	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) {
		(*m_pCurrentJsonObject)[label] = value;
	}

	else {
		value = (*m_pCurrentJsonObject)[label].get<_int16>();
	}

}

void JsonSerializationArchive::Serialize(uint32_t& value, const std::string& label)
{
	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) {
		(*m_pCurrentJsonObject)[label] = value;
	}

	else {
		value = (*m_pCurrentJsonObject)[label].get<uint32_t>();
	}

}

void JsonSerializationArchive::Serialize(_int32& value, const std::string& label)
{
	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) {
		(*m_pCurrentJsonObject)[label] = value;
	}

	else {
		value = (*m_pCurrentJsonObject)[label].get<_int32>();
	}

}

void JsonSerializationArchive::Serialize(uint64_t& value, const std::string& label)
{
	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) {
		(*m_pCurrentJsonObject)[label] = value;
	}

	else {
		value = (*m_pCurrentJsonObject)[label].get<uint64_t>();
	}

}

void JsonSerializationArchive::Serialize(_int64& value, const std::string& label)
{
	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) {
		(*m_pCurrentJsonObject)[label] = value;
	}

	else {
		value = (*m_pCurrentJsonObject)[label].get<_int64>();
	}

}

void JsonSerializationArchive::Serialize(float& value, const std::string& label)
{
	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) {
		(*m_pCurrentJsonObject)[label] = value;
	}

	else {
		value = (*m_pCurrentJsonObject)[label].get<float>();
	}

}

void JsonSerializationArchive::Serialize(double& value, const std::string& label)
{
	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) {
		(*m_pCurrentJsonObject)[label] = value;
	}

	else {
		value = (*m_pCurrentJsonObject)[label].get<double>();
	}

}

void JsonSerializationArchive::Serialize(std::string& value, const std::string& label)
{
	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) {
		(*m_pCurrentJsonObject)[label] = value;
	}

	else {
		value = (*m_pCurrentJsonObject)[label].get<std::string>();
	}

}

void JsonSerializationArchive::Serialize(VectorInterface& value, const std::string& label)
{
	if (JsonSerializationArchive::m_CurrentState == SerializationMode::Serialize) {
		JSONObject* oldJson = m_pCurrentJsonObject;

		JSONObject vectorJson;
		m_pCurrentJsonObject = &vectorJson;

		const size_t vectorSize = value.GetSize();
		for (int i = 0; i < vectorSize; i++)
		{
			value.SerializeElement(*this, i);
		}

		(*oldJson)[label] = vectorJson;
		m_pCurrentJsonObject = oldJson;
	}
	
	else {
		JSONObject* oldJson = m_pCurrentJsonObject;

		JSONObject& vectorJson = (*m_pCurrentJsonObject)[label];
		m_pCurrentJsonObject = &vectorJson;

		const size_t vectorSize = vectorJson.size();
		value.Resize(vectorSize);

		for (int i = 0; i < vectorSize; i++)
		{
			value.SerializeElement(*this, i);
		}

		m_pCurrentJsonObject = oldJson;
	}
}