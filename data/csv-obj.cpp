#include <fstream>
using namespace std;

int main(int argc, char* argv[]) {
	ifstream file ("dat.csv");
	ofstream oFile ("out.json");
	string line;

	bool kwh = false;
	bool natGas = false;
	int counter = 0;

	if ( file.is_open() )
	{
		oFile << "{\n\t\"buildings\": {\n";
		while ( getline (file,line) )
		{
			if ( line.find("SQFEET") != string::npos )
			{
				int cc = 0;
				string bldg = "\t\t";
				for (string::iterator i=line.begin(); i!=line.end(); ++i)
				{
					if ( cc == 1 ) bldg += *i;
					if ( *i == ',' ) ++cc;
				}
				bldg.pop_back();
				bldg.append(": {\n");
				oFile << bldg;
			}
			if ( kwh && (counter+1)%2 == 0 && counter < 25 )
			{
				string month = "[";
				int cc = 0;
				for (string::iterator i=line.begin(); i!=line.end(); ++i)
				{
					if ( cc >= 2 && cc < 6 )
					{
						if ( *i != '\"' ) month += *i;
					}
					if ( *i == ',' ) ++cc;
				}
				while ( month.find(",,") != string::npos ) { month.insert(month.find(",,")+1,"-1");}
				month.pop_back();
				month.append("],");
				if ( counter == 23 ) { month.pop_back(); month.append("\n\t\t\t],\n"); }
				oFile << month;
			}
			if ( natGas && (counter+1)%2 == 0 && counter < 25 )
			{
				string month = "[";
				int cc = 0;
				for (string::iterator i=line.begin(); i!=line.end(); ++i)
				{
					if ( cc >= 2 && cc < 6 && cc != 3 )
					{
						if ( *i != '\"' ) month += *i;
					}
					if ( *i == ',' ) ++cc;
				}
				while ( month.find(",,") != string::npos ) { month.insert(month.find(",,")+1,"-1");}
				month.pop_back();
				month.append("],");
				if ( counter == 23 ) { month.pop_back(); month.append("\n\t\t\t],\n"); }
				oFile << month;
			}
			if ( line.find("KWH") != string::npos )
			{
				oFile << "\t\t\t\"KWH\": [\n\t\t\t\t";
				kwh = true;
				counter = 0;
			}
			if ( line.find("CCF") != string::npos )
			{
				oFile << "\t\t\t\"CCF\": [\n\t\t\t\t";
				natGas = true;
				counter = 0;
			}
			if ( line.find("Total Cost") != string::npos )
			{
				int cc = 0;
				string totals = "\t\t\t\"Total\": ";
				for (string::iterator i=line.begin(); i!=line.end(); ++i)
				{
					if ( cc == 4 && *i != ',') totals += *i;
					if ( *i == ',' ) ++cc;
				}
				totals.append("\n\t\t},\n");
				oFile << totals;
			}
			if ( counter == 25 )
			{
				int cc = 0;
				string totArr;
				if ( kwh ) totArr = "\t\t\t\"KWHtot\": [";
				else totArr = "\t\t\t\"CCFtot\": [";
				for (string::iterator i=line.begin(); i!=line.end(); ++i)
				{
					if ( cc == 2 || cc == 4 || cc == 5)
					{
						if ( *i != '\"') totArr += *i;
					}
					if ( *i == ',' ) ++cc;
				}
				totArr.pop_back();
				totArr.append("],\n");
				oFile << totArr;
				kwh = false;
				natGas = false;
			}
			++counter;
		}
	}
	file.close();
	oFile.close();
}
