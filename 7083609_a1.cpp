#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

bool inputfile = false;
int xlow, xupp;
int ylow, yupp;
int xrange, yrange;
vector<string> uniqueCityID;

vector<string> tokenizeString (string input, string delimiter)
{
	size_t pos = 0;
	string token;
	vector<string> result;

	while ((pos = input.find(delimiter)) != string::npos) 
	{
    	token = input.substr(0, pos);
    	result.push_back (token);
		input.erase(0, pos + delimiter.length());
	}
	result.push_back (input);
	return (result);
}
//struct for each location on 2d map
struct map
{
	string cityid;
	string cityname;
	int cloud;
	int cloudsingle;
	char cloudLMH;
	int pressure;
	int pressuresingle;
	string pressureLMH;
};
struct map ** array2D = nullptr;
struct citysummary
{
	string cityid;
	string cityname;
	vector<int> xcord;
	vector<int> ycord;
	//int totcloud;
	//int totpressure;
	double avecloud;
	double avepressure;
};
struct citysummary * citylocpointer = nullptr;
//returns lmh for the value of cloud/pressure
char lmh (int val)
{
	if (0 <= val && val < 35)
	{
		return 'L';
	}
	else if (35 <= val && val < 65)
	{
		return 'M';
	}
	else if (65 <= val && val < 100)
	{
		return 'H';
	}
	else
	{
		return 'E';
	}
}
//initiate 2d array
void init2DArray (int col, int row)
{
	array2D = new struct map * [row];
	for (int i = 0; i < row; i++)
		array2D[i] = new struct map [col];
}
//initiate array for each city range
void initCityArray (int noOfCity)
{
	citylocpointer = new struct citysummary [noOfCity];
	for (int i = 0; i < uniqueCityID.size(); i++)
	{
		struct citysummary citysum = citylocpointer[i];
		citysum.cityid = uniqueCityID[i];
		for(int i = ylow; i <= yupp; i++)
		{
			for(int j = xlow; j <= xupp; j++)
			{
				struct map temp = array2D[j-xlow][i-ylow];
				if(citysum.cityid == temp.cityid)
				{
					citysum.cityname = temp.cityname;

					citysum.xcord.push_back(j);
					citysum.ycord.push_back(i);
				}
			}
		}
		sort(citysum.xcord.begin(), citysum.xcord.end());
		sort(citysum.ycord.begin(), citysum.ycord.end());
		citylocpointer[i] = citysum;
	}
}

