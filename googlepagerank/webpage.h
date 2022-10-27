#ifndef WEBPAGE_H_INCLUDED
#define WEBPAGE_H_INCLUDED

#include <string>
using namespace std;

int hash(string s, int table_size);

struct Page
{
  string name;
  Page *next;
  double weight; //  used for main page
  double new_weight; //  used for main page
  int linkedpages; //  used for main page
  int identifier;  // 1- mainpage  2-hyperlink   3-word
  int index; //  used for hyperlinks, storing the index of the corresponding page in the table (-1 if not in table)
  Page(string k, Page *n)
  {
      name = k;
      next = n;
      weight=0;
      new_weight=0;
      linkedpages=0;
      identifier=0;
      index=0;
  }
  Page(string k, Page *n, int m)
  {
      name = k;
      next = n;
      weight=1.0/m;
      new_weight=0;
      linkedpages=0;
      identifier=0;
      index=0;
  }
  Page()   // function to make everything null
  {
      name = "";
      next = NULL;
      weight=0;
      new_weight=0;
      linkedpages=0;
      identifier=0;
      index=0;
  }
};

class webPage
{
    public:
	Page **table;
	int size;
	int num_elements;
	webPage(int tablesize);
	~webPage();
	int find(string key);
	void checksize();
};


#endif // WEBPAGE_H_INCLUDED
