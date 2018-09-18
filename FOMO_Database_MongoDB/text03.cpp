// text03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <String>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace std;
using namespace rapidjson;



string storeDrink(string name, float cost, bool availability) {
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_oid_t oid;
	bson_t *document;
	char cid[25];

	mongoc_init();
	client = mongoc_client_new("mongodb://DavidGarven:database123@ds113636.mlab.com:13636/fomodb");
	collection = mongoc_client_get_collection(client, "fomodb", "drinks");
	document = bson_new();
	bson_oid_init(&oid, NULL);
	BSON_APPEND_OID(document, "_id", &oid);
	BSON_APPEND_UTF8(document, "name", name.c_str());
	BSON_APPEND_DOUBLE(document, "cost", cost);
	BSON_APPEND_BOOL(document, "availability", availability);
	bson_oid_to_string(&oid, cid);
	string id = cid;

	if(!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, document, NULL, &error)) {
		fprintf(stderr, "%s\n", error.message);
	}

	bson_destroy(document);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
	mongoc_cleanup();

	return id;
}

string storeTransaction(string payer, string payee, float amount) {
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_oid_t oid;
	bson_t *document;
	string id;
	char cid[25];

	mongoc_init();

	client = mongoc_client_new("mongodb://DavidGarven:database123@ds113636.mlab.com:13636/fomodb");
	collection = mongoc_client_get_collection(client, "fomodb", "transactions");


	document = bson_new();
	bson_oid_init(&oid, NULL);
	BSON_APPEND_OID(document, "_id", &oid);
	BSON_APPEND_UTF8(document, "payer", payer.c_str());
	BSON_APPEND_UTF8(document, "payee", payee.c_str());
	BSON_APPEND_DOUBLE(document, "amount", amount);
	bson_oid_to_string(&oid, cid);
	id = cid;

	if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, document, NULL, &error)) {
		fprintf(stderr, "%s\n", error.message);
	}

	bson_destroy(document);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
	mongoc_cleanup();

	return id;
}

string storeUser(string name, string email, string address, int age, float tab) {
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_oid_t oid;
	bson_t *document;
	string id;
	char cid[25];

	mongoc_init();

	client = mongoc_client_new("mongodb://DavidGarven:database123@ds113636.mlab.com:13636/fomodb");
	collection = mongoc_client_get_collection(client, "fomodb", "users");


	document = bson_new();
	bson_oid_init(&oid, NULL);
	BSON_APPEND_OID(document, "_id", &oid);
	BSON_APPEND_UTF8(document, "name", name.c_str());
	BSON_APPEND_UTF8(document, "email", email.c_str());
	BSON_APPEND_UTF8(document, "address", address.c_str());
	BSON_APPEND_INT32(document, "age", age);
	BSON_APPEND_DOUBLE(document, "tab", tab);
	bson_oid_to_string(&oid, cid);
	id = cid;

	if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, document, NULL, &error)) {
		fprintf(stderr, "%s\n", error.message);
	}

	bson_destroy(document);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
	mongoc_cleanup();

	return id;
}

string storeOwner(string name, string email, string location, vector<string> drinks, string transactions) {
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_oid_t oid;
	bson_t *document;
	bson_t child;
	string id;
	char cid[25];

	mongoc_init();

	client = mongoc_client_new("mongodb://DavidGarven:database123@ds113636.mlab.com:13636/fomodb");
	collection = mongoc_client_get_collection(client, "fomodb", "owners");


	document = bson_new();
	bson_oid_init(&oid, NULL);
	BSON_APPEND_OID(document, "_id", &oid);
	BSON_APPEND_UTF8(document, "name", name.c_str());
	BSON_APPEND_UTF8(document, "email", email.c_str());
	BSON_APPEND_UTF8(document, "location", location.c_str());
	BSON_APPEND_ARRAY_BEGIN(document, "drinks", &child);
	for (int i = 0; i<drinks.size(); i++) {
		string title = "drink" + i;
		string out = drinks[i];
		BSON_APPEND_UTF8(&child, title.c_str(), out.c_str());
	}
	bson_append_array_end(document, &child);
	BSON_APPEND_UTF8(document, "transactions", transactions.c_str());
	bson_oid_to_string(&oid, cid);
	id = cid;

	if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, document, NULL, &error)) {
		fprintf(stderr, "%s\n", error.message);
	}

	bson_destroy(document);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
	mongoc_cleanup();

	return id;
}


