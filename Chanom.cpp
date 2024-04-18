#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <conio.h>

using namespace std;

int stringToNumber(string str) {
	int number;
	stringstream ss(str);
	ss >> number;
	return number;
}

string numberToString(int num) {
	ostringstream os;
	os << num;
	string str = os.str();
	return str;
}

bool isNumber(string str) {
    for(int i=0; i<str.length(); i++) {
        if(isdigit(str[i]) == 0) 
			return false;
    }
    return true;
}

class Food {
	public:
		string foodId;
		string foodName;
		int quantity;
		int price;
		Food *nextFood;
		Food(string foodId, string foodName, int quantity, int price) {
			this->foodId = foodId;
			this->foodName = foodName;
			this->quantity = quantity;
			this->price = price;
			this->nextFood = NULL;
		}
};

class Menu {
	private:
		Food *firstMenu;
		Food *lastMenu;
		int count;
	public:
		Menu() {
			firstMenu = NULL;
			lastMenu = NULL;
			count = 0;
		}
		
		void addFood(string foodId, string foodName, int quantity, int price) {
			Food *temp = new Food(foodId, foodName, quantity, price);
			if(firstMenu == NULL) {
				firstMenu = temp;
				lastMenu = temp;
			} else {
				lastMenu->nextFood = temp;
				lastMenu = temp;
			}
			count++;
		}
		
		bool addNewFood(string foodName, int quantity, int price) {
			int lastId;
			if(lastMenu == NULL) {
				lastId = 1;
			} else {
				lastId = stringToNumber(lastMenu->foodId);
			}		 
			string id = numberToString(lastId+1);
			Food *temp = new Food(id, foodName, quantity, price);
			if(firstMenu == NULL) {
				firstMenu = temp;
				lastMenu = temp;
			} else {
				lastMenu->nextFood = temp;
				lastMenu = temp;
			}
			Menu::updateData();
		}
		
		void readData() {
			fstream readFile;
			readFile.open("menu.txt", ios::in);
			string line;
			string readId, readName, readQty, readPrice;
			int qty;
			float price;
			if(!readFile) {
				cout << "File not found!" << endl;
			} else {
				while( getline(readFile, line) ) {
					readId = line.substr(0, line.find(","));
					line.erase(0, line.find(",")+1);
					
					readName = line.substr(0, line.find(","));
					line.erase(0, line.find(",")+1);
					
					readPrice = line.substr(0, line.find(","));
					line.erase(0, line.find(",")+1);
					
					readQty = line;
			
					qty = stringToNumber(readQty);
					price = stringToNumber(readPrice);
					
					addFood(readId, readName, qty, price);
				}
			}	
			readFile.close();
		}
		
		void updateData() {
			ofstream updateFile("menu.txt");
     		Food *temp = firstMenu;
     		while(temp != NULL) {
     			updateFile << temp->foodId << "," << temp->foodName << "," 
					<< temp->price << "," << temp->quantity << "\n";
						
     			temp = temp->nextFood;	
			}
			updateFile.close();
		}
		
		void deleteFood(string foodId) {
			Food *temp = firstMenu;
    		Food *prev = NULL;
     
    		if(temp->foodId == foodId) {
        		firstMenu = temp->nextFood; 
        		delete temp;
    		} else {
    			while(temp != NULL) {
    				if(temp->foodId == foodId) {
    					prev->nextFood = temp->nextFood;
    					delete temp;
					}
       	 			prev = temp;
        			temp = temp->nextFood;
    			}
    		}
		}
				
		void editFoodQuantity(string foodId, int quantity) {
			Food *temp = firstMenu;
			while(temp != NULL) {
				if(temp->foodId == foodId) {
    				temp->quantity += quantity;	
					break;	
				}	
				temp = temp->nextFood;
			}
		}
		
