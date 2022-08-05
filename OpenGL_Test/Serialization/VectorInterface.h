#pragma once
#include <vector>

struct IJsonArchive {
	template<typename T>
	void Serialize(std::vector<T>& value, const std::string& label);
	

 };

struct VectorInterface
{
	virtual size_t GetSize() const = 0;
	virtual void Resize(size_t newSize) = 0;
	virtual void SerializeElement(IJsonArchive& archive, size_t index) = 0;
};

template<typename T>
struct VectorInterface_Impl : public VectorInterface
{
	VectorInterface_Impl(std::vector<T>& vector)
		: vectorReference(vector)
	{ }

	virtual size_t GetSize() const override
	{
		return vectorReference.size();
	}

	virtual void Resize(size_t newSize) override
	{
		vectorReference.resize(newSize);
	}

	virtual void SerializeElement(IJsonArchive& archive, size_t index) override
	{
		archive.Serialize(vectorReference[index], std::to_string(index));
	}

	std::vector<T>& vectorReference;
};