string getDrink(string id) {
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;
	bson_oid_t oid;
	char *str;
	string drink;
	bson_oid_init_from_string(&oid, id.c_str());
	mongoc_init();

	client = mongoc_client_new("mongodb://DavidGarven:database123@ds113636.mlab.com:13636/fomodb");
	collection = mongoc_client_get_collection(client, "fomodb", "drinks");
	query = bson_new();
	BSON_APPEND_OID(query, "_id", &oid);
	cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
	mongoc_cursor_next(cursor, &doc);
	str = bson_as_json(doc, NULL);
	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
	drink = str;
	return drink;
}

string getTransaction(string id) {
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;
	bson_oid_t oid;
	char *str;
	string transaction;
	bson_oid_init_from_string(&oid, id.c_str());
	mongoc_init();

	client = mongoc_client_new("mongodb://DavidGarven:database123@ds113636.mlab.com:13636/fomodb");
	collection = mongoc_client_get_collection(client, "fomodb", "transactions");
	query = bson_new();
	BSON_APPEND_OID(query, "_id", &oid);
	cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
	mongoc_cursor_next(cursor, &doc);
	str = bson_as_json(doc, NULL);
	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
	transaction = str;
	return transaction;
}

string getUser(string id) {
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;
	bson_oid_t oid;
	char *str;
	string user;
	bson_oid_init_from_string(&oid, id.c_str());
	mongoc_init();

	client = mongoc_client_new("mongodb://DavidGarven:database123@ds113636.mlab.com:13636/fomodb");
	collection = mongoc_client_get_collection(client, "fomodb", "users");
	query = bson_new();
	BSON_APPEND_OID(query, "_id", &oid);
	cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
	mongoc_cursor_next(cursor, &doc);
	str = bson_as_json(doc, NULL);

	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
	user = str;
	return user;
}

string getOwner(string id) {
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;
	bson_oid_t oid;
	char *str;
	string owner;
	bson_oid_init_from_string(&oid, id.c_str());
	mongoc_init();

	client = mongoc_client_new("mongodb://DavidGarven:database123@ds113636.mlab.com:13636/fomodb");
	collection = mongoc_client_get_collection(client, "fomodb", "owners");
	query = bson_new();
	BSON_APPEND_OID(query, "_id", &oid);
	cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
	mongoc_cursor_next(cursor, &doc);
	str = bson_as_json(doc, NULL);
	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
	owner = str;
	return owner;
}

void parseDrink(string id, string &name, bool &availability, double &cost) {
	string json = getDrink(id);
	Document d;
	if (d.Parse<0>(json.c_str()).HasParseError()) {
		cout << "if" << endl;
		return;
	}
	assert(d.HasMember("name"));
	assert(d["name"].IsString());
	name = d["name"].GetString();
	assert(d.HasMember("availability"));
	assert(d["availability"].IsBool());
	availability = d["availability"].GetBool();
	assert(d.HasMember("cost"));
	assert(d["cost"].IsDouble());
	cost = d["cost"].GetDouble();
	return;
}

void parseTransaction(string id, string &payer, string &payee, double &amount) {
	string json = getTransaction(id);
	Document d;
	if (d.Parse<0>(json.c_str()).HasParseError()) {
		cout << "if" << endl;
		return;
	}
	assert(d.HasMember("payer"));
	assert(d["payer"].IsString());
	payer = d["payer"].GetString();
	assert(d.HasMember("payee"));
	assert(d["payee"].IsString());
	payee = d["payee"].GetString();
	assert(d.HasMember("amount"));
	assert(d["amount"].IsDouble());
	amount = d["amount"].GetDouble();
	return;
}