		void searchMenu(string keyword) {
			int found = 0;
			int size;
			Food *temp = firstMenu;
			cout << "================================================" << endl;
			cout << setw(6) << left << "ID"
				<< setw(20) << left << "Name"
				<< setw(10) << left << "Price"
				<< "Qty" << endl;
			cout << "================================================" << endl;
			
			while(temp != NULL) {
				size = temp->foodName.length() - keyword.length();
				for(int i=0; i<=size; i++) {
					if(keyword == temp->foodName.substr(i, keyword.length()) ) {
						cout << setw(6) << left << temp->foodId
							<< setw(20) << left << temp->foodName
							<< setw(10) << left << temp->price
							<< temp->quantity << endl;
						found++;
						break;
					}
				}   					
    			temp = temp->nextFood;
			} 
			
			if(found == 0) {
				cout << "\t\tNot Found" << endl;
			} 
			cout << "================================================" << endl;
		}
		
		string getFoodName(string foodId) {
			Food *temp = firstMenu;
			while(temp != NULL) {
				if(temp->foodId == foodId) {
    				return temp->foodName;		
				}	
				temp = temp->nextFood;
			}
			return "-";
		}
		
		int getFoodPrice(string foodId) {
			Food *temp = firstMenu;
			while(temp != NULL) {
				if(temp->foodId == foodId) {
    				return temp->price;		
				}	
				temp = temp->nextFood;
			}
			return 0;
		}
		
		int getFoodQuantity(string foodId) {
			Food *temp = firstMenu;
			while(temp != NULL) {
				if(temp->foodId == foodId) {
    				return temp->quantity;		
				}	
				temp = temp->nextFood;
			}
			return 0;
		}
		
		void swapData(Food *temp) {
			Food *next = temp->nextFood;
			Food *current = new Food(temp->foodId, temp->foodName, temp->quantity, temp->price);
			current->nextFood = temp->nextFood->nextFood;
			if(lastMenu == next)
				lastMenu = current;
			
			temp->foodId = temp->nextFood->foodId;
			temp->foodName = temp->nextFood->foodName;
			temp->quantity = temp->nextFood->quantity;
			temp->price = temp->nextFood->price;
		
			temp->nextFood = current;
			delete next;
		}
		
		void sortAscendingByPrice() {
			Food *temp = NULL;
			for(int i=0; i<count; i++) {
				temp = firstMenu;				
				for(int j=0; j<count-i-1; j++) {
					if(temp->price > temp->nextFood->price) {
						swapData(temp);
					} 					
					temp = temp->nextFood;				
				} 				
			} 
		}
		
		void sortDescendingByPrice() {
			Food *temp = NULL;
			for(int i=0; i<count; i++) {
				temp = firstMenu;				
				for(int j=0; j<count-i-1; j++) {
					if(temp->price < temp->nextFood->price) {
						swapData(temp);
					} 					
					temp = temp->nextFood;				
				} 				
			} 
		}
				
		void showMenu() {
			Food *temp = firstMenu;
			cout << "================================================" << endl;
			cout << setw(6) << left << "ID"
				<< setw(20) << left << "Name"
				<< setw(10) << left << "Price"
				<< "Qty" << endl;
			cout << "================================================" << endl;
			while(temp != NULL) {
				cout << setw(6) << left << temp->foodId
					<< setw(20) << left << temp->foodName
					<< setw(10) << left << temp->price
					<< temp->quantity << endl;
				temp = temp->nextFood;
			}
			cout << "================================================" << endl;
		}
		
}; // Class Menu

class Order {
	public:
		int orderNumber;
		string phoneNumber;
		Food *firstMenu;
		Food *lastMenu;
		int totalPrice;
		int orderStatus;
		Order *nextOrder;
		
		Order() {
			firstMenu = NULL;
			lastMenu = NULL;
			totalPrice = 0;
			orderStatus = 0;	
		}
		
		void setPhoneNumber(string phone) {
			this->phoneNumber = phone;
		}
		