void safelyDeallocateMemory(int col, int row)
{
	if (col <= 0)
		return;
	
	for (int i = 0; i < row; i++)
		delete [] array2D [i];
	delete [] array2D;

	delete [] citylocpointer;
	uniqueCityID.clear();
}
//puts unique city id into a vector
void insertCityID(string cityid)
{
	if (find(uniqueCityID.begin(), uniqueCityID.end(), cityid) == uniqueCityID.end())
		uniqueCityID.push_back(cityid);
}
//opens and stores data from city file
void openCityFile(string filename)
{
	int x, y;
	ifstream myFile;
	myFile.open(filename);
	if (!myFile)
	{
		cerr << "Error: City file" << endl;
	}
	if (myFile.is_open())
	{
		string line;
		while (getline(myFile, line))
		{
			if (line.empty())
				break;
			line.erase(0,1);
			vector<string> tokenStringVector = tokenizeString (line, ", ");
			x = stoi(tokenStringVector[0])-xlow;
			tokenStringVector = tokenizeString (tokenStringVector[1], "]-");
			y = stoi(tokenStringVector[0])-ylow;
			struct map temp = array2D[x][y];
			tokenStringVector = tokenizeString (tokenStringVector[1], "-");
			temp.cityid = tokenStringVector[0];
			temp.cityname = tokenStringVector[1];
			insertCityID(temp.cityid);
			array2D[x][y] = temp;
			tokenStringVector.clear();
		}
		initCityArray(uniqueCityID.size());
	}
	myFile.close();
}
//opens and stores data from cloud file
void openCloudFile(string filename)
{
	int x, y;
	ifstream myFile;
	myFile.open(filename);
	if (!myFile)
	{
		cerr << "Error: Cloud file" << endl;
	}
	if (myFile.is_open())
	{
		string line;
		while (getline(myFile, line))
		{
			if (line.empty())
				break;
			line.erase(0,1);
			vector<string> tokenStringVector = tokenizeString (line, ", ");
			x = stoi(tokenStringVector[0])-xlow;
			tokenStringVector = tokenizeString (tokenStringVector[1], "]-");
			y = stoi(tokenStringVector[0])-ylow;
			struct map temp = array2D[x][y];
			temp.cloud = stoi(tokenStringVector[1]);
			temp.cloudsingle = (stoi(tokenStringVector[1])) / 10;
			temp.cloudLMH = lmh(stoi(tokenStringVector[1]));
			array2D[x][y] = temp;
			tokenStringVector.clear();
		}
	}
	myFile.close();
}
//opens and stores data from pressure file
void openPressureFile(string filename)
{
	int x, y;
	ifstream myFile;
	myFile.open(filename);
	if (!myFile)
	{
		cerr << "Error: Pressure file" << endl;
	}
	if (myFile.is_open())
	{
		string line;
		while (getline(myFile, line))
		{
			if (line.empty())
				break;
			line.erase(0,1);
			vector<string> tokenStringVector = tokenizeString (line, ", ");
			x = stoi(tokenStringVector[0])-xlow;
			tokenStringVector = tokenizeString (tokenStringVector[1], "]-");
			y = stoi(tokenStringVector[0])-ylow;
			struct map temp = array2D[x][y];
			temp.pressure = stoi(tokenStringVector[1]);
			temp.pressuresingle = (stoi(tokenStringVector[1])) / 10;
			temp.pressureLMH = lmh(stoi(tokenStringVector[1]));
			array2D[x][y] = temp;
			tokenStringVector.clear();
		}
	}
	myFile.close();
}
//display main menu
int mainmenu()
{
	int option;
	cout << endl << "Student ID: 3087609" << endl;
	cout << "Student Name: Yong Jia Liang" << endl;
	cout << "------------------------------------------" << endl;
	cout << "Welcome to Weather Information Processing System!" << endl << endl;
	cout << "1)\tRead in and process a configuration file" << endl;
	cout << "2)\tDisplay city map" << endl;
	cout << "3)\tDisplay cloud coverage map (cloudiness index)" << endl;
	cout << "4)\tDisplay cloud coverage map (LMH symbols)" << endl;
	cout << "5)\tDisplay atmospheric pressure map (pressure index)" << endl;
	cout << "6)\tDisplay atmospheric pressure map (LMH symbols)" << endl;
	cout << "7)\tShow weather forecast summary report" << endl;
	cout << "8)\tQuit" << endl << endl;
	cout << "Please enter your choice: " << endl;
	cin >> option;
	return option;
}
//make sure user press enter
void userenter()
{
	int i = 0;
	while (i == 0)
	{
		cout << "\nPress <enter> to go back to main menu...";
		cin.ignore();
		if (cin.get() == '\n')
			i = 1;
	}
	cout << endl;
}
void option1();
/*option 1 selected
reads in config file
further reads in city file cloud file and pressure file
reads in data on each file and inserts data into struct*/
void option1()
{
	uniqueCityID.clear();
	int check = 0; //check if all 5 info from config file read
	vector<string> citycloudpressure; //vector to store filenames from config file
	cout << "Please enter config filename (type return to go back to main menu): ";
	string filename;
	cin >> filename;
	if (filename != "return")
	{
		ifstream myFile;
		myFile.open(filename);
		if (!myFile)
		{
			cout << "Error opening : " << filename << " . Try again." << endl << endl;
			option1();
		}
		if (myFile.is_open())
		{
			cout << endl << "Opening file ..." << endl;
			string line;
			while (getline(myFile, line))
			{
				if (line.find("GridX_IdxRange") != string::npos) //x range
				{
					cout << "Reading in " << line;
					vector<string> tokenStringVector = tokenizeString (line, "=");
					vector<string> tokenStringVector2 = tokenizeString (tokenStringVector[1], "-");
					xlow = stoi(tokenStringVector2[0]);
					xupp = stoi(tokenStringVector2[1]);
					tokenStringVector.clear();
					tokenStringVector2.clear();
					xrange = xupp - xlow + 1;
					check++;
					cout << " ... done!" << endl;
				}
				else if (line.find("GridY_IdxRange") != string::npos) //y range
				{
					cout << "Reading in " << line;
					vector<string> tokenStringVector = tokenizeString (line, "=");
					vector<string> tokenStringVector2 = tokenizeString (tokenStringVector[1], "-");
					ylow = stoi(tokenStringVector2[0]);
					yupp = stoi(tokenStringVector2[1]);
					tokenStringVector.clear();
					tokenStringVector2.clear();
					yrange = yupp - ylow + 1;
					check++;
					cout << " ... done!" << endl;
				}
				else if (line.find(".txt") != string::npos) //filenames
				{
					cout << "Storing data from " << line;
					citycloudpressure.push_back(line);
					check++;
					cout << " ... done!" << endl;
				}
			}
			if (check == 5) //all data read
			{
				cout << "All records successfully stored" << endl;
				init2DArray (yrange, xrange);
				inputfile = true;
				openCityFile(citycloudpressure[0]);
				openCloudFile(citycloudpressure[1]);
				openPressureFile(citycloudpressure[2]);
				myFile.close();
				cout << "Closing file ... " << endl;
			}
			else //something missing
			{
				myFile.close();
				cout << "Only " << check << " data read, please check file." << endl;
			}
		}
		citycloudpressure.clear();
	}
}

