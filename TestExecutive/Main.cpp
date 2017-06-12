#include "../Core.Datastructures/RecordStructure.h"
#include "../Core.Datastructures/DatabaseStructure.h"
#include "../Convert/Convert.h"
#include "../Core.StorageManager/STORAGEMANAGER.H"
#include "../Core.DBEngine/NoSQLDB.h"
#include "../Core.DBEngine/IQueryChainer.h"

void insertDashedLine() {
	cout << "-------------------------------------------------------------" << endl;
}

void DemonstrateRequirement3(NoSQLDB<string> & db, const DatabaseInfo & databaseInfo) {
	cout << endl << "Requirement 3 - Add and delete records.";
	cout << endl << "Adding 2 records";
	Record<string> record;
	record.setItem("interfaces");
	record.setCategory("package");
	record.setDescription("Contains Core Interfaces");
	record.setData("CoreInterfaces.h");

	db.addRecord(record.getItem(), record);

	Record<string> recordTwo;
	recordTwo.setItem("abc");
	recordTwo.setCategory("package");
	recordTwo.setDescription("Contains Core types");
	recordTwo.setData("DatabaseStructure.h");

	cout << endl << "Trying to add a record with same key as \"project1\"" << endl;

	db.addRecord("project1", recordTwo);

	cout << endl << "Trying to add a record with same key as \"project1\"" << endl;
	db.addRecord(recordTwo.getItem(), recordTwo);

	cout << endl << "Printing the newly added records";
	vector<string> keys;
	keys.push_back(record.getItem());
	keys.push_back(recordTwo.getItem());
	db.select(SelectObjectType::SelObject, WherePredicate::WhereKey, 
		MatchType::ExactMatch, keys);
	
	cout << endl << "Now deleting record with key " + recordTwo.getItem();
	db.deleteRecord("abc");
	db.select(SelectObjectType::SelObject, WherePredicate::WhereKey,
		MatchType::ExactMatch, keys);

	}

void DemonstrateRequirement10(NoSQLDB<string> & db, const DatabaseInfo & databaseInfo) {
	cout << "Requirement 10 - Importing a file with project dependencies" << endl;
	cout << "Importing database - " << databaseInfo.DatabaseName << endl;
	db.importDB();
	cout << endl << endl << "Executing select query";
	db.select(SelectObjectType::SelObject, WherePredicate::WhereDefault,
		MatchType::ExactMatch, vector<string>());
}

void DemonstrateRequirement4(NoSQLDB<string> & db, const DatabaseInfo & databaseInfo) {
	cout << "Requirement 4 - Perform various edit operations." << endl;
	cout << endl << endl << "Executing select query for a record." << endl;
	vector<string> keys;
	keys.push_back("project1");
	keys.push_back("project2");
	db.select(SelectObjectType::SelObject, WherePredicate::WhereKey,
		MatchType::ExactMatch, keys);

	db.updateRecordMetadata("project1", MetadataType::Item, ".library");
	db.updateRecordMetadata("project1", MetadataType::Time, "10:10");
	db.updateRecordMetadata("project2", MetadataType::Category, "framework");
	db.updateRecordMetadata("project2", MetadataType::Description,
		"changed description");

	db.select(SelectObjectType::SelObject, WherePredicate::WhereKey,
		MatchType::ExactMatch, keys);
	db.updateRecordValue("interfaces", "Changed Value");

	keys.push_back("interfaces");
	db.select(SelectObjectType::SelObject, WherePredicate::WhereKey,
		MatchType::ExactMatch, keys);

	cout << endl << "By design the database doesnot support editing of keys." << endl;
	cout << endl << "If the item value is used as key for object";
	cout << endl << "then editing the item value is forbidden.";

	db.updateRecordMetadata("interfaces", MetadataType::Item, "hello");

	keys.pop_back();
	keys.push_back("project1");
	keys.push_back("project2");
	keys.push_back("project3");
	cout << endl << "Adding and deleting relationships." << endl;
	db.select(SelectObjectType::SelChildKey, WherePredicate::WhereKey,
		MatchType::ExactMatch, keys);
	db.addRelationship("project1", "project2");
	db.addRelationship("project1", "project3");
	db.addRelationship("project2", "project3");
	cout << endl << "Trying to add same child to parent relationship" << endl;
	db.addRelationship("project2", "project1");
	db.select(SelectObjectType::SelChildKey, WherePredicate::WhereKey,
		MatchType::ExactMatch, keys);
	db.deleteRelationship("project2", "project3");
	db.select(SelectObjectType::SelChildKey, WherePredicate::WhereKey,
		MatchType::ExactMatch, keys);
}

void DemonstrateRequirement5(NoSQLDB<string> & db, const DatabaseInfo & databaseInfo) {
	cout << "Requirement 5 - Persist contents of the DB to XML file." << endl;
	cout << endl << "Executing commit.";
	db.commit();
}

void DemonstrateRequirement6(NoSQLDB<string> & db, const DatabaseInfo & databaseInfo) {
	cout << endl << "Requirement 6 - Automatic commit after certain number of wirtes.";
	cout << endl << "Changing the auto commit interval to 2.";
	db.ChangeDatabaseAutoCommit(2);
	cout << endl << "Now db will automatically persist changes after 2 every writes.";
	vector<string> keys;
	keys.push_back("project1");
	db.select(SelectObjectType::SelObject, WherePredicate::WhereKey,
		MatchType::ExactMatch, keys, nullptr);
	db.updateRecordMetadata("project1", MetadataType::Category, "Cpp");
	db.updateRecordMetadata("project1", MetadataType::Description, "Changed desc.");
	db.updateRecordMetadata("project1", MetadataType::Description, "Changed descriptoin.");
	db.updateRecordMetadata("project1", MetadataType::Description, "Changed desctription again.");
	db.select(SelectObjectType::SelObject, WherePredicate::WhereKey,
		MatchType::ExactMatch, keys, nullptr);
}