		void add(string foodId, int quantity) {
			Menu menu;
			menu.readData();
			if(quantity < 1 || menu.getFoodQuantity(foodId) < quantity) {
				cout << "Can not add" << endl;
				return;
			}	
			string foodName = menu.getFoodName(foodId);
			if(foodName == "-") {
				return;
			}
			int price = menu.getFoodPrice(foodId);		
			totalPrice += quantity * price;
			
			Food *food = firstMenu;
			while(food != NULL) {
				if(food->foodId == foodId) {
					food->quantity += quantity;
					return;
				} // same id
				food = food->nextFood;
			} // check same id
			
			Food *temp = new Food(foodId, foodName, quantity, price);
			if(firstMenu == NULL) {
				firstMenu = temp;
				lastMenu = temp;
			} else {
				lastMenu->nextFood = temp;
				lastMenu = temp;
			}
		}
		
		void deleteFood(string foodId) {
			Food *temp = firstMenu;
    		Food *prev = NULL;
    		if(temp == NULL) {
    			return;
			}
     
    		if(temp->foodId == foodId) {
        		firstMenu = temp->nextFood; 
        		delete temp;
    		} else {
    			while(temp != NULL) {
    				if(temp->foodId == foodId) {
    					prev->nextFood = temp->nextFood;
    					delete temp;
					}
       	 			prev = temp;
        			temp = temp->nextFood;
    			}
    		}
		}
		
		void confirmOrder() {
			if(firstMenu == NULL) {
				cout << "Confirm Order Failed";
				return;
			}
			int checkQty = 0;
			Menu menu;
			menu.readData();
			Food *temp = firstMenu;
			while(temp != NULL) {
				if(temp->quantity > menu.getFoodQuantity(temp->foodId) ) {
					checkQty = 1;
					deleteFood(temp->foodId);
				} 
				temp = temp->nextFood;
			}
			
			if(checkQty != 0) {
				cout << "Confirm Order Failed";
				return;
			}
			
			temp = firstMenu;
			while(temp != NULL) {
				menu.editFoodQuantity(temp->foodId, temp->quantity * -1);
				temp = temp->nextFood;
			}
			menu.updateData();
			
			fstream readFile;
			readFile.open("order.txt", ios::in);
			string line;
			if(!readFile) {
				this->orderNumber = 1;
			} else {		
				while( getline(readFile, line) ) {
													
				}
				string readOrderNumber = line.substr(0, line.find(","));
				this->orderNumber = stringToNumber(readOrderNumber);
			}
			updateOrderData();
			clearOrder();
			cout << "Confirm Order Success";		
		}
		
		void updateOrderData() {
			Food *temp = firstMenu;
			fstream updateFile;
			updateFile.open("order.txt", std::ios_base::app);
			int i = 0;
     		updateFile << this->orderNumber+1 << "," << this->phoneNumber << ",";
     		while(temp != NULL) {
     			updateFile << temp->foodId << "/" << temp->quantity << "/";
     			i++;
     			temp = temp->nextFood;
			}
			updateFile << "," << this->totalPrice << "," << "10" << "\n";		
			updateFile.close();
		}
 		
		void clearOrder() {			
			while(firstMenu != NULL) {	
				Food *temp = firstMenu;
				firstMenu = firstMenu->nextFood;		
				delete temp;
			}
		}
		
		void showDetail() {
			Food *temp = firstMenu;
			cout << "================================================" << endl;
			cout << setw(6) << left << "ID"
				<< setw(20) << left << "Name"
				<< setw(10) << left << "Price"
				<< "Qty" << endl;
			cout << "================================================" << endl;
			if(temp == NULL) {
				cout << "\t\tNo food in order" << endl;
			} else {
				while(temp != NULL) {
					cout << setw(6) << left << temp->foodId
						<< setw(20) << left << temp->foodName
						<< setw(10) << left << temp->price
						<< temp->quantity << endl;
					temp = temp->nextFood;
				}
			}
			
			cout << "================================================" << endl;
		}
}; // Class Order

