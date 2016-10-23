#include <iostream>
#include <cassert>
#include <cstring>
#include "HashTable.h"

using namespace std;

HashTable::HashEntry::HashEntry(uint32_t& id, path_entry *entry) : nodeId(id), nodeEntry(entry) { }

HashTable::HashEntry::~HashEntry() {}

HashTable::HashTable(uint32_t& numofbucks) : hashentries(numofbucks), _elements(0) {

	t_hash = new LinkedList<HashEntry> *[hashentries];
	assert(t_hash != NULL);

	for (int i = 0; i < hashentries; i++)  {
		t_hash[i] = new LinkedList<HashEntry>;
		assert(t_hash[i] != NULL);
	}
	
}

HashTable::~HashTable() {

	if (t_hash != NULL) {
		iterandel();

		for (int i = 0; i < hashentries; i++) {
			if (t_hash[i] != NULL)
				delete t_hash[i];
		}

		delete[] t_hash;
		t_hash = NULL;
		hashentries = 0;
	}
}

void HashTable::iterandel() {
	//Delete every entry
	HashEntry *iter;

	for (int i = 0; i < hashentries; i++) {
		if (t_hash[i]->empty() == true) continue;
		iter = NULL;
		while ((iter = t_hash[i]->get_next()) != NULL) {
			if (iter->nodeEntry != NULL) {
				delete iter->nodeEntry;
				iter->nodeEntry = NULL;
			}
			t_hash[i]->pop_front();
			_elements--;
		}
	}
}

int HashTable::size() {
	return _elements;
}

uint32_t HashTable::hash(uint32_t& id) { 
	return id % hashentries;
}


void HashTable::insert(path_entry *nodeEntry) {
	int pos = this->hash(nodeEntry->nodeId);
	HashEntry qq(nodeEntry->nodeId, nodeEntry);
	t_hash[pos]->push_front(qq);
	_elements++;
	// cout << "inserted" << *acc << endl;
}

bool HashTable::search(uint32_t& entry_id, path_entry** data) {
	int pos = this->hash(entry_id);
	HashEntry temp(entry_id, NULL);
	HashEntry *tempx;
	tempx = t_hash[pos]->member(temp);
	if (tempx != NULL)
		*data = tempx->nodeEntry;
	return tempx != NULL;
}


void HashTable::print() {
	for (int i = 0; i < hashentries; i++) {
		if (t_hash[i] == NULL) continue;
		if (t_hash[i]->empty()) continue;
		cout << "bucket[" << i << "]:" << endl;
		t_hash[i]->print();
	}
}

ostream& operator<<(ostream& out, const HashTable::HashEntry& d) {
	if (d.nodeEntry != NULL)
		out << *(d.nodeEntry) << endl;
	return out;
}

bool operator== (const HashTable::HashEntry& e1, const HashTable::HashEntry& e2) {
	return e1.nodeId == e2.nodeId;
}

int main(void) {
	uint32_t i = 10;
	HashTable *table = new HashTable(i);
	path_entry *e1 = new path_entry();
	path_entry *e2 = new path_entry();
	e1->nodeId = 5;
	e2->nodeId = 10;

	table->insert(e1);
	table->insert(e2);
	table->print();
	path_entry *temp;
	bool b;
	uint32_t id = 5;
	if ((b = table->search(id, &temp)) != false) {
		cout << "exists" << endl;
	}
	id = 100;
	if ((b = table->search(id, &temp)) != false) {
		cout << "exists" << endl;
	} else
		cout << "does not exist" << endl;
	delete table;
}