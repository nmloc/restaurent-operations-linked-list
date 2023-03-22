#include "main.h"

class customer {
public:
    string name;
    int age;
    customer* next;
    customer* prev;
	customer(string name, int age) {
        this->name = name;
		this->age = age;
        next = nullptr;
		prev = nullptr;
    }
    
};

class queue {
public:
	int count;
    customer* head;
    customer* tail;

    queue() {
		count = 0;
        head = nullptr;
        tail = nullptr;
    }

	~queue() {
        customer* current = head;
        while (current != nullptr) {
            customer* next = current->next;
            delete current;
            current = next;
        }
		count = 0;
        head = nullptr;
        tail = nullptr;
    }

    void enqueue(string name, int age) {
        customer* newCustomer = new customer(name, age);
        if (head == nullptr) {
            head = newCustomer;
            tail = newCustomer;
        } else {
            tail->next = newCustomer;
            newCustomer->prev = tail;
            tail = newCustomer;
        }
		count++;
    }

    void dequeue() {
        if (head == nullptr) {
            return;
        }
        customer* temp = head;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        }
        delete temp;
		count--;
    }

	void print() {
        if (head == NULL) {
            return;
        }
        customer* temp = head;
        while (temp != NULL) {
            std::cout << temp->name << "\n";
            temp = temp->next;
        }
    }

	void print_reverse() {
        customer* current = tail;
        while (current != NULL) {
            cout << current->name << "\n";
            current = current->prev;
        }
    }

	void print_reverse(int num) {
        customer* current = tail;
		int count = 0;
        while (current != NULL && count < num) {
            cout << current->name << "\n";
			count++;
            current = current->prev;
        }
    }

	void swap(customer* a, customer* b) {
		string tempName = a->name;
		int tempAge = a->age;
		a->name = b->name;
		a->age = b->age;
		b->name = tempName;
		b->age = tempAge;
	}

	customer* findMax(customer* start, customer* end) {
		customer* max = start;
		customer* current = start->next;
		while (current != end->next) {
			if (current->age > max->age) {
				max = current;
			}
			current = current->next;
		}
		return max;
	}

	void selectionSort(int num) {
		if (head == nullptr || head->next == nullptr) {
			return;
		}

		customer* current = head;
		for (int i = 0; i < num && current != nullptr; i++) {
			customer* max = findMax(current, nullptr);
			swap(current, max);
			current = current->next;
		}
	}
};

//Support functions for reading command
void extract_word(string line, string& cmd, string& var2, string& var3, string& var4) {
	int space_count = 0;
	for (size_t i = 0; i < line.length(); i++) {
		if (isspace(line.at(i))) {
			space_count++;
			continue;
		}
		if (space_count == 0)
			cmd += line.at(i);
		else if (space_count == 1)
			var2 += line.at(i);
		else if (space_count == 2)
			var3 += line.at(i);
		else
			var4 += line.at(i);
	}
}

bool is_enough_age(string cmd, string var3, string var4) {
	if (cmd == "REG") {
		if (var4.empty())
			if (stoi(var3) < 16)
				return false;
			else
				return true;
		else
			if (stoi(var4) < 16)
				return false;
			else
				return true;
	}
	else if (cmd == "REGM") {
		if (stoi(var3) < 16)
			return false;
		else 
			return true;
	}
	return true;
}

//Support functions for restaurant
bool is_free(table* table) {
	return table->name == "" && table->age == 0 ? true : false;
}

bool is_full(table* recentTable) {
	table* last = recentTable;
	do {
		if (is_free(last))
			return false;
		last = last->next;
	} while (last != recentTable);

	return true;
}

int get_customer_num(table* recentTable) {
	table* last = recentTable;
	int count = 0;
	do {
		if (!is_free(last))
			count++;
		last = last->next;
	} while (last != recentTable);

	return count;
}

table* get_table_by_ID(int ID, table* recentTable) {
	table* last = recentTable;
	table* found = nullptr;
    do {
        if (last->ID == ID) {
            found = last;
        }
        last = last->next;
    } while (last != recentTable);

	return found;
}

void cleanup(table* table) {
	table->name = "";
	table->age = 0;
}

table* find_smallest_ID_free_table(table* current) {
    table* last = current;
    table* smallest_free_table = nullptr;
    do {
        if (is_free(last) && (smallest_free_table == nullptr || last->ID < smallest_free_table->ID)) {
            smallest_free_table = last;
        }
        last = last->next;
    } while (last != current);

    if (is_free(last) && (smallest_free_table == nullptr || last->ID < smallest_free_table->ID)) {
        smallest_free_table = last;
    }
    return smallest_free_table;
}