void parseUser(string id, string &name, string &email, string &address, int &age, double &tab) {
	string json = getUser(id);
	Document d;
	if (d.Parse<0>(json.c_str()).HasParseError()) {
		cout << "if" << endl;
		return;
	}
	assert(d.HasMember("name"));
	assert(d["name"].IsString());
	name = d["name"].GetString();
	assert(d.HasMember("email"));
	assert(d["email"].IsString());
	email = d["email"].GetString();
	assert(d.HasMember("address"));
	assert(d["address"].IsString());
	address = d["address"].GetString();
	assert(d.HasMember("age"));
	assert(d["age"].IsInt);
	age = d["age"].GetInt();
	assert(d.HasMember("tab"));
	assert(d["tab"].IsDouble());
	tab = d["tab"].GetDouble();
	return;
}

void parseOwner(string id, string &name, string &email, string &location, string(&drinks)[3], string &transactions) {
	string json = getOwner(id);
	Document d;
	if (d.Parse<0>(json.c_str()).HasParseError()) {
		cout << "if" << endl;
		return;
	}
	assert(d.HasMember("name"));
	assert(d["name"].IsString());
	name = d["name"].GetString();
	assert(d.HasMember("email"));
	assert(d["email"].IsString());
	email = d["email"].GetString();
	assert(d.HasMember("location"));
	assert(d["location"].IsString());
	location = d["location"].GetString();
	assert(d.HasMember("drinks"));
	assert(d["drinks"].IsArray());
	const Value& a = d["drinks"];
	for (SizeType i = 0; i < 3; i++) {
		drinks[i] = a[i].GetString();
	}
	assert(d.HasMember("transactions"));
	assert(d["transactions"].IsString());
	transactions = d["transactions"].GetString();
	return;
}

int main() {
	string name;
	bool availability;
	double cost;
	string id_drink = "5a1631b51f475c07a4003ff2";

	string payer;
	string payee;
	double amount;
	string id_transaction = "5a1769301f475c2380003112";

	string user_name;
	string user_email;
	string address;
	int age;
	double tab;
	string id_user = "5a19f2d41f475c0d58004972";

	string owner_name;
	string owner_email;
	string location;
	string drinks[3];
	string transactions;
	string id_owner = "5a19f1ed1f475c015c001242";

	parseDrink(id_drink, name, availability, cost);
	parseTransaction(id_transaction, payer, payee, amount);
	parseUser(id_user, user_name, user_email, address, age, tab);
	parseOwner(id_owner, owner_name, owner_email, location, drinks, transactions);

	cout << "Drink information:" << endl;
	cout << "Name: " + name << endl;
	cout << "Available (Yes = 1/No = 0): ";
	cout << availability << endl;
	cout << "Cost: ";
	cout << cost << endl;
	cout << "Transaction information:" << endl;
	cout << "Payer: " + payer << endl;
	cout << "Payee: " + payee << endl;
	cout << "Amount: ";
	cout << amount << endl;
	cout << "User information:" << endl;
	cout << "Name: " + user_name << endl;
	cout << "Email: " + user_email << endl;
	cout << "Address: " + address << endl;
	cout << "Age: ";
	cout << age << endl;
	cout << "Tab: ";
	cout << tab << endl;
	cout << "Owner information:" << endl;
	cout << "Name: " + owner_name << endl;
	cout << "Email: " + owner_email << endl;
	cout << "Location: " + location << endl;
	cout << "Available drinks: ";
	for (int i = 0; i < 3; i++)
		cout << drinks[i] + " ";
	cout << "\nTransactions: " + transactions << endl;


	return 0;
}



/*int main() {
	vector<string> vectr(3);
	vectr = { "Vodka", "Rum", "Beer" };
	string id = storeUser("Scotty", "noMoney@studentlife.com", "64 Abbey Road", 21, 0);
	cout << id << endl;
	return 0;
} */


	