void DemonstrateRequirement7(NoSQLDB<string> & db, const DatabaseInfo & databaseInfo) {

	cout << endl << "Requirement 7 and 8 - Queries." << endl;
	vector<string> keys;
	keys.push_back("project1");
	keys.push_back("project2");
	cout << endl << "Requirement 7.1 - Value of a specified key." << endl;
	db.select(SelectObjectType::SelValue, WherePredicate::WhereKey,
		MatchType::ExactMatch, keys);
	cout << endl << "Requirement 7.2 - Children of a specified key." << endl;
	db.select(SelectObjectType::SelChildKey, WherePredicate::WhereKey,
		MatchType::ExactMatch, keys);
	cout << endl << "Requirement 7.3 - Pattern matching in key.";
	cout << endl << "Displaying all keys." << endl;
	db.select(SelectObjectType::SelKey, WherePredicate::WhereDefault,
		MatchType::ExactMatch, keys);
	cout << endl << "Displaying all keys which match pattern : Proj" << endl;
	keys.pop_back();
	keys.pop_back();
	keys.push_back("proj");
	db.select(SelectObjectType::SelKey, WherePredicate::WhereKey,
		MatchType::PartialMatch, keys);
	cout << endl << "Requirement 7.4 - All keys where category like : app" << endl;
	keys.pop_back(); keys.push_back("App");
	db.select(SelectObjectType::SelKey, WherePredicate::WhereCategory,
		MatchType::PartialMatch, keys);

	keys.pop_back(); keys.push_back(".ex");
	cout << endl << "Requirement 7.5 - All keys where item like : .ex" << endl;
	db.select(SelectObjectType::SelKey, WherePredicate::WhereItem,
		MatchType::PartialMatch, keys);

	keys.pop_back(); keys.push_back("Core");
	cout << endl << "Requirement 7.6 - All keys where value like : Core" << endl;
	db.select(SelectObjectType::SelKey, WherePredicate::WhereValue,
		MatchType::PartialMatch, keys);
}

void DemonstrateRequirement8(NoSQLDB<string> & db, const DatabaseInfo & databaseInfo) {
	
	vector<string> keys;
	keys.push_back("proj");
	cout << endl << "Requirement 8 - Chaining queries and performing intersection and union.";
	cout << endl;
	IQueryChainer * keyset = db.select(SelectObjectType::SelKey, WherePredicate::WhereKey,
		MatchType::PartialMatch, keys);

	keys.pop_back();
	keys.push_back(".lib");
	cout << endl << "Using the last result set into the current.";
	cout << endl;
	keyset = db.select(SelectObjectType::SelKey, WherePredicate::WhereItem,
		MatchType::PartialMatch, keys, keyset->chainIntersection());

	cout << endl << "Requirement 9 - Union of queries." << endl;

	keys.pop_back();
	keys.push_back("proj");
	keyset = db.select(SelectObjectType::SelKey, WherePredicate::WhereKey,
		MatchType::PartialMatch, keys, keyset->chainUnion());

	cout << endl << "Printing final DB." << endl;
	db.select(SelectObjectType::SelObject, WherePredicate::WhereDefault,
		MatchType::PartialMatch, keys);
}

void createFile() {

	DatabaseInfo databaseInfo;
	databaseInfo.DatabaseName = "projectdependencies";
	databaseInfo.AutoCommit = -1;		//	Turning auto commit off

	NoSQLDB<string> db(databaseInfo);

	Record<string> record;
	record.setItem(".lib");
	record.setCategory("Project");
	record.setDescription("Contains Core types");
	record.setData("Core.Datastructures");

	db.addRecord("project1", record);

	Record<string> record2;
	record2.setItem(".exe");
	record2.setCategory("Project");
	record2.setDescription("Data Access Layer.");
	record2.setData("Core.StorageManager");

	db.addRecord("project2", record2);

	Record<string> record3;
	record3.setItem(".lib");
	record3.setCategory("Project");
	record3.setDescription("The NoSQL Database Engine.");
	record3.setData("Core.StorageManager");

	db.addRecord("project3", record3);

	Record<string> record4;
	record4.setItem(".lib");
	record4.setCategory("Project");
	record4.setDescription("The NoSQL Database Engine.");
	record4.setData("XMLDocument");

	db.addRecord("project4", record4);

	Record<string> record5;
	record5.setItem(".exe");
	record5.setCategory("Application");
	record5.setDescription("Test executive.");
	record5.setData("TestExecutive");

	db.addRecord("project5", record5);

	db.commit();
}


int main()
{
	cout << "OOD Assignment 1 - NoSQL Database" << endl;
	cout << "----------------------------------" << endl;
	cout << "Rohit Kulkarni - SUID - 618809126" << endl << endl;

	cout << "Test Executive - NoSQL Database." << endl << endl;
	cout << "----------------------------------" << endl;

//	createFile();

	DatabaseInfo databaseInfo;
	databaseInfo.DatabaseName = "projectdependencies";

	NoSQLDB<string> db(databaseInfo);

	DemonstrateRequirement10(db, databaseInfo);
	insertDashedLine();

	DemonstrateRequirement3(db, databaseInfo);
	insertDashedLine();

	DemonstrateRequirement4(db, databaseInfo);
	insertDashedLine();
	
	DemonstrateRequirement5(db, databaseInfo);
	insertDashedLine();

	DemonstrateRequirement6(db, databaseInfo);
	insertDashedLine();

	DemonstrateRequirement7(db, databaseInfo);
	insertDashedLine();

	DemonstrateRequirement8(db, databaseInfo);

	int a;
	cin >> a;
}


