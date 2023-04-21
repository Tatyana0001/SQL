#include <iostream>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <Wt/WDate.h>
#include "windows.h"

class Stock;
class Sale {
public:
	int price = 0;
	std::string date_sale = "00.00.0000";
	Wt::Dbo::ptr<Stock> stock;
	int count = 0;

	template<class Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, price, "price");
		Wt::Dbo::field(a, date_sale, "date_sale");
		Wt::Dbo::belongsTo(a, stock, "stock");
		Wt::Dbo::field(a, count, "count");
	}
};

class Book;
class Shop; 

class Stock {   //ассортимент, склад
public:
	Wt::Dbo::ptr<Book> book;
	Wt::Dbo::ptr<Shop> shop;
	int count = 0;
	Wt::Dbo::collection<Wt::Dbo::ptr<Sale>> sales;

	template<class Action>
	void persist(Action& a) {
		Wt::Dbo::belongsTo(a, book, "book");
		Wt::Dbo::belongsTo(a, shop, "shop");
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::hasMany(a, sales, Wt::Dbo::ManyToOne, "stock");
	}
};
class Publisher;

class Book {
public:
	std::string title = "";
	Wt::Dbo::ptr<Publisher> publisher;
	Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stocks;

	template<class Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, title, "title");
		Wt::Dbo::belongsTo(a, publisher, "publisher");
		Wt::Dbo::hasMany(a, stocks, Wt::Dbo::ManyToOne, "book");
	}
};

class Publisher {
public:
	std::string name = "";
	Wt::Dbo::collection<Wt::Dbo::ptr<Book>> books;

	template<class Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, books, Wt::Dbo::ManyToOne, "publisher");
	}
};

class Shop {
public:
	std::string name = "";
	Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stocks;

	template<class Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, stocks, Wt::Dbo::ManyToOne, "shop");
	}
};


