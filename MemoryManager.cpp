#include "MemoryManager.h"
#include <iostream>

memoryManager::memoryManager() {}

memoryManager::memoryManager(int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickLists, unsigned int numberMemoryCalls) {
	this->allocationAlgorithim = memAlockAlgo;
	this->totalMemory = totalInstalledMemory;
	this->numberQuickLists = numberQuickLists;
	this->minAmountCalls = numberMemoryCalls;
	quickFitBlocks.resize(numberQuickLists);

}

int memoryManager::getAvailableMemory() {
	return totalMemory - occupiedMemory - memoryOverhead;
}

memoryBlock* memoryManager::malloc(unsigned int SIZE) {
	if (checkFreeMemory(SIZE)) {
		return setAllocationAlgorithim(SIZE);
	}
	else {
		return nullptr;
	}
}

void memoryManager::free(memoryBlock* mem) {
	if (mem != nullptr) {
		occupiedMemory -= mem->occupiedSize;
		mem->occupiedSize = 0;
		mem->nextFreeBlock = nullptr;
		if (allocationAlgorithim == 3) {
			freeQuickList(mem);
		}
		else {
			freeMemory(mem);
		}
	}
}

bool memoryManager::checkFreeMemory(unsigned int SIZE) {
	if (getAvailableMemory() - BLOCK_SIZE >= SIZE) {
		return true;
	}
	else {
		memoryBlock* nextBl = freeBlocksList;
		while (nextBl != nullptr) {
			if (nextBl->totalBlockSize >= SIZE) {
				return true;
			}
			nextBl = nextBl->nextFreeBlock;
		}
		return false;
	}
}

void memoryManager::freeMemory(memoryBlock* mem) {
	memoryBlock* m = freeBlocksList;
	if (m != nullptr) {
		while (m->nextFreeBlock != nullptr) {
			m = m->nextFreeBlock;
		}
		m->nextFreeBlock = mem;
	}
	else {
		freeBlocksList = mem;
	}
}

memoryBlock* memoryManager::setAllocationAlgorithim(unsigned int SIZE) {
	switch (allocationAlgorithim) {
	case 1:
		return firstFit(SIZE);
	case 2:
		return bestFit(SIZE);
	case 3:
		return quickFit(SIZE);
	default:
		return NULL;
	}
}

memoryBlock* memoryManager::firstFit(int SIZE) {
	memoryBlock* m;
	m = freeBlocksList;

	while (m != nullptr) {

		if (m->totalBlockSize >= SIZE) {

			if (m == freeBlocksList) {
				m->occupiedSize = SIZE;
				occupiedMemory += SIZE;
				freeBlocksList = m->nextFreeBlock;
				m->nextFreeBlock = nullptr;
				return m;
			}

			memoryBlock* aux = freeBlocksList;
			while (aux->nextFreeBlock->blockAdress != m->blockAdress) {
				aux = aux->nextFreeBlock;
			}
			aux->nextFreeBlock = m->nextFreeBlock;
			occupiedMemory += SIZE;
			m->occupiedSize = SIZE;
			m->nextFreeBlock = nullptr;
			return m;
		}
		m = m->nextFreeBlock;
	}

	if (getAvailableMemory() - BLOCK_SIZE - SIZE >= 0) {
		occupiedMemory += SIZE;
		memoryOverhead += BLOCK_SIZE;
		m = new memoryBlock(SIZE, adrCounter++);
		memory.emplace_back(m);
		return m;
	}

	else {
		return nullptr;
	}


}

memoryBlock* memoryManager::bestFit(int SIZE) {
	memoryBlock* m = freeBlocksList;

	int count = INT32_MAX;
	memoryBlock* bf = nullptr;
	int test;
	while (m != nullptr) {
		test = m->totalBlockSize - SIZE;
		if (test >= 0 && test <= count) {
			count = test;
			bf = m;
		}
		m = m->nextFreeBlock;
	}

	if (bf != nullptr && bf->totalBlockSize <= SIZE) {

		if (bf == freeBlocksList) {
			bf->occupiedSize = SIZE;
			occupiedMemory += SIZE;
			freeBlocksList = bf->nextFreeBlock;
			bf->nextFreeBlock = nullptr;
			return bf;
		}
		else {
			m = freeBlocksList;
			while (m->nextFreeBlock->blockAdress != bf->blockAdress) {
				m = m->nextFreeBlock;
			}

			m->nextFreeBlock = bf->nextFreeBlock;
			occupiedMemory += SIZE;
			bf->occupiedSize = SIZE;
			bf->nextFreeBlock = nullptr;
			return bf;
		}

	}

	if ((getAvailableMemory() - BLOCK_SIZE - SIZE) >= 0) {
		occupiedMemory += SIZE;
		memoryOverhead += BLOCK_SIZE;
		m = new memoryBlock(SIZE, adrCounter++);
		memory.emplace_back(m);
		return m;
	}
	else {
		return nullptr;
	}
}

