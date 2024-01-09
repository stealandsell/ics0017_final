#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
using namespace std;
#pragma warning ( disable : 4996 )
#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_YELLOW  "\x1b[1;33m"
#define ANSI_COLOR_BLUE    "\x1b[1;34m"
#define ANSI_COLOR_MAGENTA "\x1b[1;35m"
#define ANSI_COLOR_CYAN    "\x1b[1;36m"
#define ANSI_COLOR_RESET   "\x1b[1;0m"


class DataStructure
{

public:

	DataStructure(char* pFilename = nullptr)
	{

		if (pFilename == nullptr) { structure = nullptr; } //if the file is gone, so is the structure
		else {

			ifstream input;
			input.open(pFilename); //opening file

			string fID, lID, day, month; //gettign the file data 
			unsigned long int code;
			int d, year;
			while (input >> fID >> lID >> code >> d >> month >> year) 
			{
				fID = fID + ' ' + lID; //insert space

				ITEM9* item = new ITEM9();
				item->pID = _strdup((char*)fID.c_str()); //dupplicate pId
				item->Code = code;
				item->pDate->Day = stoi(day); //stoi to change str to int
				item->pDate->pMonth = _strdup((char*)month.c_str()); //dublicate month str to pMonth
				item->pDate->Year = year;
				this->operator+=(item);
			}

			input.close();
		}
	}

	~DataStructure() { delete_HD(this->structure); } //deconstructor


	DataStructure(const DataStructure& Original) 
	{
		copy_DS(Original.structure); 
	}

	int GetItemsNumber() {
		int n = 0;

		HEADER_D* headerVar;
		for (headerVar = this->structure; headerVar; headerVar = headerVar->pNext) {

			HEADER_A* otheHeaderV;
			for (otheHeaderV = headerVar->pHeaderA; otheHeaderV; otheHeaderV = otheHeaderV->pNext) {

				ITEM9* i;
				for (i = (ITEM9*)(otheHeaderV->pItems); i; i = i->pNext, n++);
			}
		}
		return n;
	}

	ITEM9* GetItem(char* pID) {
		if (pID == nullptr) { return NULL; }
		find_ID(pID);
		for (HEADER_D* headerVar = this->structure; headerVar != nullptr && first >= headerVar->cBegin; headerVar = headerVar->pNext)
		{
			if (headerVar->cBegin == first)
			{
				for (HEADER_A* HeaderA = headerVar->pHeaderA; HeaderA != nullptr && second >= HeaderA->cBegin; HeaderA = HeaderA->pNext)
				{
					if (HeaderA->cBegin == second)
					{
						for (ITEM9* itemHeaderVar = (ITEM9*)HeaderA->pItems; itemHeaderVar != nullptr; itemHeaderVar = itemHeaderVar->pNext)
						{
							if (!strcmp(itemHeaderVar->pID, pID))
								return itemHeaderVar;
						}
					 //
					}
						
				}

			}
				
					

		}
			//items are inserted as long as headerVariable is present in struct, is not null 
				
		return NULL;
	}

	void operator+=(ITEM9* item) { add_HD(item); }

	void operator-=(char* pID) { remove_item(pID); }

	DataStructure& operator=(const DataStructure& Right) { delete_HD(this->structure); copy_DS(Right.structure); }

	int operator==(DataStructure& Other)
	{

		if (this->structure != nullptr && Other.structure == nullptr)
			return 0;
		else if (this->structure == nullptr && Other.structure != nullptr)
			return 0;

		//loops through both headers and checks if number of entities is equal 
		for (HEADER_D* headerVar = this->structure, *op = Other.structure;
			headerVar != nullptr && op != nullptr; headerVar = headerVar->pNext, op = op->pNext)
		{

			if (headerVar == nullptr || op == nullptr) return 0;

			for (HEADER_A* headerA = headerVar->pHeaderA, *opp = op->pHeaderA; headerA || opp; headerA = headerA->pNext, opp = opp->pNext)
			{
				if (headerA == nullptr || opp == nullptr) return 0;

				for (ITEM9* i = (ITEM9*)headerA->pItems, *oi = (ITEM9*)opp->pItems; i || oi; i = i->pNext, oi = oi->pNext)
				{
					if (i == nullptr || oi == nullptr)
						return 0;
					if (strcmp(i->pID, oi->pID))
						return 0;
				}
			}
		}

		return 1;
	}