table* find_biggest_ID_free_table(table* current) {
    table* last = current;
    table* biggest_free_table = nullptr;
    do {
        if (is_free(last) && (biggest_free_table == nullptr || last->ID > biggest_free_table->ID)) {
            biggest_free_table = last;
        }
        last = last->next;
    } while (last != current);
    if (is_free(last) && (biggest_free_table == nullptr || last->ID > biggest_free_table->ID)) {
        biggest_free_table = last;
    }
    return biggest_free_table;
}

table* get_free_consecutive_tables_started_with_biggest_ID(int num, table* recentTable) {
	int maxID = -1;
    table* maxTable = nullptr;
    table* start = nullptr;
    int count = 0;
    table* last = recentTable;
    do {
        if (is_free(last)) {
            if (!start) {
                start = last;
            }
            count++;
            if (count == num + 1) {
                // found more than num continuous free tables
                // reset start and count to find another set of free tables
                start = start->next;
				last = start->next;
                count = 1;
            }
        } else {
            // reset start and count to find another set of free tables
            start = nullptr;
            count = 0;
        }
        if (count == num && start->ID > maxID) {
            // found num continuous free tables with higher ID than previous
            // update maxID and maxTable
            maxID = start->ID;
            maxTable = start;
        }
        last = last->next;
    } while (last != recentTable);

	maxTable = maxTable->next;
    return maxTable;
}

void merge_tables(table* headMerge, int numNodes, table* startMerge, table* recentTable) {
	startMerge = headMerge->next;

    table* last = headMerge;
    for (int i = 1; i < numNodes; i++) {
        last = last->next;
    }
    headMerge->next = last->next;
}

void unmerge_tables(table* headMerge, table* startMerge) {
	headMerge->next = startMerge;
}

void print_from_latest_customer(table* recentTable, string soon_to_newest_customer_list) {
	string delimiter = ", ";
	size_t pos = soon_to_newest_customer_list.rfind(delimiter);
	while (pos != std::string::npos) {
		string latestID = soon_to_newest_customer_list.substr(pos + delimiter.length());
		table* latestTable = get_table_by_ID(stoi(latestID), recentTable);
		cout << latestTable->name << "\n";
		pos = soon_to_newest_customer_list.rfind(delimiter, pos - 1);
	}
}

void remove_ID_from_string(string& str, int value) {
    string value_str = to_string(value);
    size_t pos = str.find(value_str);
    if (pos != std::string::npos) {
        str.erase(pos, value_str.length());
        if (pos > 0 && str.at(pos - 2) == ',') {
            str.erase(pos - 2, 2);
        } else if (pos < str.length() && str.at(pos) == ',') {
            str.erase(pos, 2);
        }
    }
}

//Command implementation
void REG_cmd(string var2, string var3, string var4, queue* q, table* &recentTable, string& soon_to_newest_customer_list) {
	// there is no free table
	if (is_full(recentTable)) {
		if (var4.empty()) {
			q->enqueue(var2, stoi(var3));
		}
		else {
			q->enqueue(var3, stoi(var4));
		}
	}
	// there are some free tables
	else {
		if (var4.empty()) {
			table* smallest_free_table = find_smallest_ID_free_table(recentTable);
			smallest_free_table->name = var2;
			smallest_free_table->age = stoi(var3);
			
			if (soon_to_newest_customer_list.length() > 0)
				soon_to_newest_customer_list = soon_to_newest_customer_list + ", " + to_string(smallest_free_table->ID);
			else
				soon_to_newest_customer_list += to_string(smallest_free_table->ID);

			// update recentTable pointer
			recentTable = smallest_free_table;
		}
		else {
			// Find the table with the given ID (var2)
			table* last = recentTable;
			do {
				if (last->ID == stoi(var2))
					break;
				last = last->next;
			} while (last != recentTable);

			if (is_free(last)) {
				last->name = var3;
				last->age = stoi(var4);
				
				if (soon_to_newest_customer_list.length() > 0)
					soon_to_newest_customer_list = soon_to_newest_customer_list + ", " + to_string(last->ID);
				else
					soon_to_newest_customer_list += to_string(last->ID);

				// update recentTable pointer
				recentTable = last;
			}
			else {
				// Find another free table if the given ID table was booked
				table* newLast = recentTable;
				int targetID = recentTable->ID;

				do {
					if ((targetID == newLast->ID) && is_free(newLast))
						break;
					targetID++;
					if (targetID == 15) targetID = 1;
					newLast = newLast->next;
				} while (recentTable != newLast);

				newLast->name = var3;
				newLast->age = stoi(var4);

				if (soon_to_newest_customer_list.length() > 0)
					soon_to_newest_customer_list = soon_to_newest_customer_list + ", " + to_string(newLast->ID);
				else
					soon_to_newest_customer_list += to_string(newLast->ID);

				// update recentTable pointer
				recentTable = newLast;
			}
		}
	}
}