memoryBlock* memoryManager::quickFit(int SIZE) {
	updateStatistics(SIZE);
	vector<int> v = getMostFrequent(numberQuickLists);
	if (v.empty()) {
		return firstFit(SIZE);
	}
	else if (v == quickListMembers) {
		return quickFitAlocation(SIZE);
	}
	else {
		//preencher o quickFitBlocks

		memoryBlock* aux = freeBlocksList;
		if (aux == nullptr) {
			return firstFit(SIZE);
		}
		else {
			

			//@esvaziar a lista
			for (int i = 0; i < quickListMembers.size(); i++) {
				for (int j = 0; j < quickFitBlocks.at(i).size(); j++) {
					while (aux->nextFreeBlock != nullptr) {
						aux = aux->nextFreeBlock;
					}
					aux->nextFreeBlock = quickFitBlocks.at(i).at(j);
					quickFitBlocks.at(i).at(j)->nextFreeBlock = nullptr;
					quickFitBlocks.at(i).erase(quickFitBlocks.at(i).begin() + j);
				}
				aux = freeBlocksList;
			}

			quickListMembers = v;

			//@preencher a lista

			aux = freeBlocksList;
			for (int i = 0; i < quickListMembers.size(); i++) {
				while (aux != nullptr) {
				
					if (aux->totalBlockSize == quickListMembers.at(i)) {
						if (aux == freeBlocksList) {
							freeBlocksList = aux->nextFreeBlock;
							aux->nextFreeBlock = nullptr;
							quickFitBlocks.at(i).emplace_back(aux);
						}
						else {
							memoryBlock* aux1 = freeBlocksList;

							while (aux1->nextFreeBlock->blockAdress != aux->blockAdress) {
								aux1 = aux1->nextFreeBlock;
							}

							aux1->nextFreeBlock = aux->nextFreeBlock;
							aux->nextFreeBlock = nullptr;

							quickFitBlocks.at(i).emplace_back(aux);
						}

					}

					aux = aux->nextFreeBlock;
				}
				aux = freeBlocksList;
			}

		}

		return quickFitAlocation(SIZE);
	}
}

void memoryManager::updateStatistics(int SIZE) {
	for (auto& i : statistics) {
		if (i.first == SIZE) {
			i.second++;
			return;
		}
	}
	statistics.insert(pair<int, int>(SIZE, 1));
}

vector<int> memoryManager::getMostFrequent(int QTY) {
	map<int, int> aux;

	vector<int> res;

	for (auto& i : statistics) {

		aux.insert(pair<int, int>(i.second, i.first));
	}

	int counter = 0;
	for (map<int, int>::const_iterator it = aux.end(); it != aux.begin(); it--) {

		if (it != aux.end()) {
			if (counter < QTY && it->first >= minAmountCalls) {

				counter++;

				res.push_back(it->second);
			}
		}


	}

	return res;
}

void memoryManager::freeQuickList(memoryBlock* mem) {
	for (int i = 0; i < quickListMembers.size(); i++) {
		if (quickListMembers[i] == mem->totalBlockSize) {
			quickFitBlocks.at(i).emplace_back(mem);
			occupiedMemory -= mem->occupiedSize;
			mem->occupiedSize = 0;
			mem->nextFreeBlock = nullptr;
			return;
			
		}
	}
	mem->nextFreeBlock = nullptr;
	freeMemory(mem);
}

memoryBlock* memoryManager::quickFitAlocation(int SIZE) {
	for (int i = 0; i < quickListMembers.size(); i++) {
		if (SIZE == quickListMembers[i]) {
			for (int j = 0; quickFitBlocks.at(i).size(); j++) {
				memoryBlock* m = quickFitBlocks.at(i).at(j);
				if (m->occupiedSize == 0) {
					m->occupiedSize = SIZE;
					m->nextFreeBlock = nullptr;
					quickFitBlocks.at(i).erase(quickFitBlocks.at(i).begin()+j);
					return m;
				}
			}
		}

	}
	return firstFit(SIZE);
}
