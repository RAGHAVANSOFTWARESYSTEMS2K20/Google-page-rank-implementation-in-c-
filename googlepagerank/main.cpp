
#include <cstring>
#include <fstream>
#include <iostream>
#include "webpage.h"
#include "recommend.h"

// 2 hash tables .....  1- holds main page and points to all words and hyperlinks.
//                      2- holds words and points to all pages where the word appears.

using namespace std;
int main()
{
	cout<<endl<<endl<<"\t--------------------  GOOGLE PAGERANK ALGORITHM AND SUGGESSIONS USING TRIE  --------------------"<<endl;
	ifstream fin,file;
	string s;
	int count=0;
	struct Trie* Root=getnode();
	int counter, i, index;
	Page *temp=NULL;
	Page *foundpage=NULL;
	Page *newPage=NULL;
	Page *ptr=NULL;
	fin.open("webpages.txt");

	// count the number of pages in the file
	while (fin >> s)
    {
		if(s=="NEWPAGE")
		{
			count++;
		}
	}
	fin.close();

	// allocate a hash table twice the size of the number of pages found
	webPage Pages(count*2);

	// Load the pages into the hash table and build each page's linked list
	fin.open("webpages.txt");
	Page *word_or_link=NULL;
	cout<<endl<<"Building page hash table..."<<endl;
	while (fin >> s)
	{
		if(s=="NEWPAGE")
		{
			fin >> s;
			index=::hash(s,Pages.size);
			newPage=new Page(s,NULL,count);
			while(Pages.table[index]!=NULL)
			{
				index++;
				if(index==Pages.size)
				{
					index=0;
				}
			}
			Pages.table[index]=newPage;
			temp=Pages.table[index];
		}
		else
		{
			int found=s.find("http://");     //to check for link
			if(found != string::npos)
			{
				word_or_link=new Page(s,NULL);
				temp->next=word_or_link;
                temp=word_or_link;
				word_or_link->identifier=1;
			}
			else
			{
				word_or_link=new Page(s,NULL);  //to check for word
				temp->next=word_or_link;
				word_or_link->identifier=2;
                temp=word_or_link;
			}
		}
	}
	fin.close();

	// Set the linkedpages field of each page, which is how many hyperlinks are
	// on that page, only counting hyperlinks that point to pages with corresponding
	// NEWPAGE records for them
	for(i=0;i<Pages.size;i++)
    {
        if(Pages.table[i]!=NULL)
        {
            ptr=Pages.table[i]->next;
            while(ptr!=NULL)
            {
                if(ptr->identifier==1)
                {
                    ptr->index = Pages.find(ptr->name);
                    if(ptr->index!=-1)
                    {
                        Pages.table[i]->linkedpages++;
                    }
                }
                ptr=ptr->next;
            }
        }
    }

	// Run Pagerank algorithm
	cout<<endl<<"Running Pagerank algorithm..."<<endl;
	for(counter=0;counter<50;counter++)
	{
		for(i=0;i<Pages.size;i++)
        {
            if(Pages.table[i]!=NULL)
            {
				Pages.table[i]->new_weight=0.1/count;
			}
		}
		for(i=0;i<Pages.size;i++)
		{
			if(Pages.table[i]!=NULL)
			{
				ptr=Pages.table[i]->next;
				while(ptr!=NULL)
				{
					if(ptr->identifier==1)
					{
						if(ptr->index!=-1)
						{
							foundpage=Pages.table[ptr->index];
							foundpage->new_weight=foundpage->new_weight+0.9*Pages.table[i]->weight/Pages.table[i]->linkedpages;
						}
					}
                    ptr=ptr->next;
				}
			}
		}
		for(i=0;i<Pages.size;i++)
        {
            if(Pages.table[i]!=NULL)
            {
				Pages.table[i]->weight=Pages.table[i]->new_weight;
			}
		}
	}

	// building hash table of the words pointing to pages

	string mainpage;
	webPage Words(1000);
	fin.open("webpages.txt");
	cout<<endl<<"Building word hash table..."<<endl;
    while (fin >> s)
    {
        if(s=="NEWPAGE")
        {
			fin >> s;
			mainpage=s;
		}
		else
        {
			int found=s.find("http://"); //hyperlinks are ignored
            if(found != string::npos)
            {
				continue;
			}
			else
			{
				Words.checksize(); // if hash table gets half full, a new hash table twice the size is created
						   // and everything is rehashed into the new table
				if(Words.find(s)==-1)
				{
					Page *newWord=new Page(s,NULL);
					int wordindex=::hash(s,Words.size);     //if found empty places
                    while(Words.table[wordindex]!=NULL)
                    {
                        wordindex++;
                        if(wordindex==Words.size)
                        {
                            wordindex=0;
                        }
                    }
                    Words.table[wordindex]=newWord;
                    Words.table[wordindex]->next= new Page(mainpage,NULL);
				}
                else
                {
                    Page *alreadyintable = Words.table[Words.find(s)];    //if word is found
                    Page *pushback = alreadyintable->next;
                    alreadyintable->next= new Page(mainpage,NULL);
                    alreadyintable->next->next=pushback;
                }
			}
		}
	}
	fin.close();

	string k;                       //file reading to insert into trie
	file.open("data.txt");
	cout<<endl<<"Trie insertion started ..."<<endl;
	while (file >> k)
    {
        if(k=="NEWPAGE")
        {
			file >> k;
			mainpage=k;
		}
		else
		{
			int found=k.find("http://"); //hyperlinks are ignored
            if(found != string::npos)
            {
				continue;
			}
			else
			{
			    insert(Root,k);
            }
        }
    }
    file.close();
    cout<<endl<<"Trie insertion done...\n"<<endl;	// completed insertion on trie

	string term;
	int numpages;
	cout << "Enter the word you want to search (Enter 'Iwanttostop' to end): ";
	cin >> term;
	string str=term;

	while(term!="Iwanttostop")
    {
		numpages=0;
		if(Words.find(term)==-1)
		{
			cout<<"Word not found"<<endl;
		}
		else
		{
            Page *foundterm = Words.table[Words.find(term)];
            foundterm=foundterm->next;
            while(foundterm!=NULL)
            {
                cout << int (Pages.table[Pages.find(foundterm->name)]->weight *7100000)<< "  -  " << foundterm->name << endl;
                foundterm=foundterm->next;
                numpages++;
            }
		}
		cout<<"\nPages found: "<<numpages<<"\n"<<endl;
		str=str.substr(0,2);
		cout<<endl;
		cout<<"You can also search for the following words : \n"<<endl;
		int sugg=Recommend(Root,str);
		cout << "\nEnter the word you want to search (Enter 'Iwanttostop' to end): ";
		cin >> term;
		str=term;
	}
	if(term=="Iwanttostop")
    {
        exit(0);
    }
	return 0;
}