void option2()
{
	cout << "  ";
	for(int i = 0; i < (xrange + 2); i++)
		cout << "  #";
	cout << endl;

	for(int i = yupp; i >= ylow; i--)
	{
		if(i > 9)
			cout << i << "  #";
		else
			cout << i << "   #";
		
		for(int j = xlow; j <= xupp; j++)
		{
			struct map temp = array2D[j-xlow][i-ylow];
			if(temp.cityid != "")
				cout << "  " << temp.cityid;
			else
				cout << "   ";
		}
		cout << "  #" << endl;
	}
	cout << "  ";
	for(int i = 0; i < (xrange + 2); i++)
		cout << "  #";
	cout << endl << "     ";
	for(int j = xlow; j <= xupp; j++)
	{
		if (j > 9)
			cout << " " << j;
		else
			cout << "  " << j;
	}
	cout << endl;
}

void option3()
{
	cout << "  ";
	for(int i = 0; i < (xrange + 2); i++)
		cout << "  #";
	cout << endl;

	for(int i = yupp; i >= ylow; i--)
	{
		if(i > 9)
			cout << i << "  #";
		else
			cout << i << "   #";
		
		for(int j = xlow; j <= xupp; j++)
		{
			struct map temp = array2D[j-xlow][i-ylow];
			cout << "  " << temp.cloudsingle;
		}
		cout << "  #" << endl;
	}
	cout << "  ";
	for(int i = 0; i < (xrange + 2); i++)
		cout << "  #";
	cout << endl << "     ";
	for(int j = xlow; j <= xupp; j++)
	{
		if (j > 9)
			cout << " " << j;
		else
			cout << "  " << j;
	}
	cout << endl;
}

void option4()
{
	cout << "  ";
	for(int i = 0; i < (xrange + 2); i++)
		cout << "  #";
	cout << endl;

	for(int i = yupp; i >= ylow; i--)
	{
		if(i > 9)
			cout << i << "  #";
		else
			cout << i << "   #";
		
		for(int j = xlow; j <= xupp; j++)
		{
			struct map temp = array2D[j-xlow][i-ylow];
			cout << "  " << temp.cloudLMH;
		}
		cout << "  #" << endl;
	}
	cout << "  ";
	for(int i = 0; i < (xrange + 2); i++)
		cout << "  #";
	cout << endl << "     ";
	for(int j = xlow; j <= xupp; j++)
	{
		if (j > 9)
			cout << " " << j;
		else
			cout << "  " << j;
	}
	cout << endl;
}