void REGM_cmd(string var2, string var3, string var4, table* &recentTable, bool REGMinProgress, table* startMerge, table* headMerge, string& soon_to_newest_customer_list) {
	headMerge = get_free_consecutive_tables_started_with_biggest_ID(stoi(var4), recentTable);

	if (headMerge == nullptr || REGMinProgress == true) {
		return;
	}
	else {
		REGMinProgress = true;
		merge_tables(headMerge, stoi(var4), startMerge, recentTable);
		
		headMerge->name = var2;
		headMerge->age = stoi(var3);

		if (soon_to_newest_customer_list.length() > 0)
			soon_to_newest_customer_list = soon_to_newest_customer_list + ", " + to_string(headMerge->ID);
		else
			soon_to_newest_customer_list += to_string(headMerge->ID);

		// update recentTable pointer
		recentTable = headMerge;
	}
}

void CLE_cmd(string var2, table* &recentTable, bool REGMinProgress, table* &headMerge, table* &startMerge, string& soon_to_newest_customer_list) {
	int id = stoi(var2);
	table* foundTable = get_table_by_ID(id, recentTable);

	if (is_free(foundTable) || id < 0 || id > MAXSIZE)
		return;

	// bàn ghép
	if ((headMerge != nullptr) && (id == headMerge->ID)) {
		cleanup(headMerge);
		remove_ID_from_string(soon_to_newest_customer_list, foundTable->ID);
		unmerge_tables(headMerge, startMerge);

		// Update recentTable pointer
		recentTable = headMerge;

		// Update REGM cmd variables
		REGMinProgress = false;
		headMerge = nullptr;
		startMerge = nullptr;
	}
	// bàn đơn
	else {
		cleanup(foundTable);
		remove_ID_from_string(soon_to_newest_customer_list, foundTable->ID);

		// Update recentTable pointer
		recentTable = foundTable;
	}
}

void PS_cmd(string var2, table* recentTable, string soon_to_newest_customer_list, queue* q) {
	int num = stoi(var2);
	if (num < 1 || num > 2*MAXSIZE)
		return;

	int customerNum = get_customer_num(recentTable);
	if (customerNum == 0) {
		cout << "Empty" << "\n";
		return;
	}
	else if (var2 == "") {
		print_from_latest_customer(recentTable, soon_to_newest_customer_list);
		return;
	}
	else {
		if (num <= customerNum) {
			if (q->count < num) {
				q->print_reverse();
				int numLeft = num - q->count;
				string delimiter = ", ";
				size_t pos = soon_to_newest_customer_list.rfind(delimiter);
				int count = 0;

				while (pos != std::string::npos && count < numLeft) {
					string latestID = soon_to_newest_customer_list.substr(pos + delimiter.length());
					table* foundTable = get_table_by_ID(stoi(latestID), recentTable);
					cout << foundTable->name << "\n";
					pos = soon_to_newest_customer_list.rfind(delimiter, pos - 1);
					count++;
				}

				if (count < numLeft) {
					table* foundTable = get_table_by_ID(stoi(soon_to_newest_customer_list.substr(0, pos + delimiter.length())),recentTable);
					cout << foundTable->name << "\n";
				}
			}
		}
		else {
			string delimiter = ", ";
			size_t pos = soon_to_newest_customer_list.rfind(delimiter);
			int count = 0;

			while (pos != std::string::npos && count < num) {
				string latestID = soon_to_newest_customer_list.substr(pos + delimiter.length());
				table* foundTable = get_table_by_ID(stoi(latestID), recentTable);
				cout << foundTable->name << "\n";
				pos = soon_to_newest_customer_list.rfind(delimiter, pos - 1);
				count++;
			}

			if (count < num) {
				table* foundTable = get_table_by_ID(stoi(soon_to_newest_customer_list.substr(0, pos + delimiter.length())),recentTable);
				cout << foundTable->name << "\n";
			}
		}
	}
}