class Customer {
	private:
		string phoneNumber;
		string password;
		string name;
		string address;
	public:
		Customer() {
			phoneNumber = "";
			name = "";
			address = "";
		}
		
		bool checkMember(string phoneNumber, string password) {
			fstream readFile;
			readFile.open("customer.txt", ios::in);
			if(!readFile) {			
				readFile.close();
				return false;
			} else {
				string line, phone, pass;
				while( getline(readFile, line) ) {
					phone = line.substr(0, line.find(",") );
					line.erase(0, line.find(",")+1);
					pass = line.substr(0, line.find(",") );
					if(phoneNumber == phone && password == pass) {
						this->phoneNumber = phoneNumber;
						this->password = password;
						line.erase(0, line.find(",")+1);
						this->name = line.substr(0, line.find(","));
						line.erase(0, line.find(",")+1);
						this->address = line;
						readFile.close();
						return true;
					}									
				}
				readFile.close();
				return false;
			}
		}
		
		bool checkRegister(string phone, string pass, string name, string addr) {
			if(isNumber(phone) == false || phone.length() != 10) {
				return false;
			}
			
			if(checkDataCorrect(phone) == false || checkDataCorrect(pass) == false) {
				return false;
			}
			if(checkDataCorrect(name) == false || checkDataCorrect(addr) == false) {
				return false;
			}
			fstream readFile;
			readFile.open("customer.txt", ios::in);
			if(!readFile) {
				cout << "Error file not found" << endl;			
				readFile.close();
				return false;
			} else {
				string line, readPhone;
				while( getline(readFile, line) ) {
					readPhone = line.substr(0, line.find(",") );
					if(readPhone == phone) {
						readFile.close();
						return false;
					}									
				}
				readFile.close();
				fstream updateFile;
				updateFile.open("customer.txt", std::ios_base::app);
				updateFile << phone << "," << pass << "," 
					<< name << "," << addr << "\n";
				updateFile.close();
				this->phoneNumber = phone;
				this->password = pass;
				this->name = name;
				this->address = addr;
				return true;
			}
		}
		
		void updateData() {
			fstream readFile;
			int count = 0;
			string line, data = "";
			string checkPhone;
			readFile.open("customer.txt", ios::in);
			if(!readFile) {			
				readFile.close();
				return;
			} else {
				while( getline(readFile, line) ) {
					data += line + "/";
					count++;												
				}				
				readFile.close();
				ofstream updateFile("customer.txt");
				for(int i=0; i<count; i++) {
					checkPhone = data.substr(0, data.find(","));
					if(checkPhone == this->phoneNumber) {
						updateFile << this->phoneNumber << "," << this->password << ","
							<< this->name << "," << this->address << "\n";						
					} else {
						updateFile << data.substr(0, data.find("/")) << "\n";
					}
					data.erase(0, data.find("/")+1);
				}
				updateFile.close();
			}
		}
		
		bool checkDataCorrect(string data) {
			int check = -1;
			check = data.find("/");
			if(check != -1) {
				return false;
			} else {
				return true;
			}
		}
		
		void updatePassword(string newPass) {
			if(checkDataCorrect(newPass) == true) {
				this->password = newPass;
				updateData();	
				cout << "Update Success";
				getch();	
			} else {
				cout << "Update Failed (Can not use /)" << endl;
				getch();
			}
		}
		
		void updateName(string newName) {
			if(checkDataCorrect(newName) == true) {
				this->name = newName;
				updateData();	
				cout << "Update Success";
				getch();
			} else {
				cout << "Update Failed (Can not use /)" << endl;
				getch();
			}			
		}
		
		void updateAddress(string newAddr) {
			if(checkDataCorrect(newAddr) == true) {
				this->address = newAddr;
				updateData();
				cout << "Update Success";
				getch();	
			} else {
				cout << "Update Failed (Can not use /)" << endl;
				getch();
			}
		}
		