void option5()
{
	cout << "  ";
	for(int i = 0; i < (xrange + 2); i++)
		cout << "  #";
	cout << endl;

	for(int i = yupp; i >= ylow; i--)
	{
		if(i > 9)
			cout << i << "  #";
		else
			cout << i << "   #";
		
		for(int j = xlow; j <= xupp; j++)
		{
			struct map temp = array2D[j-xlow][i-ylow];
			cout << "  " << temp.pressuresingle;
		}
		cout << "  #" << endl;
	}
	cout << "  ";
	for(int i = 0; i < (xrange + 2); i++)
		cout << "  #";
	cout << endl << "     ";
	for(int j = xlow; j <= xupp; j++)
	{
		if (j > 9)
			cout << " " << j;
		else
			cout << "  " << j;
	}
	cout << endl;
}

void option6()
{
	cout << "  ";
	for(int i = 0; i < (xrange + 2); i++)
		cout << "  #";
	cout << endl;

	for(int i = yupp; i >= ylow; i--)
	{
		if(i > 9)
			cout << i << "  #";
		else
			cout << i << "   #";
		
		for(int j = xlow; j <= xupp; j++)
		{
			struct map temp = array2D[j-xlow][i-ylow];
			cout << "  " << temp.pressureLMH;
		}
		cout << "  #" << endl;
	}
	cout << "  ";
	for(int i = 0; i < (xrange + 2); i++)
		cout << "  #";
	cout << endl << "     ";
	for(int j = xlow; j <= xupp; j++)
	{
		if (j > 9)
			cout << " " << j;
		else
			cout << "  " << j;
	}
	cout << endl;
}

