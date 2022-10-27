#ifndef RECOMMEND_H_INCLUDED
#define RECOMMEND_H_INCLUDED

struct Trie   //trie declaration
{
    struct Trie *child[26];
    bool EOW;
};

struct Trie *getnode()      // to assign null values
{
    struct Trie *ptr=new Trie;
    ptr->EOW=false;
    for(int i=0;i<26;i++)
    {
        ptr->child[i]=nullptr;
    }
    return ptr;
}

void insert(struct Trie *root,string s)    //insertion into trie
{
   struct Trie *ptr=root;
   for(int i=0;i<s.length();i++)
   {
       int j=(int)s[i]-(int)'a';
       if(!ptr->child[j])
       {
           ptr->child[j]=getnode();
       }
       ptr=ptr->child[j];
   }
   ptr->EOW=true;   //to confirm end of the word
}

bool search(struct Trie *root,string s)       // to search in trie
{
    struct Trie *ptr=root;
    for(int i=0;i<s.length();i++)
    {
        int j=(int)s[i]-(int)'a';
        if(!ptr->child[j])
            return false;
        ptr=ptr->child[j];
    }
    return (ptr->EOW && ptr!=nullptr);
}

bool last(struct Trie *root)  //function to check if the element is last in the trie
{
    for(int i=0;i<26;i++)
    {
        if(root->child[i])
            return 0;
    }
    return 1;
}

void suggestion(struct Trie *root,string s)   //fucntion used to find other possible suggestions
{
    if(root->EOW)
    {
        cout<<s<<endl;
    }
    if(last(root))
        return;
    for(int i=0;i<26;i++)
    {
        if(root->child[i])
        {
            s.push_back(97+i);
            suggestion(root->child[i],s);
            s.pop_back();
        }
    }
}

int Recommend(struct Trie *root,string word)
{
    struct Trie *ptr=root;
    for(int i=0;i<word.length();i++)
    {
        int j=(int)word[i]-(int)'a';
        if(!ptr->child[j])
            return 0;
        ptr=ptr->child[j];
    }
    bool check_word=(ptr->EOW==true);
    bool check_last=last(ptr);
    if(check_word && check_last)
    {
        cout<<word<<endl;
        return -1;
    }
    string k;
    if(!check_last)
    {
        k=word;
        suggestion(ptr,k);
        return 1;
    }
}
#endif // RECOMMEND_H_INCLUDED