int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleOutputCP(CP_UTF8);
	try {
		std::string connectionString =
			"host=localhost"
			" port=5432"
			" dbname=Lesson_6"
			" user=postgres"
			" password = Tanusha7392";

		auto postgres = std::make_unique<Wt::Dbo::backend::Postgres>(connectionString);

		Wt::Dbo::Session session;
		session.setConnection(std::move(postgres));
		session.mapClass<Publisher>("publisher");
		session.mapClass<Book>("book");
		session.mapClass <Shop>("shop");
		session.mapClass <Stock>("stock");
		session.mapClass <Sale>("sale");


		session.createTables(); //Создаем таблицы

//Task 2:
		Wt::Dbo::Transaction transaction{ session };

		//Добавляем авторов
		std::unique_ptr<Publisher> publisher1{ new Publisher() };
		publisher1->name = "Margo";
		std::unique_ptr<Publisher> publisher2{ new Publisher() };
		publisher2->name = "John";
		std::unique_ptr<Publisher> publisher3{ new Publisher() };
		publisher3->name = "Joe";
		std::unique_ptr<Publisher> publisher4{ new Publisher() };
		publisher4->name = "Kate";
		Wt::Dbo::ptr<Publisher> publisherPtr1 = session.add(std::move(publisher1));
		Wt::Dbo::ptr<Publisher> publisherPtr2 = session.add(std::move(publisher2));
		Wt::Dbo::ptr<Publisher> publisherPtr3 = session.add(std::move(publisher3));
		Wt::Dbo::ptr<Publisher> publisherPtr4 = session.add(std::move(publisher4));

		//Добавляем книги
		std::unique_ptr<Book> book1{ new Book() };
		book1->title = "Love";
		std::unique_ptr<Book> book2{ new Book() };
		book2->title = "Health";
		std::unique_ptr<Book> book3{ new Book() };
		book3->title = "Sky";
		std::unique_ptr<Book> book4{ new Book() };
		book4->title = "Museum";
		std::unique_ptr<Book> book5{ new Book() };
		book5->title = "My little son";
		Wt::Dbo::ptr<Book> bookPtr1 = session.add(std::move(book1));
		Wt::Dbo::ptr<Book> bookPtr2 = session.add(std::move(book2));
		Wt::Dbo::ptr<Book> bookPtr3 = session.add(std::move(book3));
		Wt::Dbo::ptr<Book> bookPtr4 = session.add(std::move(book4));
		Wt::Dbo::ptr<Book> bookPtr5 = session.add(std::move(book5));

		//Добавляем авторов книг

		Wt::Dbo::ptr<Book> B1 = session.find<Book>().where("title = ?").bind("Love");
		Wt::Dbo::ptr<Publisher>P1 = session.find<Publisher>().where("name = ?").bind("Margo");
		B1.modify()->publisher = P1;

		Wt::Dbo::ptr<Book> B2 = session.find<Book>().where("id = ?").bind(2);
		Wt::Dbo::ptr<Publisher>P2 = session.find<Publisher>().where("name = ?").bind("John");
		B2.modify()->publisher = P2;

		Wt::Dbo::ptr<Book> B3 = session.find<Book>().where("title = ?").bind("Sky");
		Wt::Dbo::ptr<Publisher>P3 = session.find<Publisher>().where("name = ?").bind("Joe");
		B3.modify()->publisher = P3;

		Wt::Dbo::ptr<Book> B4 = session.find<Book>().where("id = ?").bind(4);
		Wt::Dbo::ptr<Publisher>P4 = session.find<Publisher>().where("name = ?").bind("Kate");
		B4.modify()->publisher = P4;

		Wt::Dbo::ptr<Book> B5 = session.find<Book>().where("id = ?").bind(5);
		Wt::Dbo::ptr<Publisher>P5 = session.find<Publisher>().where("id = ?").bind(2);
		B5.modify()->publisher = P5;

		//Добавляем магазины
		std::unique_ptr<Shop> shop1{ new Shop() };
		shop1->name = "BookShop1";
		std::unique_ptr<Shop> shop2{ new Shop() };
		shop2->name = "BookShop2";
		std::unique_ptr<Shop> shop3{ new Shop() };
		shop3->name = "BookShop3";
		Wt::Dbo::ptr<Shop> shopPtr1 = session.add(std::move(shop1));
		Wt::Dbo::ptr<Shop> shopPtr2 = session.add(std::move(shop2));
		Wt::Dbo::ptr<Shop> shopPtr3 = session.add(std::move(shop3));

		//Добавляем ассортимент, количество
		std::unique_ptr<Stock> stock1{ new Stock() };
		std::unique_ptr<Stock> stock2{ new Stock() };
		std::unique_ptr<Stock> stock3{ new Stock() };
		std::unique_ptr<Stock> stock4{ new Stock() };
		std::unique_ptr<Stock> stock5{ new Stock() };
		stock1->count = 5;
		stock2->count = 9;
		stock3->count = 2;
		stock4->count = 6;
		stock5->count = 3;
		Wt::Dbo::ptr<Stock> stockPtr1 = session.add(std::move(stock1));
		Wt::Dbo::ptr<Stock> stockPtr2 = session.add(std::move(stock2));
		Wt::Dbo::ptr<Stock> stockPtr3 = session.add(std::move(stock3));
		Wt::Dbo::ptr<Stock> stockPtr4 = session.add(std::move(stock4));
		Wt::Dbo::ptr<Stock> stockPtr5 = session.add(std::move(stock5));

		//Вводим данные book,shop в Stock
		Wt::Dbo::ptr<Stock> S1 = session.find<Stock>().where("id = ?").bind(1);
		Wt::Dbo::ptr<Stock> S2 = session.find<Stock>().where("id = ?").bind(2);
		Wt::Dbo::ptr<Stock> S3 = session.find<Stock>().where("id = ?").bind(3);
		Wt::Dbo::ptr<Stock> S4 = session.find<Stock>().where("id = ?").bind(4);
		Wt::Dbo::ptr<Stock> S5 = session.find<Stock>().where("id = ?").bind(5);
		Wt::Dbo::ptr<Book>b1 = session.find<Book>().where("id = ?").bind(1);
		Wt::Dbo::ptr<Book>b2 = session.find<Book>().where("id = ?").bind(2);
		Wt::Dbo::ptr<Book>b3 = session.find<Book>().where("id = ?").bind(3);
		Wt::Dbo::ptr<Book>b4 = session.find<Book>().where("id = ?").bind(4);
		Wt::Dbo::ptr<Book>b5 = session.find<Book>().where("id = ?").bind(5);
		Wt::Dbo::ptr<Shop>s1 = session.find<Shop>().where("id = ?").bind(1);
		Wt::Dbo::ptr<Shop>s2 = session.find<Shop>().where("id = ?").bind(2);
		Wt::Dbo::ptr<Shop>s3 = session.find<Shop>().where("id = ?").bind(3);
		S1.modify()->book = b1;
		S1.modify()->shop = s1;
		S2.modify()->book = b2;
		S2.modify()->shop = s2;
		S3.modify()->book = b3;
		S3.modify()->shop = s3;
		S4.modify()->book = b4;
		S4.modify()->shop = s2;
		S5.modify()->book = b5;
		S5.modify()->shop = s1;

		//Добавляем Sale
		std::unique_ptr<Sale> sale1{ new Sale() };
		sale1->price = 450;
		sale1->date_sale = "01.05.2023";
		sale1->count = 5;
		Wt::Dbo::ptr<Sale> salePtr1 = session.add(std::move(sale1));
		std::unique_ptr<Sale> sale2{ new Sale() };
		sale2->price = 545;
		sale2->date_sale = "08.05.2023";
		sale2->count = 8;
		Wt::Dbo::ptr<Sale> salePtr2 = session.add(std::move(sale2));
		std::unique_ptr<Sale> sale3{ new Sale() };
		sale3->price = 850;
		sale3->date_sale = "23.05.2023";
		sale3->count = 12;
		Wt::Dbo::ptr<Sale> salePtr3 = session.add(std::move(sale3));
		std::unique_ptr<Sale> sale4{ new Sale() };
		sale4->price = 150;
		sale4->date_sale = "13.05.2023";
		sale4->count = 2;
		Wt::Dbo::ptr<Sale> salePtr4 = session.add(std::move(sale4));
		std::unique_ptr<Sale> sale5{ new Sale() };
		sale5->price = 555;
		sale5->date_sale = "01.06.2023";
		sale5->count = 23;
		Wt::Dbo::ptr<Sale> salePtr5 = session.add(std::move(sale5));

		//Вводим данные stock в Sale
		Wt::Dbo::ptr<Sale> Sl1 = session.find<Sale>().where("id = ?").bind(1);
		Wt::Dbo::ptr<Sale> Sl2 = session.find<Sale>().where("id = ?").bind(2);
		Wt::Dbo::ptr<Sale> Sl3 = session.find<Sale>().where("id = ?").bind(3);
		Wt::Dbo::ptr<Sale> Sl4 = session.find<Sale>().where("id = ?").bind(4);
		Wt::Dbo::ptr<Sale> Sl5 = session.find<Sale>().where("id = ?").bind(5);
		Sl1.modify()->stock = S1;
		Sl2.modify()->stock = S2;
		Sl3.modify()->stock = S3;
		Sl4.modify()->stock = S4;
		Sl5.modify()->stock = S5;

		transaction.commit();
		
		//Открыть после ввода данных. Поиск магазина по id
/*
		typedef Wt::Dbo::collection < Wt::Dbo::ptr<Publisher>> Publishers;
		int id;
		std::cout << "Enter publisher id: ";
		std::cin >> id;
		Wt::Dbo::ptr<Publisher> p = session.find<Publisher>().where("id = ?").bind(id);
		std::cout << p->name << ". ";
		std::cout << "Books are in the shops: ";
		typedef Wt::Dbo::collection < Wt::Dbo::ptr<Book>> Books;
		Books bo = session.query<Wt::Dbo::ptr<Book>>("select id from book id").where("publisher_id = ?").bind(id);
		for (auto& b : bo) {
			typedef Wt::Dbo::collection < Wt::Dbo::ptr<Stock>> Stocks;
			Stocks sk = session.find<Stock>("id").where("book_id = ?").bind(b);
			for (auto& s : sk) {
				typedef Wt::Dbo::collection < Wt::Dbo::ptr<Shop>> Shops;
				Shops shp = session.find<Shop>("name").where("id = ?").bind(s->shop);
				for (auto& n : shp) {
					std::cout << n->name << " ";
				}
			}
		}*/
	}
	catch (const Wt::Dbo::Exception& e) {
		std::cout << e.what() << "\n";
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
	}
	return 0;
}