	void Write(char* pFilename)
	{
		/*
		std::string input;
		std::cin >> input;
		std::ofstream out("output.txt");

		out << input;
		out.close();
		*/

		if (this->structure == nullptr) { cout << ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET " -------- Structure is missing data"; }
		ofstream output;
		
		output.open(pFilename);
		for (HEADER_D* headerVar = this->structure; headerVar; headerVar = headerVar->pNext) {
			for (HEADER_A* headerA = headerVar->pHeaderA; headerA; headerA = headerA->pNext) {
				for (ITEM9* i = (ITEM9*)headerA->pItems; i; i = i->pNext)
				{
					
					output << i->pID << endl;
					//if(!i->pNext)
						
				}
			}
			
		}
		if(output.is_open())
			output.close();


		/* 
		
		*/
		
	}

	friend ostream& operator<<(ostream& ostr, const DataStructure& str) {

		int n = 1;
		for (HEADER_D* headerVar = str.structure; headerVar; headerVar = headerVar->pNext)
		{
			for (HEADER_A* otheHeaderV = headerVar->pHeaderA; otheHeaderV; otheHeaderV = otheHeaderV->pNext)

				for (ITEM9* i = (ITEM9*)(otheHeaderV->pItems); i; i = i->pNext, n++)
				{
					ostr << n << ')' << i->pID << ' ' << i->Code << ' ' << i->pDate->Day << ' ' << i->pDate->pMonth << ' ' << i->pDate->Year << '\n';
				}

		}
		return ostr;
	}

	HEADER_D* structure = nullptr;

private:
	char first = ' ';
	char second = ' ';
	void find_ID(char* name)
	{
		first = *name;
		char* pName = name;
		for (int i = 1; !isupper(*(pName + i)); i++) { second = *(pName + i + 1); }
	}
	void delete_item(ITEM9* item)
	{
		if (item->pNext != nullptr)
			delete_item(item->pNext);

		item = nullptr;
	}
	void delete_HA(HEADER_A* HeaderA)
	{
		if (HeaderA->pNext != nullptr)
			delete_HA(HeaderA->pNext);

		delete_item((ITEM9*)HeaderA->pItems);
		HeaderA = nullptr;
	}
	void delete_HD(HEADER_D* headerVar) {
		if (headerVar->pNext != nullptr)
			delete_HD(headerVar->pNext);

		delete_HA(headerVar->pHeaderA);
		headerVar = nullptr;
	}