void PQ_cmd(string var2, table* recentTable, string soon_to_newest_customer_list, queue* q) {
	// reverse ordered string
	string str = soon_to_newest_customer_list;
    string delimiter = ", ";
    string token;
    size_t pos = 0;
    string reversed_str = "";

    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        reversed_str = token + ", " + reversed_str;
        str.erase(0, pos + delimiter.length());
    }
    reversed_str = str + ", " + reversed_str;
	reversed_str.erase(reversed_str.length() - 2, 2);
	//

	int num = stoi(var2);
	if (num < 1 || num > 2*MAXSIZE)
		return;

	if (q->count == 0) {
		cout << "Empty" << "\n";
		return;
	}
	else if (var2 == "") {
		if (q->count > 0)
			q->print();
	}
	else {
		if (q->count > 0) {
			if (q->count < num)
				q->print();
			else {
				print_from_latest_customer(recentTable, reversed_str);
			}
		}
		else {
			string delimiter = ", ";
			size_t pos = reversed_str.rfind(delimiter);
			int count = 0;

			while (pos != std::string::npos && count < num) {
				string latestID = reversed_str.substr(pos + delimiter.length());
				table* foundTable = get_table_by_ID(stoi(latestID), recentTable);
				cout << foundTable->name << "\n";
				pos = reversed_str.rfind(delimiter, pos - 1);
				count++;
			}

			if (count < num) {
				table* foundTable = get_table_by_ID(stoi(reversed_str.substr(0, pos + delimiter.length())),recentTable);
				cout << foundTable->name << "\n";
			}

			q->print();
		}
	}
}

void PT_cmd(table* recentTable) {
	int recentID = recentTable->ID;
	int targetID = recentID + 1;
	table* last = recentTable;
	
	cout << last->ID << '-' << last->name << "\n";
	while (true) {
		if (targetID > MAXSIZE)
			targetID = 1;
		 // main part của hàm này
		if (last->ID == targetID) {
			if ((!is_free(last))) 
				cout << last->ID << '-' << last->name << "\n";
				
			targetID++;
		}
		//

		if (targetID == recentID)
			return;

		last = last->next;
	}
}

void SQ_cmd(string var2, table* recentTable, queue* q) {
	int num = stoi(var2);
	if (num <= q->count) {
		q->selectionSort(num);
		q->print();
	}
	else if (q->count == 0)
		cout << "Empty" << "\n";
	else if (num > q->count && num <= MAXSIZE) {
		q->selectionSort(num);
		q->print();
	}
}

void simulate(string filename, restaurant* r)
{
	ifstream infile;
    infile.open(filename);
    // Kiểm tra xem file có được mở thành công hay không
    if (infile.is_open()) {
		
		queue *q = new queue();
		string line;

		// Variable for REGM cmd
		bool REGMinProgress = false;
		table* headMerge = nullptr;
		table* startMerge = nullptr;

		// Variable for PS cmd
		string soon_to_newest_customer_list = "";

		while (getline(infile, line)) {

			string cmd = "";
			string var2 = "";
			string var3 = "";
			string var4 = "";

			extract_word(line, cmd, var2, var3, var4);

			if (cmd == "REG") {
				if (!is_enough_age(cmd, var3, var4))
					continue;
				else {
					REG_cmd(var2, var3, var4, q, r->recentTable, soon_to_newest_customer_list);
				}
			}
			else if (cmd == "REGM") {
				if (!is_enough_age(cmd, var3, var4))
					continue;
				else {
					REGM_cmd(var2, var3, var4, r->recentTable, REGMinProgress, startMerge, headMerge, soon_to_newest_customer_list);
				}
			}
			else if (cmd == "CLE") {
				CLE_cmd(var2, r->recentTable, REGMinProgress, headMerge, startMerge, soon_to_newest_customer_list);
			}
			else if (cmd == "PS") {
				PS_cmd(var2, r->recentTable, soon_to_newest_customer_list, q);
			}
			else if (cmd == "PQ") {
				PQ_cmd(var2, r->recentTable, soon_to_newest_customer_list, q);
			}
			else if (cmd == "PT") {
				PT_cmd(r->recentTable);
			}
			else if (cmd == "SQ") {
				SQ_cmd(var2, r->recentTable, q);
			}
			else
				cout << "Invalid command!" << "\n";
		}
		delete q;
		infile.close();
    }
	else
		return;
}