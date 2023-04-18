#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>


class MyDatabase {
private:
	bool connect() {
		if (!c) {
			try {
				c = new pqxx::connection("host = localhost "
					"port = 5432 "
					"dbname = Lesson_5 "
					"user = postgres "
					"password = Tanusha7392 "
				);
			}
			catch (pqxx::sql_error e) {
				std::cout << "SQL error" << e.what() << "\n";
			}
			catch (const std::exception ex) {
				std::cout << "General error" << ex.what() << "\n";
			}
		} return c->is_open();
	}
	int sql(const std::string& str) {
		if (!c || !c->is_open()) {
			if (!connect()) {
				std::cout << "Failed to connect to the database\n";
				return -1;
			}
		}
		pqxx::work tx{ *c };
		try {
			pqxx::result result = tx.exec(str);
			tx.commit();
			return result.affected_rows();
		}
		catch (pqxx::sql_error e) {
			std::cout << "SQL error" << e.what() << "\n";
		}
		catch (const std::exception ex) {
			std::cout << "General error" << ex.what() << "\n";
		}
	}
public:
	MyDatabase() = default;
	MyDatabase(const MyDatabase&) = delete;
	MyDatabase& operator=(const MyDatabase&) = delete;
	int Create() {
		return sql("CREATE TABLE IF NOT EXISTS Users(UserId SERIAL PRIMARY KEY, FirstName VARCHAR(40) NOT NULL, LastName VARCHAR(40) NOT NULL);"
			"CREATE TABLE IF NOT EXISTS Emails(UserId INTEGER NOT NULL REFERENCES Users(UserId) ON DELETE CASCADE, Email VARCHAR(50) NOT NULL UNIQUE, CONSTRAINT Email_db PRIMARY KEY(UserId, Email));"
			"CREATE TABLE IF NOT EXISTS Phones(UserId INTEGER NOT NULL REFERENCES Users(UserId) ON DELETE CASCADE, PhoneNumber BIGINT NOT NULL UNIQUE, CONSTRAINT Phone_db PRIMARY KEY(UserId, PhoneNumber));");
	}
	int AddUsers(const std::string& FName, const std::string& LName) {
		return sql("INSERT INTO Users (FirstName, LastName) VALUES ('" + tx.esc(FName) + "', '" + tx.esc(LName) + "');");
	}
	int AddPhone(const int UsId, const long long int Phone) { 
		return sql("INSERT INTO Phones (UserId, PhoneNumber) VALUES (" + tx.esc(std::to_string(UsId)) + ", " + tx.esc(std::to_string(Phone)) + "); ");
	}
	int ChangeData(const int UsId, const std::string& FName, const std::string& LName) {
		return sql("UPDATE Users SET FirstName = '" + tx.esc(FName) + "', LastName = '" + tx.esc(LName) + "'" + "WHERE UserId = " + tx.esc(std::to_string(UsId)));
	}
	int DeletePhone(const int UsId) {
		return sql("DELETE FROM Phones WHERE PhoneId = " + tx.esc(std::to_string(UsId)));
	}
	int DeletePhone(const int UsId, const long long int Number) {
		return sql("DELETE FROM Phones WHERE UserId = " + tx.esc(std::to_string(UsId)) + " AND PhoneNumber = " + tx.esc(std::to_string(Number)));
	}
	int DeleteUsers(const int UsId) {
		return sql("DELETE FROM Users WHERE UserId = " + tx.esc(std::to_string(UsId)));
	}
	int FindUsers(const std::string& FName, const std::string& LName, const std::string& email, const long long int phone) {
		if (!c || !c->is_open()) {
			if (!connect()) {
				std::cout << "Failed to connect to the database" << "\n";
				return -1;
			}
		}
		pqxx::work tx{ *c };
		std::string select = "SELECT Users.UserId, Users.FirstName, Users.LastName, CASE WHEN Emails.Email IS NULL THEN '' ELSE Emails.Email END, CASE WHEN Phones.PhoneNumber IS NULL THEN '' ELSE Phones.PhoneNumber::varchar END FROM Users LEFT JOIN Emails on Emails.UserId = Users.UserId LEFT JOIN Phones on Phones.UserId = Users.UserId";
		if (!FName.empty()) {
			select += " WHERE Users.FirstName = '" + tx.esc(FName) + "'";
		}
		if (!LName.empty()) {
				select += " WHERE Users.LastName = '" + tx.esc(LName) + "'";
		}
		if (!email.empty()) {
			select += " WHERE Emails.Email = '" + tx.esc(email) + "'";
		}
		if (phone != 0) {
			select += " WHERE Phones.PhoneNumber = " + tx.esc(std::to_string(phone));
		}
		try {

			for (auto [id, FirstName, LastName, email, phone] : tx.query<int, std::string, std::string, std::string, std::string>(select))
			{
				std::cout << std::to_string(id) << " " << FirstName << " " << LastName << " " << email << " " << phone << "\n";
			}
		}
		catch (pqxx::sql_error e) {	
			std::cout << "SQL error: " << e.what() << "\n";
			return -1;
		}
		catch (std::exception ex) {
			std::cout << "General error: " << ex.what() << "\n";
			return -1;
		}
	}
	pqxx::connection* c{ nullptr };

	~MyDatabase() {
		if (c) {
			c->close(); delete c;
		}
	}
};



int main() {
	SetConsoleOutputCP(CP_UTF8);
	MyDatabase MyDatabase;
	MyDatabase.Create();
	if (MyDatabase.AddUsers("Ivan", "Ivnov") > 0) {
		std::cout << "User Ivan Ivnov added\n";
	};
	if (MyDatabase.AddUsers("Ivan", "Smirnov")) {
		std::cout << "User Ivan Smirnov added\n";
	}
	if (MyDatabase.AddUsers("Sofia", "Petrova")) {
		std::cout << "User Sofia Petrova added\n";
	}
	if (MyDatabase.AddUsers("Maria", "Kotova")) {
		std::cout << "User Maria Kotova added\n";
	}
	if (MyDatabase.AddUsers("Pavel", "Petrov")) {
		std::cout << "User Pavel Petrov added\n";
	}
	if (MyDatabase.ChangeData(1, "Ivan", "Ivanov")) {
		std::cout << "Data for user with id = 1 has been changed\n";
	}
	if (MyDatabase.AddPhone(1, 11111111111)) {
		std::cout << "Phone number for user with id = 1 added\n";
	}
	if (MyDatabase.AddPhone(1, 22222222222)) {
		std::cout << "Phone number for user with id = 1 added\n";
	}
	if (MyDatabase.AddPhone(2, 33333333333)) {
		std::cout << "Phone number for user with id = 2 added\n";
	}
	if (MyDatabase.AddPhone(3, 44444444444)) {
		std::cout << "Phone number for user with id = 3 added\n";
	}
	if (MyDatabase.AddPhone(3, 55555555555)) {
		std::cout << "Phone number for user with id = 3 added\n";
	}
	MyDatabase.FindUsers("Ivan", "", "", 0);
	MyDatabase.FindUsers("", "", "", 44444444444);
	MyDatabase.DeletePhone(1, 11111111111);
	MyDatabase.DeleteUsers(5); 

	return 0;
}