#pragma once

#include "object.hpp"

#define TABLE_MAX_LOAD (0.75)

struct Entry
{
    ObjectString* m_pKey;
    Value m_oValue;
};

class Table
{
public:
	Table();
    int m_iCount;
    int m_iCapacity;
	std::vector<Entry> m_vEntries;

	void AdjustCapacity(int capacity);
	Entry& FindEntry(ObjectString* pKey);
	bool Set(ObjectString *key, Value value);
	void AddAll(Table& from, Table& to);
	bool Get(ObjectString *key, Value& value);
	bool Delete(ObjectString *key);
	ObjectString *FindString(const char *chars, int length, uint32_t hash);
	ObjectString *FindString(const std::string& string, uint32_t hash);
	void MarkTable();
	void RemoveWhite();
};