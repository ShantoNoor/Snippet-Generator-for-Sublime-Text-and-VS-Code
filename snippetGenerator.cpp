#include <iostream>
#include <regex>
#include <fstream>
#include <sstream>
#include <dir.h>

using namespace std;

bool strEmpty(string str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if((str[i] >= 'A' && str[i] <= 'Z') || 
		   (str[i] >= 'a' && str[i] <= 'z'))
		{
			return false;
		}
	}
	cout << "Empty String!!" << endl;	
	return true;
}

struct Name
{
	string source;
	string fileName;
	string info;
	string code;
	string tab;
	string scope;
	string lastStr;
};
Name name;

string v; 
stringstream vss;
ofstream os, vs;

void printVSinfo()
{
	vs << "// Location : C:\\Users\\Shanto\\AppData\\Roaming\\Code\\User\\snippets" << "\n";
	vs << "// C++ Snippet for VS Code" << "\n";
	vs << "// Created by snippetGenerator" << "\n";
	vs << "// Source : " << name.source << "\n";
	vs << "// Author : Shanto Noor!" << "\n\n"; 
}

void printSUBLinfo()
{
	os << "<!-- Location : C:\\Users\\Shanto\\AppData\\Roaming\\Sublime Text 3\\Packages\\User -->" << "\n";
	os << "<!-- C++ Snippet for Sublime Text 3 -->" << "\n";
	os << "<!-- Created by snippetGenerator -->" << "\n";
	os << "<!-- Source : " << name.source << " -->\n";
	os << "<!-- Author : Shanto Noor! -->"; 
}

string tab; int scount = 0; 
void CreateSublimeSinppet()
{
	vss.str(name.tab);
	vss.clear(); 
	while(vss >> v) 
	{
		tab = "C:/Users/Shanto/AppData/Roaming/Sublime Text 3/Packages/User/" 
				+ name.fileName + " [" + v + "]" + ".sublime-snippet";
		os.open(tab);

		os << "<snippet>" << "\n";
		os << "\t<description>|" << name.info << "|</description>" << "\n";
		os << "\t<content><![CDATA[" << "\n";
		os << name.code << "\n";
		os << "]]></content>" << "\n";
		os << "\t<tabTrigger>" << v << "</tabTrigger>" << "\n";
		os << "\t<scope>" << name.scope << "</scope>" << "\n";
		os << "</snippet>" << "\n\n";

		printSUBLinfo();
		os.close();

		scount++;
	}
}

int vcount = 0;
void CreateVSCodeSnippet()
{
	// printing fileName
	vs << "\t\"" << name.fileName << "\": {\n";

	// printing prefix
	vss.str(name.tab);
	vss.clear(); 
	vs << "\t\t\"prefix\": ["; 
	while(vss >> v) vs << "\"" << v << "\", ";
	vs << "],\n";

	// printing body
	vss.str(name.code);
	vss.clear();
	vs << "\t\t\"body\": [\n";
	while(getline(vss, v)) 
	{
		vs << "\t\t\t\""; 
		for(unsigned int i = 0; i < v.size(); i++)
		{
			if(v[i] == '\"') vs << "\\";
			vs << v[i];
		}
		vs << "\"," << "\n";
	}
	vs << "\t\t],\n";

	// printing description
	vs << "\t\t\"description\": \"|" << name.info << "|\"\n";
	vs << "\t}," << "\n";

	vcount++;
}

bool Search(string& str)
{
	smatch m;
	regex e;		
	bool isFound;

	e = "<snippet.\\d+>";
	isFound = regex_search(str, m, e);
	if(!isFound || strEmpty(m.str()))
	{
		cout << "Last Head : " << name.lastStr << endl;
		cout << "Failled to Match <snippet>" << endl;
		return false;
	}
	name.lastStr = m.str();

	e = "<fileName>(.*?)</fileName>";
	isFound = regex_search(str, m, e);
	if(!isFound || strEmpty(m[1].str()))
	{
		cout << "Last Head : " << name.lastStr << endl;
		cout << "Failled to Match <fileName>" << endl;
		return false;
	}
	name.fileName = m[1].str();

	e = "<info>(.*?)</info>";
	isFound = regex_search(str, m, e);
	if(!isFound || strEmpty(m[1].str()))
	{
		cout << "Last Head : " << name.lastStr << endl;
		cout << "Failled to Match <info>" << endl;
		return false;
	}
	name.info = m[1].str();

	e = "<code>\n((\n|.)*?)\n\t</code>";
	isFound = regex_search(str, m, e);
	if(!isFound || strEmpty(m[1].str()))
	{
		cout << "Last Head : " << name.lastStr << endl;
		cout << "Failled to Match <code>" << endl;
		return false;
	}
	name.code = m[1].str();

	e = "<tab>(.*?)</tab>";
	isFound = regex_search(str, m, e);
	if(!isFound || strEmpty(m[1].str()))
	{
		cout << "Last Head : " << name.lastStr << endl;
		cout << "Failled to Match <tab>" << endl;
		return false;
	}
	name.tab = m[1].str();

	e = "<scope>(.*?)</scope>";
	isFound = regex_search(str, m, e);
	if(!isFound || strEmpty(m[1].str()))
	{
		cout << "Last Head : " << name.lastStr << endl;
		cout << "Failled to Match <scope>" << endl;
		return false;
	}
	name.scope = m[1].str();

	return true;
}

int main()
{
	string str;

	stringstream ss;

	name.source = "cpp.snipx";

	ifstream infile;
	infile.open(name.source);

	vs.open("C:/Users/Shanto/AppData/Roaming/Code/User/snippets/cpp.json");
	printVSinfo();
	vs << "{" << "\n";

	// mkdir("subl");

	while(getline(infile, str))
	{
		ss << str;
		ss << '\n';
		if(str == "</snippet>")
		{
			str = ss.str();

			if(!Search(str)) return -1;

			CreateSublimeSinppet();
			CreateVSCodeSnippet();

			ss.str(std::string());
		}
	}

	vs << "}";
	vs.close();

	infile.close();

	cout << "Successully Created : " << scount << " sublime-snippet files!" << endl;
	cout << "Successully Created : " << vcount << " vscode-snippets in cpp.json!" << endl;

	cin.get();

	return 0;
}