		string getName() {
			return name;
		}
		
		string getAddress() {
			return address;
		}
}; // Class Customer

int main() {
	string phoneNumber;
	string password = "";
	string star = "";
    char charPass;
    string choice;
    string fname, lname, addr;
    Customer customer;
    Order order;
    
    landingPage:
    	system("cls");
    	fstream readFile;
		readFile.open("landingpage.txt", ios::in);
		string line;
		while( getline(readFile, line) ) {
			cout << line << endl;						
		}
		readFile.close();
		cout << "Enter: ";
		cin >> choice;
		if(choice == "1") {
			goto loginPage;
		} else if(choice == "2") {
			goto registerPage;
		} else if(choice == "3") {
			return 0;
		} else {
			goto landingPage;
		}
	
	registerPage:
		system("cls");
		cout <<"\t\t= = = = = = = = = = = = = = = = = = =\n"
			<<"\t\t|     Register for new customer     |\n"
			<<"\t\t= = = = = = = = = = = = = = = = = = =\n\n";
		cout <<"\t\tPhone number: ";
		cin >> phoneNumber;
		cout << "\t\tPassword: ";
		cin >> password;
		cout << "\t\tFirst name: ";
		cin >> fname;
		cout << "\t\tLast name: ";
		cin >> lname;
		getchar();
		cout << "\t\tAddress: ";
		getline(cin, addr);
		do {
			cout << "\t\tConfirm(y/n): ";
			cin >> choice;
		} while(choice != "y" && choice != "n");		
		if(choice == "y") {
			if(customer.checkRegister(phoneNumber, password, fname + " " + lname, addr) == true) {
				cout << "\t\tRegister Success";
				order.setPhoneNumber(phoneNumber);
				getch();
				goto homePage;
			} else {
				cout << "\t\tRegister Failed";
				getch();
				goto landingPage;
			}			 
		} else {
			goto landingPage;
		}	
	
	loginPage:
		password = "";
		star = "";
		system("cls");
		cout << "\t\t= = = = = = = = = = = = = = = = = = =\n"
			<< "\t\t|             Login stage           |\n"
			<< "\t\t= = = = = = = = = = = = = = = = = = =\n\n";
    	cout << "\t\tPhone number: ";
    	cin >> phoneNumber;
		cout << "\t\tPassword: ";
    	charPass = _getch();
    		
    	while(charPass != 13) {  			
        	if(charPass == 8 && star.length() != 0) {
            	star.resize(star.length() - 1);
            	cout << star;
            	password.resize(password.length() - 1);
        	} else if(charPass != 8) {
            	star = star + "*";
            	cout << star;
            	password += charPass;
        	}
        	charPass = _getch();
        	system("cls");
        	cout << "\t\t= = = = = = = = = = = = = = = = = = =\n"
				<< "\t\t|             Login stage           |\n"
				<< "\t\t= = = = = = = = = = = = = = = = = = =\n\n";
        	cout << "\t\tPhone number: " << phoneNumber << endl;
        	cout << "\t\tPassword: ";
    	} // input password
    	
		if(customer.checkMember(phoneNumber, password) == true) {
			cout << star << endl;
			cout << "\t\tLogin Success";
			order.setPhoneNumber(phoneNumber);
			getch();
			goto homePage;	
		} else {
			cout << star << endl;	
			cout << "\t\tLogin Failed";
			getch();
			goto landingPage;
		}
	
	homePage:
		system("cls");
		readFile.open("homepage.txt", ios::in);
		while( getline(readFile, line) ) {
			cout << line << endl;						
		}
		readFile.close();
		cout << "Enter: ";
		cin >> choice;
	
	if(choice == "6") {
		return 0;
	} // Exit
	
	if(choice == "1") {
		showMenuPage:
			system("cls");
			string keyword;
			Menu menu;
			menu.readData();
			menu.showMenu();
			cout << "1.Sort ascending by price" << endl;
			cout << "2.Sort descending by price" << endl;
			cout << "3.Search drinks by name" << endl;
			cout << "4.Back" << endl;
			cout << "================================================" << endl;
			cout << "Enter: ";
			cin >> choice;
			if(choice == "1") {
				system("cls");
				menu.sortAscendingByPrice();
				menu.showMenu();
				cout << "Press any key to continue";
				getch();
				goto showMenuPage;
			} else if(choice == "2") {
				system("cls");
				menu.sortDescendingByPrice();
				menu.showMenu();
				cout << "Press any key to continue";
				getch();
				goto showMenuPage;
			} else if(choice == "3") {
				system("cls");
				cout << "=============== Search Drinks ==============" << endl;
				cout << "Enter Name: ";
				cin >> keyword;
				system("cls");
				menu.searchMenu(keyword);	
				cout << "Press any key to continue";
				getch();
				goto showMenuPage;
			} else if(choice == "4") {
				goto homePage;
			} else {
				goto showMenuPage;
			}
			
	} else if(choice == "2") {
		addPage:
			system("cls");
			string id, again, qty;		
			order.showDetail();
			cout << "Enter ID: ";
			cin >> id;
			cout << "Enter Quantity: ";
			cin >> qty;
			if(isNumber(qty) == false) {
				order.add(id, 0);
			} else {
				order.add(id, stringToNumber(qty));
			}	
			
			do {	
				cout << "Again? (y/n): ";
				cin >> again;
			} while(again != "y" && again != "n");
				
			if(again == "y") {
				goto addPage;
			} else if(again == "n") {
				goto homePage;
			}
			
	} else if(choice == "3") {
		deletePage:
			system("cls");
			cout << "==================== Delete ====================" << endl;
			order.showDetail();
			string id;
			cout << "ID: ";
			cin >> id;
			order.deleteFood(id);
			goto homePage;
		
	} else if(choice == "4") {	
		confirmPage:
			system("cls");
			string confirm;
			cout << "================= Confirm Order ================" << endl;
			order.showDetail();
			cout << "1.Confirm" << endl;
			cout << "2.Back" << endl;
			cout << "================================================" << endl;
			cout << "Enter: ";
			cin >> confirm;
			if(confirm == "1") {
				order.confirmOrder();
				getch();
				goto homePage;
			} else if(confirm == "2") {
				goto homePage;
			} else {
				goto confirmPage;
			}
		
	} else if(choice == "5") {
		informationPage:
			system("cls");
			fname = customer.getName().substr(0, customer.getName().find(" "));
			lname = customer.getName();
			lname.erase(0, lname.find(" ")+1);
			cout << "================== Information ================" << endl;
			cout << "1.Frist Name: " << fname << endl;
			cout << "2.Last Name: " << lname << endl;
			cout << "3.Address: " << customer.getAddress() << endl;
			cout << "4.Password" << endl;
			cout << "5.Back" << endl;
	 		cout << "================================================" << endl;
	 		cout << "Enter: ";
	 		cin >> choice;
	 		if(choice == "1") {
	 			cout << "New First Name: ";
	 			cin >> fname;
	 			customer.updateName(fname + " " + lname);
	 			goto informationPage;
			} else if(choice == "2") {
				cout << "New Last Name: ";
	 			cin >> lname;
	 			customer.updateName(fname + " " + lname);
	 			goto informationPage;
			} else if(choice == "3") {
				cout << "New Address: ";
	 			cin >> addr;
	 			customer.updateAddress(addr);
	 			goto informationPage;
			} else if(choice == "4") {
				cout << "New Password: ";
	 			cin >> password;
	 			customer.updatePassword(password);
	 			goto informationPage;
			} else if(choice == "5") {
				goto homePage;
			} else {
				goto informationPage;
			}
		
	} else {
		goto homePage;
	}
		
	return 0;
} // main