	void copy_DS(HEADER_D* Origin)
	{
		this->structure = new headerD();

		HEADER_D* prevLine = nullptr;
		HEADER_D* recursiveVar = this->structure;

		for (HEADER_D* headerVar = Origin; headerVar != nullptr; headerVar = headerVar->pNext)
		{
			recursiveVar->pPrior = prevLine;
			prevLine = recursiveVar;
			recursiveVar->cBegin = headerVar->cBegin;
			recursiveVar->pHeaderA = new headerA();
			HEADER_A* recursiveVar2 = recursiveVar->pHeaderA;

			if (headerVar->pNext != nullptr)
			{
				HEADER_D* nextLine = new headerD();
				recursiveVar->pNext = nextLine;
				recursiveVar = nextLine;
			}

			for (HEADER_A* HeaderA2 = headerVar->pHeaderA; HeaderA2 != nullptr; HeaderA2 = HeaderA2->pNext)
			{
				recursiveVar2->cBegin = HeaderA2->cBegin;
				recursiveVar2->pItems = new ITEM9();
				ITEM9* recursiveVar3 = (ITEM9*)recursiveVar2->pItems;

				if (HeaderA2->pNext != nullptr) {
					HEADER_A* nextLine2 = new headerA();
					recursiveVar2->pNext = nextLine2;
					recursiveVar2 = nextLine2;
				}

				for (ITEM9* itemHeaderVar = (ITEM9*)HeaderA2->pItems; itemHeaderVar != nullptr; itemHeaderVar = itemHeaderVar->pNext)
				{
					// todo strcpy
					recursiveVar3->pID = _strdup(itemHeaderVar->pID);
					recursiveVar3->Code = itemHeaderVar->Code;
					recursiveVar3->pDate = itemHeaderVar->pDate;
					if (itemHeaderVar->pNext != nullptr)
					{
						ITEM9* nextLine3 = new ITEM9();
						recursiveVar3->pNext = nextLine3;
						recursiveVar3 = nextLine3;
					}
				}
			}
		}
	}

	void add_HD(ITEM9* item) {

		if (item == nullptr) { return; }

		find_ID(item->pID);

		for (HEADER_D* headerVariable = this->structure; headerVariable != nullptr
			&& first >= headerVariable->cBegin; headerVariable = headerVariable->pNext)
		{
			if (first == headerVariable->cBegin)
			{
				add_HA(headerVariable, item);
				return;
			}
		}
		// does not exist
		HEADER_D* headDV = new headerD();
		headDV->cBegin = first;
		if (this->structure == nullptr)
		{ //first
			headDV->pNext = this->structure;
			this->structure = headDV;
			add_HA(headDV, item);
			return;
		}
		else if (first < this->structure->cBegin) { // just first
			headDV->pNext = this->structure;
			this->structure->pPrior = headDV;
			this->structure = headDV;
			add_HA(headDV, item);
			return;
		}

		HEADER_D* headerVariable = this->structure;
		for (headerVariable; headerVariable != nullptr && first > headerVariable->cBegin; headerVariable = headerVariable->pNext)
		{ // if item in last
			if (headerVariable->pNext == nullptr)
			{
				headDV->pPrior = headerVariable;
				headerVariable->pNext = headDV;
				add_HA(headDV, item);
				return;
			}
		}
		headDV->pPrior = headerVariable->pPrior;
		headDV->pNext = headerVariable;
		headerVariable->pPrior->pNext = headDV;
		headerVariable->pPrior = headDV;
		add_HA(headDV, item);
	}

	void add_HA(HEADER_D* headerVar, ITEM9* item)
	{
		for (HEADER_A* otheHeaderV = headerVar->pHeaderA; otheHeaderV != nullptr
			&& second >= otheHeaderV->cBegin; otheHeaderV = otheHeaderV->pNext)
		{
			if (second == otheHeaderV->cBegin) {
				add_item(otheHeaderV, item);
				return;
			}
		}
		// does not
		HEADER_A* headDV = new headerA(); //create new header
		headDV->cBegin = second;
		if (headerVar->pHeaderA == nullptr)
		{ //first only
			headDV->pNext = headerVar->pHeaderA;
			headerVar->pHeaderA = headDV;
			add_item(headDV, item);
			return;
		}
		else if (second < headerVar->pHeaderA->cBegin)
		{ // just first
			HEADER_A* temp = headerVar->pHeaderA;
			headDV->pNext = temp;
			headerVar->pHeaderA = headDV;
			add_item(headDV, item);
			return;
		}

		HEADER_A* otheHeaderV = headerVar->pHeaderA;
		HEADER_A* prevpp = otheHeaderV;
		for (otheHeaderV; otheHeaderV != nullptr && second > otheHeaderV->cBegin; otheHeaderV = otheHeaderV->pNext) { //add to end
			if (otheHeaderV->pNext == nullptr) {
				otheHeaderV->pNext = headDV;
				add_item(headDV, item);
				return;
			}
			prevpp = otheHeaderV;
		}
		// add to middle
		headDV->pNext = otheHeaderV;
		prevpp->pNext = headDV;
		add_item(headDV, item);
	}