int rainchance(char acc, char ap)
{
	int probability;
	if (ap == 'L' && acc == 'H')
	{
		probability = 90;
	}
	else if (ap == 'L' && acc == 'M')
	{
		probability = 80;
	}
	else if (ap == 'L' && acc == 'L')
	{
		probability = 70;
	}
	else if (ap == 'M' && acc == 'H')
	{
		probability = 60;
	}
	else if (ap == 'M' && acc == 'M')
	{
		probability = 50;
	}
	else if (ap == 'M' && acc == 'L')
	{
		probability = 40;
	}
	else if (ap == 'H' && acc == 'H')
	{
		probability = 30;
	}
	else if (ap == 'H' && acc == 'M')
	{
		probability = 20;
	}
	else if (ap == 'H' && acc == 'L')
	{
		probability = 10;
	}
	else 
	{
		probability = 0;
	}
	return probability;
}
void rainsymbol(int probability)
{
	if (probability == 90)
	{
		cout << "~~~~" << endl;
		cout << "~~~~~" << endl;
		cout << "\\\\\\\\\\" << endl << endl;
	}
	else if (probability == 80)
	{
		cout << "~~~~" << endl;
		cout << "~~~~~" << endl;
		cout << " \\\\\\\\" << endl << endl;
	}
	else if (probability == 70)
	{
		cout << "~~~~" << endl;
		cout << "~~~~~" << endl;
		cout << "  \\\\\\" << endl << endl;
	}
	else if (probability == 60)
	{
		cout << "~~~~" << endl;
		cout << "~~~~~" << endl;
		cout << "   \\\\" << endl << endl;
	}
	else if (probability == 50)
	{
		cout << "~~~~" << endl;
		cout << "~~~~~" << endl;
		cout << "    \\" << endl << endl;
	}
	else if (probability == 40)
	{
		cout << "~~~~" << endl;
		cout << "~~~~~" << endl << endl;
	}
	else if (probability == 30)
	{
		cout << "~~~" << endl;
		cout << "~~~~" << endl << endl;
	}
	else if (probability == 20)
	{
		cout << "~~" << endl;
		cout << "~~~" << endl << endl;
	}
	else if (probability == 10)
	{
		cout << "~" << endl;
		cout << "~~" << endl << endl;
	}
}
void option7()
{
	cout << endl << "Weather Forecast Summary Report" << endl << "-------------------------------" << endl;
	for (int i = 0; i < uniqueCityID.size(); i++)
	{
		struct citysummary citysum = citylocpointer[i];
		int xstart, xend, ystart, yend;
		int cloudtotal = 0;
		int pressuretotal = 0;
		int counter = 0;
		if (citysum.xcord.front() == xlow)
			xstart = xlow;
		else
			xstart = citysum.xcord.front() - 1;
		
		if (citysum.xcord.back() == xupp)
			xend = xupp;
		else
			xend = citysum.xcord.back() + 1;
		
		if (citysum.ycord.front() == ylow)
			ystart = ylow;
		else
			ystart = citysum.ycord.front() - 1;
		
		if (citysum.ycord.back() == yupp)
			yend = yupp;
		else
			yend = citysum.ycord.back() + 1; 

		for (int i = ystart; i <= yend; i++)
		{
			for (int j = xstart; j <= xend; j++)
			{
				struct map temp = array2D[j-xlow][i-ylow];
				cloudtotal += temp.cloud;
				pressuretotal += temp.pressure;
				counter++;
			}
		}
		//citysum.totcloud = cloudtotal;
		//citysum.totpressure = pressuretotal;
		citysum.avecloud = cloudtotal / double(counter);
		citysum.avepressure = pressuretotal / double(counter);
		int rainprobability = rainchance(lmh(citysum.avecloud), lmh(citysum.avepressure));
		cout << "City Name : " << citysum.cityname << endl;
		cout << "City ID   : " << citysum.cityid << endl;
		cout << "Ave. Cloud Cover (ACC)  : " << fixed << setprecision(2) << citysum.avecloud << " (" << lmh(citysum.avecloud) << ")" << endl;
		cout << "Ave. Pressure    (AP)  : " << fixed << setprecision(2) << citysum.avepressure << " (" << lmh(citysum.avepressure) << ")" << endl;
		cout << "Probability of Rain (%) : " << fixed << setprecision(2) << rainprobability << endl;
		rainsymbol(rainprobability);
		citylocpointer[i] = citysum;
	}
}

int main()
{
	int checkexit = '\0';;
	while (checkexit == '\0')
	{
		int option = mainmenu();
		if (option == 8)
		{
			cout << "Exiting application." << endl;
			safelyDeallocateMemory(xupp, yupp);
			checkexit = 1;
		}
		else if (8 < option || option < 1)
		{
			cout << endl << "Please select an option 1 - 8. ";
			userenter();
		}
		else if (inputfile == false && option != 1)
		{
			cout << endl << "No file detected. Please input file.";
			userenter();
		}
		else
		{
			switch (option)
			{
				case 1:
					cout << endl << "You have selected option 1" << endl << endl;
					option1();
					break;
				case 2:
					cout << endl << "You have selected option 2" << endl << endl;
					option2();
					userenter();
					break;
				case 3:
					cout << endl << "You have selected option 3" << endl << endl;
					option3();
					userenter();
					break;
				case 4:
					cout << endl << "You have selected option 4" << endl << endl;
					option4();
					userenter();
					break;
				case 5:
					cout << endl << "You have selected option 5" << endl << endl;
					option5();
					userenter();
					break;
				case 6:
					cout << endl << "You have selected option 6" << endl << endl;
					option6();
					userenter();
					break;
				case 7:
					cout << endl << "You have selected option 7" << endl << endl;
					option7();
					userenter();
					break;
				default: 
					cout << endl << "Please select an option 1 - 8. " << endl << endl;
			}
		}
	}
	return 0;
}
