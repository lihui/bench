#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<strstream>
#include<chrono>
#include<algorithm>
#include<cstdio>
using namespace std;

/**
* Created by lihui on 2014/4/1.
*/

class Slice{
   public:
   const char* start;
   const char* end;
   Slice(){}
   Slice(const char* cstr){
         start=cstr;
         while(*cstr) cstr++;
         end=cstr;
   }
  Slice (const char* start ,const char* end){
        this->start=start;
        this->end=end;
  }
};

static bool isWhite(char ch){
   return ' '==ch||'\n'==ch||'\r'==ch||'\t'==ch;
}

Slice trim(Slice slice){
   const char* start=slice.start;
   const char* end=slice.end-1;
   if(start>end) return slice;
   while(isWhite(*start)) start++;
   while(isWhite(*end)&&end>=start) end--;
   
  return Slice{start,end+1};
}
size_t find(const char* str,char ch){
       const char* head=str;
       while(*str!=ch&&*str!=0) str++;
       return str-head;
}
static string toString(Slice slice){
        slice=trim(slice);
        return string(slice.start,slice.end-slice.start);
}
class CsvParser {
private:
	const char*  _pos = 0;
public:
	CsvParser(const char* str) {
		_pos=str;
	}

	vector<vector<Slice>> parseCsv() {
	       vector<vector<Slice>> rs;
	        do{
		    rs.push_back (parseLine());
		}while(consume('\n'));
		return rs;
	}
       
	vector<Slice> parseLine() {
		vector<Slice> row;
		do{
                    row.push_back(parseField());
                }while(consume(','));
		return row;
	}

	Slice parseField() {
		if (next('"'))  return parseQField();
	        return parseSField();	 
	}

	Slice  parseSField(){
                Slice field;
		field.start=_pos;
	        while(*_pos!=','&&*_pos!='\n'&&*_pos!=0) _pos++;
                field.end=_pos;
		return field;
	}

	Slice  parseQField() {
                Slice field;
                consume('"');
		field.start=_pos;
		while (*_pos!=0) {
                      if(*_pos=='"'&& *(_pos+1)=='"') {_pos+=2; continue;}
                      if(*_pos=='"') break;
                      _pos++;
		}
                field.end=_pos;
                consume('"');
                return field;

	}
       
        bool next(char ch){
            const char* p=_pos;
            while(isWhite(*p)){
                if(*p==ch) break;
                p++;
            }
            return *p==ch;
            
        }
       
	bool consume(char ch) {
            const char* p=_pos;
            while(isWhite(*p)){
                if(*p==ch) break;
                p++;
            }
            if (*p==ch) {
                 _pos=p+1;
                 return true;
            }
            return false;
	}
};
 static  vector<vector<Slice> > parse(const char* str) {
	CsvParser parser(str);
        return parser.parseCsv();
}

	  int  main() {
	        std::ifstream ifs("test.csv");
		string content;
		getline(ifs, content, (char)-1); 
		auto s = chrono::steady_clock::now(); 
		vector<vector<Slice>> r ;
		for (int i = 0; i < 1000; i++) r = parse(content.c_str());
		auto t = chrono::steady_clock::now() - s;
		for(auto row:r){
		for(auto field: row) cout<<toString(field)<<","; 
                printf("\n");
		
		}

   	cout << chrono::duration_cast<chrono::milliseconds>(t).count() << "ms" << endl;
              while(true);
              return 0;

	}