	void add_item(HEADER_A* otheHeaderV, ITEM9* item)
	{

		for (ITEM9* itemHeaderVar = (ITEM9*)otheHeaderV->pItems; itemHeaderVar != nullptr; itemHeaderVar = itemHeaderVar->pNext) {
			if (!strcmp(itemHeaderVar->pID, item->pID)) {
				cout << ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET " -------- Item " << item->pID << " already exists!" << endl;
				return;
			}
		}

		ITEM9* itemHeaderVar = (ITEM9*)otheHeaderV->pItems;
		for (itemHeaderVar; itemHeaderVar != nullptr; itemHeaderVar = itemHeaderVar->pNext) { // goes to the last item
			if (itemHeaderVar->pNext == nullptr) { break; }
		}
		if (itemHeaderVar == nullptr) { otheHeaderV->pItems = item; }
		else { itemHeaderVar->pNext = item; }
	}

	bool valid_id(char* pID)
	{
		if (pID == nullptr) {
			printf(ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET " -------- parameters are empty\n");
			return false;
		}

		if (!isupper(*pID)) {
			printf(ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET " -------- Identifier \"%s\" is in Wrong Format\n", pID);;
			return false;
		}
		char* letter;
		for (letter = pID + 1; isalpha(*letter) || '-' == *letter; letter++);
		if (!isspace(*letter)) {
			printf(ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET " -------- Identifier \"%s\" is in Wrong Format\n", pID);;
			return false;
		}
		letter++;
		if (!isupper(*letter)) {
			printf(ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET " -------- Identifier \"%s\" is in Wrong Format\n", pID);;
			return false;
		}

		for (letter++; isalpha(*letter) || '-' == *letter; letter++);
		if ('\0' != *letter) {
			printf(ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET " -------- Identifier \"%s\" is in Wrong Format\n", pID);;
			return false;
		}

		return true;
	}

	void remove_item(char* pID) {

		if (!valid_id(pID)) { return; }

		find_ID(pID);

		HEADER_D* headerVar = this->structure;
		for (headerVar; headerVar != nullptr && headerVar->cBegin <= first; headerVar = headerVar->pNext)
		{
			if (first == headerVar->cBegin)
			{
				HEADER_A* HeaderA, * prevpp = headerVar->pHeaderA;
				for (HeaderA = headerVar->pHeaderA; HeaderA != nullptr && HeaderA->cBegin <= second; HeaderA = HeaderA->pNext)
				{

					if (second == HeaderA->cBegin)
					{
						ITEM9* itemHeaderVar, * prevppp = (ITEM9*)HeaderA->pItems;

						for (itemHeaderVar = (ITEM9*)HeaderA->pItems; itemHeaderVar != nullptr; itemHeaderVar = itemHeaderVar->pNext)
						{
							if (!strcmp(itemHeaderVar->pID, pID))
							{

								ITEM9* compare = (ITEM9*)HeaderA->pItems; //first item to compare 
								if (itemHeaderVar->pID == compare->pID && itemHeaderVar->pNext == nullptr)
								{
									remove_header_a(prevpp, headerVar, second);
								}
								else if (itemHeaderVar->pID == compare->pID)
								{ //remove first
									HeaderA->pItems = itemHeaderVar->pNext;
								}
								else if (itemHeaderVar->pNext == nullptr)
								{ //last 
									prevppp->pNext = nullptr;
								}
								else {
									prevppp->pNext = itemHeaderVar->pNext;
								}
								itemHeaderVar->pID = nullptr;
								itemHeaderVar = nullptr;
								return;
							}

							prevppp = itemHeaderVar;
						}
					}

					prevpp = HeaderA;
				}
			}
		}

		cout << ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET " -------- Item " << pID << " doesn't exist" << endl;
		return;
	}
	void remove_header_a(HEADER_A* prev, HEADER_D* headerVar, char C) {
		if (prev == headerVar->pHeaderA && prev->pNext == nullptr)
		{// removing HEADER_D entirely if there is no previous variable
			remove_header_d(headerVar);
			prev = nullptr;
		}
		else if (prev == headerVar->pHeaderA && prev->cBegin == C) { // remove first line
			headerVar->pHeaderA = prev->pNext;
			prev = nullptr;
		}
		else {
			// remove anything other than those two conditions above
			HEADER_A* remove = prev->pNext;
			prev->pNext = remove->pNext;
			remove = nullptr;
		}

	}
	void remove_header_d(HEADER_D* headerVar)
	{
		if (headerVar == this->structure) { // first line 
			this->structure = headerVar->pNext;
			headerVar = nullptr;
		}
		else if (headerVar->pNext == nullptr) { //last line
			headerVar->pPrior->pNext = nullptr;
			headerVar = nullptr;
		}
		else {
			// middle
			headerVar->pPrior->pNext = headerVar->pNext;
			headerVar->pNext->pPrior = headerVar->pPrior;
			headerVar = nullptr;
		}
	}
};

int main()
{
	cout << "\n----------" ANSI_COLOR_GREEN "Step 1-3" ANSI_COLOR_RESET"----------\n\n" << endl;
	DataStructure* printData = new DataStructure;

	for (int i = 0; i < 10; i++)
	{
		*printData += (ITEM9*)GetItem(9);
	}
	cout << *printData << endl << endl;

	cout << "\n----------" ANSI_COLOR_GREEN "Step 4" ANSI_COLOR_RESET"----------\n\n" << endl;
	cout << "Items total: " << printData->GetItemsNumber() << endl << endl;


	cout << "\n----------" ANSI_COLOR_GREEN "Step 5" ANSI_COLOR_RESET"----------\n\n" << endl;


	ITEM9* lightCyan = printData->GetItem((char*)"Light Cyan");
	cout << lightCyan->pID << ' ' << lightCyan->Code << ' ' << lightCyan->pDate << endl << endl;




	cout << "\n----------" ANSI_COLOR_GREEN "Step 6" ANSI_COLOR_RESET"----------\n\n" << endl;
	ITEM9* X_X = printData->GetItem((char*)"X X");

	if (X_X == NULL) 
	{
		cout << ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET " -------- Item does not exist" << endl << endl; 
	}

	cout << "\n----------" ANSI_COLOR_GREEN "Step 7" ANSI_COLOR_RESET"----------\n\n" << endl;
	DataStructure* copy = new DataStructure(*printData);
	cout << *copy << endl << endl;

	cout << "\n----------" ANSI_COLOR_GREEN "Step 8" ANSI_COLOR_RESET"----------\n\n" << endl;
	*printData -= (char*)"Banana Mania";
	*printData -= (char*)"Persian Green";
	*printData -= (char*)"Vegas Gold";
	cout << *printData << endl << endl;

	cout << "\n----------" ANSI_COLOR_GREEN "Step 9" ANSI_COLOR_RESET"----------\n\n" << endl;
	cout << (*copy == *printData) << endl;

	cout << "\n----------" ANSI_COLOR_GREEN "Step 10-11" ANSI_COLOR_RESET"----------\n\n" << endl;
	printData->Write((char*)"file.txt");

	DataStructure* copy2 = new DataStructure((char*)"file.txt");
	cout << *copy2 << endl << endl;
	cout << (*copy2 == *printData) << endl << endl;

	cout << "\n----------" ANSI_COLOR_GREEN "Step 12" ANSI_COLOR_RESET"----------\n\n" << endl;
	copy2 = copy;
	cout << *copy2 << endl << endl;

	return 0;
}